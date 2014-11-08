#!/usr/bin/env python2
# -*- coding:utf-8 -*-
from PyQt5 import QtWidgets, QtCore, QtGui
from PyQt5.QtCore import pyqtSignal
from .tray_icon import ICON_ENABLED
from .settings import load_config, save_config


class ConfigDialog(QtWidgets.QDialog):
    preferenceChanged = pyqtSignal(name="preferenceChanged")

    def __init__(self, parent=None):
        super(ConfigDialog, self).__init__(parent)
        self.setWindowTitle("Danmaku")
        self.setWindowIcon(QtGui.QIcon(ICON_ENABLED))

        self._dft = load_config()

        layout = QtWidgets.QVBoxLayout()

        hbox = QtWidgets.QHBoxLayout()
        hbox.addWidget(QtWidgets.QLabel("Font Family: "))
        self._font_family = QtWidgets.QFontComboBox(self)
        self._font_family.setCurrentFont(QtGui.QFont(self._dft['font_family']))
        hbox.addWidget(self._font_family)
        layout.addLayout(hbox)

        hbox = QtWidgets.QHBoxLayout()
        hbox.addWidget(QtWidgets.QLabel("Font Size: "))
        self._font_size = QtWidgets.QSpinBox(self)
        self._font_size.setValue(self._dft['font_size'])
        hbox.addWidget(self._font_size)
        layout.addLayout(hbox)

        hbox = QtWidgets.QHBoxLayout()
        hbox.addWidget(QtWidgets.QLabel("Speed Scale: "))
        self._speed = QtWidgets.QSlider(QtCore.Qt.Horizontal, self)
        self._speed.setTickInterval(1)
        self._speed.setMaximum(21)
        self._speed.setMinimum(4)
        self._speed.setValue(10)
        self._speed_indicator = QtWidgets.QLabel("1.0", self)
        self._speed.sliderMoved.connect(self.update_speed_indicator)
        hbox.addWidget(self._speed)
        hbox.addWidget(self._speed_indicator)
        layout.addLayout(hbox)

        hbox = QtWidgets.QHBoxLayout()
        self._save = QtWidgets.QPushButton("&Save && Apply", self)
        self._apply = QtWidgets.QPushButton("&Apply", self)
        self._cancel = QtWidgets.QPushButton("&Cancel", self)
        self._save.released.connect(self.save_preferences)
        self._apply.released.connect(self.emit_new_preferences)
        self._cancel.released.connect(self.hide)
        hbox.addWidget(self._save)
        hbox.addWidget(self._apply)
        hbox.addWidget(self._cancel)
        layout.addLayout(hbox)

        self.setLayout(layout)

    def update_speed_indicator(self):
        val = self._speed.value()
        self._speed_indicator.setText("{:.1f}".format(val/10.0))

    def preferences(self):
        return {
            'font_family': self._font_family.currentText(),
            'font_size': self._font_size.value(),
            'speed_scale': self._speed.value() / 10.0,
        }

    def save_preferences(self):
        opts = load_config()
        new_opts = self.preferences()
        opts['font_family'] = new_opts['font_family']
        opts['font_size'] = new_opts['font_size']
        opts['speed_scale'] = new_opts['speed_scale']
        save_config(opts)
        self.emit_new_preferences()

    def emit_new_preferences(self):
        self.preferenceChanged.emit()
        self.hide()

# vim: ts=4 sw=4 sts=4 expandtab
