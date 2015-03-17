#!/bin/bash
qmake-qt5 main.pro \
      CONFIG+="bundled-celt no-bundled-opus no-bundled-speex no-speechd no-g15 \
               no-xevie no-embed-qt-translations no-update packaged no-overlay no-plugins"
exec make debug
