#! /usr/bin/perl
#
# Copyright 2005-2016 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

use warnings;
use strict;

open(F, ">../src/mumble/mumble_flags.qrc");
print F qq|<RCC><qresource>\n|;

opendir(D, "../icons/flags");
my @entries = grep /\.png/, readdir(D);
foreach my $e (@entries) {
  print F qq|<file alias="flags/$e">../../icons/flags/$e</file>\n|;
}
print F qq|</qresource></RCC>\n|;
close(F);

