# Build Fedora 25 image
FROM fedora:25

RUN dnf -y install \
    gcc-c++ \
    qt5-qtx11extras-devel \
    qt5-linguist \
    qt5-devel \
    cmake

RUN mkdir -p /usr/src/app
WORKDIR /usr/src/app

COPY . /usr/src/app
