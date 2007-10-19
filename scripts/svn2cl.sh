#! /bin/bash

svn update
svn log -v --xml | perl scripts/svn2cl.pl
svn -m "Changelog update" commit
