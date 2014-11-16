#!/usr/bin/env python2
# -*- coding:utf-8 -*-
import json
import uuid
import requests
from PyQt4 import QtCore
from PyQt4.QtCore import pyqtSignal


class SubscribeThread(QtCore.QThread):
    new_danmaku = pyqtSignal(str, str, str, name="newDanmaku")
    _uuid = str(uuid.uuid1())

    def __init__(self, server, channel, passwd, parent=None):
        super(SubscribeThread, self).__init__(parent)
        self.server = str(server)
        self.channel = str(channel)
        self.passwd = str(passwd)

    def run(self):
        uri = "/api/v1.1/channels/{cname}/danmaku".format(cname=self.channel)
        if uri.startswith("/") and self.server.endswith("/"):
            server = self.server[:-1]
        else:
            server = self.server

        url = server + uri
        headers = {
            "X-GDANMAKU-SUBSCRIBER-ID": self._uuid,
            "X-GDANMAKU-AUTH-KEY": self.passwd,
        }

        while 1:
            try:
                res = requests.get(url, headers=headers)
            except requests.exceptions.ConnectionError:
                continue
            if res.status_code == 200 and res.text:
                try:
                    dm_opts = json.loads(res.text)
                except:
                    continue
                else:
                    for dm in dm_opts:
                        self.new_danmaku.emit(
                            dm['text'], dm['style'], dm['position'])

    def __del__(self):
        self.wait()

# vim: ts=4 sw=4 sts=4 expandtab
