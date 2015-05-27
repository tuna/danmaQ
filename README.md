# DanmaQ

DanmaQ, pronounced as `/danmakju:/` is a small QT program to play danmaku on any screen.

## Warning

DanmaQ is still under development, documents might be outdated.

## Run Demo

### Dependencies

`danmaQ` depends on `Qt4` and `qjson`. 

if you use Windows, download bundled binary version from 
[releases page](https://github.com/bigeagle/danmaQ/releases/).

### Use TUNA Service

First u need to create a channel, go to http://dm.tuna.moe/ and create a channel, 
(let's use `ooxx` as the channel name and `passw0rd` as the password)

then run `danmaQ` and fill `http://dm.tuna.moe` to server, 
and your channel name (`ooxx`) and channel password (`passw0rd)`. 

then open http://dm.tuna.moe/ and click to your channel page, then post.

### Self Hosted Service

Clone https://github.com/tuna/gdanmaku-server and run `webserver.py` to start a publishing server.

### Installation

#### From source

````
apt-get install libqjson0-dev cmake  # for ubuntu and debian
zypper install libqjson cmake        # for openSUSE
dnf install qjson-devel cmake        # fedora

mkdir build && cd build 
cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release 
make && sudo make install
````

#### Windows binary

Download from https://github.com/bigeagle/danmaQ/releases/

#### ArchLinux

```
yaourt -S danmaq-git
```

## TODO

- [x] Multi-Screen support
- [x] Chatting
- [ ] Deb package
- [ ] RPM package

## Screenshot

![](https://raw.githubusercontent.com/bigeagle/danmaQ/master/screenshots/xiaowang.png)
