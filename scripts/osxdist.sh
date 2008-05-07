#!/bin/bash
ver=`cat src/mumble/Global.h | grep '#define MUMBLE_VERSION' | sed 's,.*MUMBLE_VERSION\ ,,'`
if [ "$ver" != "" ]; then
	python scripts/osxdist.py $ver
else
	python scripts/osxdist.py
fi
