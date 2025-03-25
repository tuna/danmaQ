
# DanmaQ  

[![Ubuntu](https://github.com/tuna/danmaQ/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/tuna/danmaQ/actions/workflows/ubuntu.yml)
[![macOS](https://github.com/tuna/danmaQ/actions/workflows/macOS.yml/badge.svg)](https://github.com/tuna/danmaQ/actions/workflows/macOS.yml)
[![Windows](https://github.com/tuna/danmaQ/actions/workflows/windows.yml/badge.svg)](https://github.com/tuna/danmaQ/actions/workflows/windows.yml)

DanmaQ, pronounced as `/danmakju:/` is a small QT program to play danmaku on any screen.

![](https://raw.githubusercontent.com/tuna/danmaQ/master/screenshots/xiaowang.png)

DanmaQ is **still under development**, documents might be outdated.

## Dependencies

danmaQ depends on Qt6. Qt 6.8 LTS is recommended.

## Installation

### Source

```shell
apt-get install qt6-base-dev qt6-webengine-dev qt6-tools-dev-tools cmake ninja-build # for debian / ubuntu
dnf install qt6-qtbase-devel qt6-qtwebengine-devel qt6-linguist cmake ninja-build    # for fedora
brew install qt cmake                                                                # for macOS

mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release 
make && sudo make install
```

### Windows

Download binary from [releases page](https://github.com/tuna/danmaQ/releases/).

Or you can also use CMake and Qt to build it yourself, but there is no help available temporarily.

### Arch Linux

```shell
pacman -S danmaq
```

### Fedora

```shell
dnf install danmaq
```

### Debian / Ubuntu / AOSC OS

```shell
apt install danmaq
```

## Usage

### TUNA Public Service

First you need to create a channel, go to https://dm.tuna.moe:8443/ and create a channel, 
(let's use `ooxx` as the channel name and `passw0rd` as the password)

Then run `danmaQ` and fill `https://dm.tuna.moe:8443` to server, 
and channel name (`ooxx`) & channel password (`passw0rd`). 

Then any audiences can open https://dm.tuna.moe:8443/ and click to your channel page,
lauch danmaku in your screen!

### Self Hosted Service

See [gdanmaku-server](https://github.com/tuna/gdanmaku-server).
