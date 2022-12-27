#!/usr/bin/env bash

sudo apt update

sudo apt -y install \
	build-essential \
	g++-multilib \
	ninja-build \
	pkg-config \
	qtbase5-dev \
	qttools5-dev \
	qttools5-dev-tools \
	libqt5svg5-dev \
	libboost-dev \
	libssl-dev \
	libprotobuf-dev \
	protobuf-compiler \
	libprotoc-dev \
	libcap-dev \
	libxi-dev \
	libasound2-dev \
	libasound2-plugins \
	libasound2-plugins-extra \
	libogg-dev \
	libsndfile1-dev \
	libopus-dev \
	libspeechd-dev \
	libavahi-compat-libdnssd-dev \
	libzeroc-ice-dev \
	zsync \
	appstream \
	libpoco-dev
