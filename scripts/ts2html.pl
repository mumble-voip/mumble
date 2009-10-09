#! /usr/bin/perl

use warnings;
use strict;
use XML::Simple;
use Data::Dumper;
use HTML::Entities;

my $ref = XMLin('-', ForceArray => 1);

#print Dumper($ref);

print qq|<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE html
  PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<title>English Mumble</title>
</head>
<body>
|;

foreach my $entry (@{$$ref{'context'}}) {
#  print Dumper($entry);
  
  print "<h1>".$entry->{'name'}[0]."</h1>\n";

  my $msg = $entry->{'message'};
  foreach my $m (@{$msg}) {
    my $text = $m->{'source'}[0];
    $text =~ s/\&(\S+)/$1/g;
    $text = encode_entities($text, '&%');
    $text =~ s/^<\-$/&lt;-/;
    if ($text =~ /\<p\>/) {
      print $text;
    }  else {
      print "<p>".$text."</p>";
    }
    print "\n";
  }
#  my $ctx = $$ref{'context'}->{$entry};
#  print "HAXX\n";
#  print Dumper($entry);
#  print Dumper($ctx);
#  my $r = $$ctx{'message'};
#  print Dumper($r);
  
#  foreach my $msg (keys %{$$ctx{'message'}}) {
#    print Dumper($msg);
#  }
#  last;
}

print "</body></html>\n";
