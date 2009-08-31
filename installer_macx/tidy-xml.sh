#!/bin/bash
# Tidy up the xml that PackageMaker spits out.
tidy -mi -xml -utf8 MumbleInstaller.pmdoc/*.xml
