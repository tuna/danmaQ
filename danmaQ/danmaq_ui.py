#!/usr/bin/env python3
# -*- coding:utf-8 -*-
import sys
from random import randint
from threading import Lock

from PyQt4 import QtCore, QtGui
from PyQt4.QtCore import pyqtSignal

from .settings import load_config

color_styles = {
    "white": ('rgb(255, 255, 255)', QtGui.QColor("black"), ),
    "black": ('rgb(0, 0, 0)', QtGui.QColor("white"), ),
    "blue": ('rgb(20, 95, 198)', QtGui.QColor("white"), ),
    "cyan": ('rgb(0, 255, 255)', QtGui.QColor("black"), ),
    "red": ('rgb(231, 34, 0)', QtGui.QColor("white"), ),
    "yellow": ('rgb(255, 221, 2)', QtGui.QColor("black"), ),
    "green": ('rgb(4, 202, 0)', QtGui.QColor("black"), ),
    "purple": ('rgb(128, 0, 128)', QtGui.QColor("white"), ),
}

OPTIONS = load_config()

if sys.platform == "win32":
    import win32api


class Danmaku(QtGui.QLabel):
    _lock = Lock()
    vertical_slots = None

    _font_family = OPTIONS['font_family']
    _speed_scale = OPTIONS['speed_scale']
    _font_size = OPTIONS['font_size']
    _interval = 10
    _style_tmpl = "font-size: {font_size}pt;" \
        + "font-family: {font_family};" \
        + "font-weight: bold;" \
        + "color: {color}; "

    exited = pyqtSignal(str, name="exited")

    @classmethod
    def set_options(cls, opts):
        cls._font_family = opts['font_family']
        cls._font_size = opts['font_size']
        cls._speed_scale = opts['speed_scale']

    def __init__(self, text="text", style='white', position='fly', parent=None):
        super(Danmaku, self).__init__(text, parent)

        self._text = text
        self._style = style
        self._position = position

        self.setWindowTitle("Danmaku")
        self.setStyleSheet("background:transparent; border:none;")
        self.setAttribute(QtCore.Qt.WA_TranslucentBackground, True)
        self.setAttribute(QtCore.Qt.WA_DeleteOnClose, True)

        self.setWindowFlags(
            QtCore.Qt.ToolTip
            | QtCore.Qt.FramelessWindowHint
        )

        if sys.platform == "win32":
            # Win32 Dark Magic, disable window drop shadows
            win32api.SetClassLong(
                self.winId().__int__(),
                -26,
                0x0008 & ~0x00020000)

        self.init_text(text, style)

        self._width = self.frameSize().width()
        self._height = self.frameSize().height()
        self.screenGeo = QtGui.QDesktopWidget().screenGeometry()

        with Danmaku._lock:
            if Danmaku.vertical_slots is None:
                Danmaku.vertical_slots = [0] * \
                    int((self.screenGeo.height() - 20) / self._height)

        self.quited = False
        self.position_inited = False
        self.init_position()

    def init_text(self, text, style):
        # self.label = QtGui.QLabel(text, parent=self)
        tcolor, bcolor = color_styles.get(style, color_styles['white'])

        effect = QtGui.QGraphicsDropShadowEffect(self)
        effect.setBlurRadius(7)
        effect.setColor(bcolor)
        effect.setOffset(0, 0)

        self.setStyleSheet(
            self._style_tmpl.format(
                font_size=self._font_size,
                font_family=self._font_family,
                color=tcolor,
            )
        )

        self.setGraphicsEffect(effect)
        self.setContentsMargins(0, 0, 0, 0)

        # layout = QtGui.QVBoxLayout()
        # layout.addWidget(self.label, 0, QtCore.Qt.AlignVCenter)
        # layout.setContentsMargins(0, 0, 0, 0)
        # self.setLayout(layout)

        _msize = self.minimumSizeHint()
        # _msize.setHeight(self.label.height()+16)
        self.resize(_msize)

    def init_position(self):
        self.vslot = None
        self.show()
        if self._position == 'fly':
            self.x = self.screenGeo.width()
            self.y = randint(20, self.screenGeo.height()-self._font_size-20)
            self.step = min(len(self._text) * 0.05 + 1.5, 10) * self._speed_scale
            QtCore.QTimer.singleShot(self._interval, self.fly)

        elif self._position == 'bottom':
            self.x = (self.screenGeo.width() - self._width) / 2
            with Danmaku._lock:
                i = 0
                for i, v in enumerate(Danmaku.vertical_slots[::-1]):
                    if v == 0:
                        Danmaku.vertical_slots[-(i+1)] = 1
                        self.vslot = -(i+1)
                        break
                else:
                    self.hide()
                    QtCore.QTimer.singleShot(1000, self.init_position)
                    return

                self.y = self.screenGeo.height() + self._height * self.vslot - 10
                QtCore.QTimer.singleShot(5000, self.clean_close)

        elif self._position == 'top':
            self.x = (self.screenGeo.width() - self._width) / 2
            with Danmaku._lock:
                i = 0
                for i, v in enumerate(Danmaku.vertical_slots):
                    if v == 0:
                        Danmaku.vertical_slots[i] = 1
                        self.vslot = i
                        break
                else:
                    self.hide()
                    QtCore.QTimer.singleShot(1000, self.init_position)
                    return

                self.y = self._height * self.vslot
                QtCore.QTimer.singleShot(5000, self.clean_close)

        self.move(self.x, self.y)
        self.position_inited = True

    def fly(self):
        _x = int(self.x)
        self.x -= self.step
        x_dst = int(self.x)
        if self.x < -self._width:
            self.clean_close()
        else:
            QtCore.QTimer.singleShot(self._interval, self.fly)

        if _x != x_dst:
            self.move(x_dst, self.y)

    def clean_close(self):
        if self.quited is False:
            self.quited = True

            with Danmaku._lock:
                # print(Danmaku.count)
                if self.vslot is not None:
                    Danmaku.vertical_slots[self.vslot] = 0

            self.exited.emit(str(id(self)))
            self.destroy()


class DanmakuTestApp(QtGui.QDialog):
    def __init__(self, parent=None):
        super(DanmakuTestApp, self).__init__(parent)
        self.setWindowTitle("Danmaku")
        self.lineedit = QtGui.QLineEdit("Text")
        self.style = QtGui.QLineEdit("blue")
        self.position = QtGui.QLineEdit("top")
        self.pushbutton = QtGui.QPushButton("Send")
        layout = QtGui.QVBoxLayout()
        layout.addWidget(self.lineedit)
        layout.addWidget(self.style)
        layout.addWidget(self.position)
        layout.addWidget(self.pushbutton)
        self.setLayout(layout)
        self.pushbutton.clicked.connect(self.new_danmaku)
        self.dms = {}

    def new_danmaku(self):
        text = self.lineedit.text()
        style = self.style.text()
        position = self.position.text()
        dm = Danmaku(text, style=style, position=position, parent=self)
        dm.exited.connect(self.delete_danmaku)
        self.dms[str(id(dm))] = dm
        dm.show()

    def delete_danmaku(self, _id):
        dm = self.dms.pop(_id)
        dm.close()


def dm_test():
    import signal
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    signal.signal(signal.SIGTERM, signal.SIG_DFL)
    app = QtGui.QApplication(sys.argv)
    danmakuTestApp = DanmakuTestApp()
    danmakuTestApp.show()
    sys.exit(app.exec_())

if __name__ == "__main__":
    dm_test()

# vim: ts=4 sw=4 sts=4 expandtab
