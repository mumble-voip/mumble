#! /usr/bin/perl

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

