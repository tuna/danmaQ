# DanmaQ

DanmaQ, pronounced as `/danmakju:/` is a small QT program to play danmaku on any screen.

## Warning

DanmaQ is still under development, documents might be outdated.

## Run Demo

### Dependencies

`danmaQ` depends on `requests` and `PyQt5`, you can either install via `pip` or system package manager,
if you use Windows, please download and install python3.4 and PyQt5 manually.

### Use TUNA Service

First u need to create a channel, go to http://dm.tuna.moe/ and create a channel, 
(let's use `ooxx` as the channel name and `passw0rd` as the password)

then run `python danmaQ.py` and fill `http://dm.tuna.moe` to server, 
and your channel name (`ooxx`) and channel password (`passw0rd)`.

then open http://dm.tuna.moe/ and click to your channel page, then post.

### Server Hosted Service

Clone https://github.com/tuna/gdanmaku-server and run `webserver.py` to start a publishing server.

### Installation

run `python setup.py install`.

## Screenshot

![](https://raw.githubusercontent.com/bigeagle/danmaQ/master/screenshots/xiaowang.png)
