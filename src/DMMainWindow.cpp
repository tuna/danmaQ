/*
 * This file is part of danmaQ.
 * 
 * DanmaQ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * DanmaQ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QCheckBox>

#include "common.hpp"

DMMainWindow::DMMainWindow(QApplication *app)
{

	this->app = app;

	this->setWindowTitle("DanmaQ");
	this->setWindowIcon(QIcon(":icon_active.png"));
	this->trayIcon = new DMTrayIcon(this);

	QVBoxLayout *layout = new QVBoxLayout(this);

	QHBoxLayout *hbox = new QHBoxLayout(this);
	hbox->addWidget(new QLabel(tr("Server: "), this));
	this->server = new QLineEdit("danmaQ://", this);
	this->server->setMinimumWidth(200);
	hbox->addWidget(this->server);
	layout->addLayout(hbox);

	hbox = new QHBoxLayout(this);
	this->hideBtn = new QPushButton(tr("Hide"), this);
	this->mainBtn = new QPushButton(tr("Subscribe"), this);
	hbox->addWidget(this->hideBtn);
	hbox->addWidget(this->mainBtn);
	layout->addLayout(hbox);

	this->setLayout(layout);

	this->fontSize = 30;
	this->lineHeight = this->fontSize * 1.2;
	this->fontFamily = QString(
		"WenQuanYi Micro Hei, Source Han Sans CN, Source Han Sans, WenQuanYi Zen Hei,"
		"Microsoft YaHei, SimHei, "
		"STHeiti, Hiragino Sans GB, "
		"sans-serif");
	this->speedScale = 1.0;

	connect(this->mainBtn, &QPushButton::released, this, &DMMainWindow::toggle_subscription);
	connect(this->hideBtn, &QPushButton::released, this, &DMMainWindow::hide);
	connect(this->trayIcon->refreshScreenAction, &QAction::triggered, this, &DMMainWindow::reset_canvases);
	connect(this->trayIcon->showAction, &QAction::triggered, this, &DMMainWindow::show);
	connect(this->trayIcon->aboutAction, &QAction::triggered, this, &DMMainWindow::show_about_dialog);
	connect(this->trayIcon->exitAction, &QAction::triggered, this->app, &QApplication::quit);

	auto primaryScreen = QGuiApplication::primaryScreen();
	auto center = primaryScreen->geometry().center();
	this->move(center.x() - this->width() / 2, center.y() - this->height() / 2);
	this->show();
}

void DMMainWindow::toggle_subscription()
{
	if (this->subscriber == nullptr)
	{
		myDebug << "new Subscriber";
		this->subscriber = new Subscriber(server->text(), this);
		for (auto w = this->dm_canvases.begin(); w != this->dm_canvases.end(); ++w)
		{
			connect(
				this->subscriber, &Subscriber::new_danmaku,
				qobject_cast<DMCanvas *>(*w), &DMCanvas::new_danmaku);
		}
		connect(
			this->subscriber, &Subscriber::started,
			this, &DMMainWindow::on_subscription_started);
		connect(
			this->subscriber, &Subscriber::finished,
			this, &DMMainWindow::on_subscription_stopped);
		connect(
			this->subscriber, &Subscriber::new_alert,
			this, &DMMainWindow::on_new_alert);
		this->subscriber->start();
		this->reset_canvases();
	}
	else
	{
		this->subscriber->finish();
		emit stop_subscription();
		this->subscriber = nullptr;
	}
}

void DMMainWindow::init_canvases()
{
	this->screenWidget->hide();
	auto screens = QApplication::screens();
	for (int i = 0; i < screens.size(); i++)
	{
		if (this->screenBoxes[i]->checkState())
		{
			auto screen = screens[i];
			DMCanvas *canvas = new DMCanvas(screen, this);
			this->dm_canvases.append(canvas);
			if (this->subscriber != nullptr)
			{
				connect(
					this->subscriber, &Subscriber::new_danmaku,
					canvas, &DMCanvas::new_danmaku);
			}
			this->app->connect(
				screen, &QScreen::availableGeometryChanged,
				this, &DMMainWindow::reset_canvases);
			canvas->new_danmaku(tr("Screen") + QString::number(i), 0xFFFFFF, 5);
		}
	}
}

void DMMainWindow::reset_canvases()
{
	myDebug << "Resetting canvases";

	for (auto w = this->dm_canvases.begin(); w != this->dm_canvases.end(); ++w)
	{
		delete *w;
	}
	this->dm_canvases.clear();

	if (screenWidget != nullptr)
	{
		delete this->screenWidget;
		this->screenWidget = nullptr;
	}
	this->screenBoxes.clear();

	this->screenWidget = new QWidget;
	this->screenWidget->setWindowTitle(tr("Screen configuration"));
	this->screenWidget->setMinimumWidth(300);
	QVBoxLayout *layout = new QVBoxLayout;

	auto screens = QGuiApplication::screens();
	for (int i = 0; i < screens.size(); i++)
	{
		int x = screens[i]->size().width(), y = screens[i]->size().height();
		QCheckBox *box = new QCheckBox(tr("Screen") + QString("%1: (%2, %3)").arg(i).arg(x).arg(y), this->screenWidget);
		this->screenBoxes.append(box);
		layout->addWidget(box);
	}

	QPushButton *screenBtn = new QPushButton(tr("Submit"), this);
	layout->addWidget(screenBtn);
	this->screenWidget->setLayout(layout);

	connect(screenBtn, &QPushButton::released, this, &DMMainWindow::init_canvases);

	auto primaryScreen = QGuiApplication::primaryScreen();
	auto center = primaryScreen->geometry().center();
	screenWidget->move(center.x() - this->width() / 2, center.y() - this->height() / 2);
	screenWidget->show();
	screenWidget->raise();
	screenWidget->setFocus();
}

void DMMainWindow::on_subscription_started()
{
	myDebug << "Subscription Started";
	this->hide();
	this->trayIcon->set_icon_running();
	this->mainBtn->setText(tr("Unsubscribe"));
	this->trayIcon->showMessage(tr("Subscription Started"), tr("Let's Go"));
}

void DMMainWindow::on_subscription_stopped()
{
	myDebug << "Subscription Stopped";
	this->trayIcon->set_icon_stopped();
	this->mainBtn->setText(tr("Subscribe"));
}

void DMMainWindow::on_new_alert(QString msg)
{
	myDebug << "Alert:" << msg;
	this->trayIcon->showMessage(tr("Ooops!"), msg, QSystemTrayIcon::Critical);
	this->subscriber->finish();
	emit stop_subscription();
	this->subscriber = nullptr;
}

void DMMainWindow::show_about_dialog()
{
	this->show();
	QMessageBox::about(
		this, tr("About"),
		R"(
                <strong>DanmaQ</strong>
                <p>Version )" DANMAQ_VERSION R"( </p>
                <p>Copyright &copy; 2015-2021 Justin Wong & TUNA members<br />
                Tsinghua University TUNA Association</p>
                <p> Source Code Available under GPLv3<br />
                <a href='https://github.com/tuna/danmaQ'>
                https://github.com/tuna/danmaQ
                </a></p>
                )");
}

DMTrayIcon::DMTrayIcon(QWidget *parent)
	: QSystemTrayIcon(parent)
{
	this->icon_running = QIcon(":icon_active.png");
	this->icon_stopped = QIcon(":icon_inactive.png");
	this->setIcon(this->icon_stopped);

	QMenu *menu = new QMenu(tr("Settings"));
	this->showAction = menu->addAction(tr("Show Main Window"));
	this->refreshScreenAction = menu->addAction(tr("Refresh Screen"));
	this->aboutAction = menu->addAction(tr("About"));
	this->exitAction = menu->addAction(tr("Exit"));

	this->setContextMenu(menu);

	connect(
		this, &DMTrayIcon::activated,
		this, &DMTrayIcon::on_activated);
	this->show();
}

void DMTrayIcon::on_activated(QSystemTrayIcon::ActivationReason e)
{
	if (e == this->Trigger)
	{
		auto *parent = qobject_cast<QWidget *>(this->parent());
		if (parent == nullptr)
		{
			return;
		}
		if (parent->isVisible())
		{
			parent->hide();
		}
		else
		{
			parent->show();
		}
	}
}

void DMTrayIcon::set_icon_running()
{
	this->setIcon(this->icon_running);
}

void DMTrayIcon::set_icon_stopped()
{
	this->setIcon(this->icon_stopped);
}
