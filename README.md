
# DanmaQ ![Travis CI status](https://travis-ci.org/bigeagle/danmaQ.svg?branch=master)

DanmaQ, pronounced as `/danmakju:/` is a small QT program to play danmaku on any screen.

![](https://raw.githubusercontent.com/bigeagle/danmaQ/master/screenshots/xiaowang.png)

DanmaQ is **still under development**, documents might be outdated.

## Dependencies

`danmaQ` depends on `Qt5`. 

## Installation

### AppImage
On Linux X86-64, you can download the bundled AppImage.
   
    wget https://github.com/bigeagle/danmaQ/releases/download/cpp-0.1/danmaQ.AppImage
    chmod +x danmaQ.AppImage
    ./danmaQ.AppImage

### Source

    apt-get install qt5-default libqt5x11extras5-dev cmake  # for ubuntu and debian
    zypper install  libQt5X11Extras5 cmake          # for openSUSE
    dnf install  qt5-qtx11extras-devel cmake        # fedora
    
    mkdir build && cd build 
    cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release 
    make && sudo make install

### Windows

Download binary from [releases page](https://github.com/bigeagle/danmaQ/releases/).

### Arch Linux

    yaourt -S danmaq-git

## Usage

### Use TUNA Service

First you need to create a channel, go to http://dm.tuna.moe/ and create a channel, 
(let's use `ooxx` as the channel name and `passw0rd` as the password)

Then run `danmaQ` and fill `http://dm.tuna.moe` to server, 
and channel name (`ooxx`) & channel password (`passw0rd`). 

Then any audiences can open http://dm.tuna.moe/ and click to your channel page,
lauch danmaku in your screen!

### Self Hosted Service

See [gdanmaku-server](https://github.com/tuna/gdanmaku-server).

## TODO

- [x] Multi-Screen support
- [x] Chatting
- [ ] Deb package
- [ ] RPM package
