#! /bin/bash

cvs update -Pd .
scripts/cvs2cl.pl -f CHANGES .
cvs commit -m "" CHANGES
