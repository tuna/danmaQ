#!/usr/bin/env python3
# -*- coding:utf-8 -*-
import sys
from PyQt4 import QtGui
from datetime import datetime
from . import __version__
from .danmaq_ui import Danmaku
from .tray_icon import DanmaQTrayIcon, ICON_ENABLED
from .settings import load_config, save_config
from .config_dialog import ConfigDialog
from .subscriber import SubscribeThread


class DanmakuApp(QtGui.QWidget):
    def __init__(self, parent=None):
        super(DanmakuApp, self).__init__(parent)
        self.setWindowTitle("Danmaku")
        self.setWindowIcon(QtGui.QIcon(ICON_ENABLED))

        self.trayIcon = DanmaQTrayIcon(self)
        self.trayIcon.show()
        self.config_dialog = ConfigDialog(self)
        self._options = load_config()

        layout = QtGui.QVBoxLayout()
        hbox = QtGui.QHBoxLayout()
        hbox.addWidget(QtGui.QLabel("Server: ", self))
        self._server = QtGui.QLineEdit(
            self._options['http_stream_server'], self)
        hbox.addWidget(self._server)
        layout.addLayout(hbox)

        hbox = QtGui.QHBoxLayout()
        hbox.addWidget(QtGui.QLabel("Save As Default Server: ", self))
        self._save_server = QtGui.QCheckBox(self)
        hbox.addWidget(self._save_server)
        layout.addLayout(hbox)

        hbox = QtGui.QHBoxLayout()
        hbox.addWidget(QtGui.QLabel("Channel: ", self))
        self._chan = QtGui.QLineEdit("demo", self)
        hbox.addWidget(self._chan)
        layout.addLayout(hbox)

        hbox = QtGui.QHBoxLayout()
        hbox.addWidget(QtGui.QLabel("Password: ", self))
        self._passwd = QtGui.QLineEdit("", self)
        self._passwd.setEchoMode(QtGui.QLineEdit.Password)
        hbox.addWidget(self._passwd)
        layout.addLayout(hbox)

        hbox = QtGui.QHBoxLayout()
        self.config_button = QtGui.QPushButton("&Preferences", self)
        self.hide_button = QtGui.QPushButton("&Hide", self)
        self.main_button = QtGui.QPushButton("&Subscribe", self)
        hbox.addWidget(self.config_button)
        hbox.addWidget(self.hide_button)
        hbox.addWidget(self.main_button)
        layout.addLayout(hbox)
        self.setLayout(layout)

        self.config_button.released.connect(self.config_dialog.show)
        self.hide_button.released.connect(self.hide)
        self.main_button.released.connect(self.subscribe_danmaku)
        self.config_dialog.preferenceChanged.connect(self.apply_new_preference)
        self.trayIcon.toggleAction.triggered.connect(self.subscribe_danmaku)
        self.trayIcon.showAction.triggered.connect(self.show)
        self.trayIcon.configAction.triggered.connect(self.config_dialog.show)
        self.trayIcon.aboutAction.triggered.connect(self.show_about_dialog)
        self.trayIcon.exitAction.triggered.connect(self.close)

        self.workThread = None
        self.dms = {}

    def place_center(self):
        # Align Center
        screenGeo = QtGui.QDesktopWidget().screenGeometry()
        self.move(
            screenGeo.width() / 2 - self.width() / 2,
            screenGeo.height() / 2 - self.height() / 2,
        )

    def subscribe_danmaku(self):
        if self.workThread is None or self.workThread.isFinished():
            self.workThread = SubscribeThread(
                self._server.text(),
                self._chan.text(),
                self._passwd.text(),
                parent=self,
            )
            self.workThread.started.connect(self.on_subscription_started)
            self.workThread.finished.connect(self.on_subscription_finished)
            self.workThread.start()
            self.workThread.new_danmaku.connect(self.on_new_danmaku)
            self.hide()
        else:
            self.workThread.terminate()

    def on_new_danmaku(self, text, style, position):
        dm = Danmaku(text, style=style, position=position, parent=self)
        dm.exited.connect(self.delete_danmaku)
        self.dms[str(id(dm))] = dm

    def delete_danmaku(self, _id):
        self.dms.pop(str(_id))

    def on_subscription_started(self):
        if self._save_server.isChecked():
            opts = load_config()
            opts['http_stream_server'] = self._server.text()
            save_config(opts)

        self.main_button.setText("Unsubscribe")
        self.trayIcon.set_icon_running()
        self.trayIcon.showMessage(
            "DanmaQ",
            "Subscribing danmaku from {}".format(self.workThread.server)
        )

    def on_subscription_finished(self):
        _dms = [dm for _, dm in self.dms.items()]
        for dm in _dms:
            dm.hide()
            dm.clean_close()
        self.trayIcon.set_icon_not_running()
        self.main_button.setText("Subscribe")
        self.trayIcon.showMessage("DanmaQ", "Subscription Finished")

    def apply_new_preference(self):
        pref = self.config_dialog.preferences()
        Danmaku.set_options(pref)

    def show_about_dialog(self):
        QtGui.QMessageBox.about(
            self,
            "About DanmaQ",
            """
            <strong>DanmaQ</strong>
            <p>Version: {version} <br />
            Copyright &copy; {year} Justin Wong <br />
            Tsinghua University TUNA Association
            </p>
            <p>
            Source Code Available under GPLv3
            <br />
            <a href="https://github.com/bigeagle/danmaQ">
                https://github.com/bigeagle/danmaQ
            </a>
            </p>
            """.format(
                version=__version__,
                year=datetime.now().strftime("%Y"),
            )
        )


def main():
    import signal
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    signal.signal(signal.SIGTERM, signal.SIG_DFL)
    app = QtGui.QApplication(sys.argv)
    danmakuApp = DanmakuApp()
    danmakuApp.show()
    danmakuApp.place_center()
    sys.exit(app.exec_())

if __name__ == "__main__":
    main()
# vim: ts=4 sw=4 sts=4 expandtab
