# Build Ubuntu 18.04 image
FROM ubuntu:18.04

# see https://docs.docker.com/engine/userguide/eng-image/dockerfile_best-practices/#/run
RUN apt-get update && \
  apt-get install -y --no-install-recommends \
  g++ \
  qt5-default \
  qttools5-dev-tools \
  libqt5x11extras5-dev \
  make \
  cmake

RUN mkdir -p /usr/src/app
WORKDIR /usr/src/app

COPY . /usr/src/app
