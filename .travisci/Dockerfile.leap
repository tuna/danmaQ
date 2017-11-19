
# Build the latest openSUSE Leap (42.x) image
FROM opensuse:leap

RUN zypper --non-interactive in --no-recommends \
    gcc-c++ \
    libqt5-qtx11extras-devel \
    libqt5-linguist-devel \
    libqt5-qtbase-devel \
    cmake

RUN mkdir -p /usr/src/app
WORKDIR /usr/src/app

COPY . /usr/src/app
