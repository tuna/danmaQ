#!/usr/bin/env python3
# -*- coding:utf-8 -*-
from __future__ import (absolute_import, division,
                        print_function, unicode_literals)
import os
import sys
import json

DEFAULT_OPTIONS = {
    'http_stream_server': "http://dm.tuna.moe",
    'font_family': "WenQuanYi Micro Hei",
    'font_size': 28,
    'speed_scale': 1.0,
    'to_extend_screen': False,
}

_xdg_cfg_dir = os.environ.get(
    "XDG_CONFIG_HOME", os.path.join(os.path.expanduser("~"), ".config"))

_cfg_file = os.path.join(_xdg_cfg_dir, "danmaQ", "config.json")


def load_config():
    options = dict(DEFAULT_OPTIONS.items())

    if os.path.exists(_cfg_file):
        try:
            with open(_cfg_file, 'rb') as f:
                if sys.version_info[0] == 3:
                    s = bytes(f.read()).decode('utf-8')
                    opts = json.loads(s)
                else:
                    opts = json.load(f)
            options['to_extend_screen'] = opts['to_extend_screen']
            options['font_family'] = opts['font_family']
            options['font_size'] = opts['font_size']
            options['speed_scale'] = opts['speed_scale']
            options['http_stream_server'] = opts['http_stream_server']
        except:
            options = dict(DEFAULT_OPTIONS.items())
            save_config(options)
    else:
        if not os.path.exists(os.path.dirname(_cfg_file)):
            os.makedirs(os.path.dirname(_cfg_file))
        save_config(options)

    return options


def save_config(options):
    with open(_cfg_file, 'wb') as f:
        if sys.version_info[0] == 3:
            s = json.dumps(options, indent=4)
            f.write(bytes(s, 'utf-8'))
        else:
            json.dump(options, f, indent=4)


class MultiscreenManager(object):
    def __init__(self):
        self._primary_screen = 0
        self._geoms = []

    @property
    def primary_screen(self):
        return self._primary_screen

    def geometry(self, idx):
        return self._geoms[idx]

    def populate_geometries(self, geoms):
        self._geoms = geoms

    def set_primary(self, idx):
        self._primary_screen = idx

    def get_screen_idx(self, secondary=False):
        if len(self._geoms) < 2:
            return 0

        if len(self._geoms) >= 3:
            raise NotImplementedError

        if secondary:
            return 1 if self._primary_screen == 0 else 0
        else:
            return self._primary_screen

    def get_offset_x(self, idx, reversed=False):
        geom = self.geometry(idx)
        return geom.x() if not reversed else (geom.x() - geom.width())

    def get_origin_y(self, idx):
        geom = self.geometry(idx)
        return geom.y()


multiscreen_manager = MultiscreenManager()


# vim: ts=4 sw=4 sts=4 expandtab
