#! /bin/bash

svn update
svn log -v --xml | python svn2log.py -s -O -L -H -p '/(branches/[^/]+|trunk)/' -D 86400 -o /dev/stdout -o CHANGES
