
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

DanmaQ >= 0.3 works as a client of [Comment9](https://github.com/prnake/Comment9).

DanmaQ < 0.3 should be used as a client to [gdanmaku-server](https://github.com/tuna/gdanmaku-server).
