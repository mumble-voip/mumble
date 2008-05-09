#! /usr/bin/perl

use warnings;
use strict;
use Carp;

sub spitout($$) {
  my ($var,$file)=@_;

  my $ret;


  open(IN, $file) or croak;
  my $l = join("", <IN>);
  $l =~ s/\r//g;
  $l =~ s/\f//g;
  $l =~ s/\"/\\\"/g;

  $l = join("\\n\"\n\"",split(/\n/, $l));

  return qq!static const char *${var} = \"! . $l . "\";\n\n\n";
}

open(F, "> ../src/mumble/licenses.h");

print F spitout("licenseMumble", "../installer/gpl.txt");

close(F);
