#!/usr/bin/env python2
# -*- coding:utf-8 -*-
import os
from PyQt5 import QtWidgets, QtGui

ICON_DIR = os.path.join(
    os.path.dirname(os.path.abspath(__file__)), "images")

ICON_ENABLED = os.path.join(ICON_DIR, "statusicon.png")
ICON_DISABLED = os.path.join(ICON_DIR, "statusicon_disabled.png")


class DanmaQTrayIcon(QtWidgets.QSystemTrayIcon):

    def __init__(self, parent=None):
        self._icon = QtGui.QIcon(ICON_DISABLED)
        super(DanmaQTrayIcon, self).__init__(self._icon, parent)
        menu = QtWidgets.QMenu(parent)
        self.toggleAction = menu.addAction("Toggle Subscription")
        self.showAction = menu.addAction("Show Main Window")
        self.exitAction = menu.addAction("Exit")
        self.setContextMenu(menu)
        self.activated.connect(self.on_activate)

    def on_activate(self, reason):
        if reason == self.Trigger:
            if self.parent().isVisible():
                self.parent().hide()
            else:
                self.parent().show()

    def set_icon_not_running(self):
        self.setIcon(QtGui.QIcon(ICON_DISABLED))

    def set_icon_running(self):
        self.setIcon(QtGui.QIcon(ICON_ENABLED))

# vim: ts=4 sw=4 sts=4 expandtab
