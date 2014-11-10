#!/usr/bin/env python3
# -*- coding:utf-8 -*-
import os
import sys
from PyQt4 import QtGui

__dir = os.path.dirname(sys.executable) \
    if getattr(sys, 'frozen', False) else \
    os.path.dirname(os.path.realpath(__file__))

ICON_DIR = os.path.join(__dir, "images")

ICON_ENABLED = os.path.join(ICON_DIR, "statusicon.png")
ICON_DISABLED = os.path.join(ICON_DIR, "statusicon_disabled.png")


class DanmaQTrayIcon(QtGui.QSystemTrayIcon):

    def __init__(self, parent=None):
        self._icon = QtGui.QIcon(ICON_DISABLED)
        super(DanmaQTrayIcon, self).__init__(self._icon, parent)
        menu = QtGui.QMenu(parent)
        self.toggleAction = menu.addAction("Toggle Subscription")
        self.showAction = menu.addAction("Show Main Window")
        self.configAction = menu.addAction("Preferences")
        self.aboutAction = menu.addAction("About")
        self.exitAction = menu.addAction("Exit")
        self.setContextMenu(menu)
        self.activated.connect(self.on_activate)
        self.show()

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
