#!/usr/bin/env python2
# -*- coding:utf-8 -*-
import sys
from PyQt5 import QtWidgets, QtCore


class Danmaku(QtWidgets.QWidget):
    def __init__(self, text="text", parent=None):
        super(Danmaku, self).__init__(parent)
        self.label = QtWidgets.QLabel(text, parent=self)
        layout = QtWidgets.QVBoxLayout()
        layout.addWidget(self.label)
        self.setLayout(layout)

        self.setWindowFlags(QtCore.Qt.ToolTip
                            |QtCore.Qt.FramelessWindowHint)

        self.resize(self.label.minimumSizeHint())
        self.setWindowTitle("Danmaku")
        self.closed = False
        # QtCore.QTimer.singleShot(20, self.refresh)
        QtCore.QTimer.singleShot(10, self.fly)

        self.width = self.frameSize().width()
        self.height = self.frameSize().height()
        self.screenGeo = QtWidgets.QDesktopWidget().screenGeometry()
        self.x = self.screenGeo.width()
        self.y = self.screenGeo.height() / 2 - self.height/2
        self.move(self.x, self.y)

        self.setStyleSheet("background:transparent")
        self.setAttribute(QtCore.Qt.WA_DeleteOnClose, True)
        self.setAttribute(QtCore.Qt.WA_TranslucentBackground, True)

    def fly(self):
        self.x -= 5
        self.move(self.x, self.y)
        if self.x < -self.width:
            self.clean_close()
        else:
            QtCore.QTimer.singleShot(10, self.fly)

    def clean_close(self):
        self.parent().delete_danmaku(id(self))
        self.destroy()
        # self.close()


class DanmakuApp(QtWidgets.QDialog):
    def __init__(self, parent=None):
        super(DanmakuApp, self).__init__(parent)
        self.setWindowTitle("Danmaku")
        self.lineedit = QtWidgets.QLineEdit("Text")
        self.pushbutton = QtWidgets.QPushButton("Send")
        layout = QtWidgets.QVBoxLayout()
        layout.addWidget(self.lineedit)
        layout.addWidget(self.pushbutton)
        self.setLayout(layout)
        self.pushbutton.clicked.connect(self.new_danmaku)
        self.dms = {}

    def new_danmaku(self):
        text = self.lineedit.text()
        dm = Danmaku(
            u"<font size=28 color=red><b>{}</b></font>".format(text),
            parent=self)
        self.dms[id(dm)] = dm
        dm.show()

    def delete_danmaku(self, _id):
        dm = self.dms.pop(_id)
        dm.close()


if __name__ == "__main__":
    import signal
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    signal.signal(signal.SIGTERM, signal.SIG_DFL)
    app = QtWidgets.QApplication(sys.argv)
    danmakuApp = DanmakuApp()
    danmakuApp.show()
    sys.exit(app.exec_())

# vim: ts=4 sw=4 sts=4 expandtab
