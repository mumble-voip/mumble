#! /usr/bin/perl -w

use warnings;
use strict;
use XML::Simple;
use Data::Dumper;
use Date::Parse;
use Text::Wrap qw(wrap fill);

print "Reading XML...\n";

my $ref = XMLin('-');

my %log;
my %rev;

$Text::Wrap::columns = 79;

print "Parsing log...\n";

foreach my $entry (@{$$ref{'logentry'}}) {
  my ($ss,$mm,$hh,$day,$month,$year,$zone) = strptime($$entry{'date'});
  my $date = sprintf("%04d-%02d-%02d", $year + 1900, $month + 1, $day);
  if (! $log{$date}) {
    $log{$date} = ();
    $rev{$date} = {};
  }
  push @{$log{$date}},$entry;
  $rev{$date}->{$$entry{'revision'}}=1;
#  print Dumper($entry);
#  print "$date\n";
}
#print Dumper(\%rev);

print "Creating CHANGES...\n";

open(C, ">CHANGES");

foreach my $date (reverse sort keys %log) {
   my @revs=sort {$a <=> $b} keys %{$rev{$date}};
   my $minrev = $revs[0];
   my $maxrev = $revs[$#revs-1];
   print C "$date Revision $minrev";
   if ($maxrev != $minrev) {
     print C "-$maxrev";
   }
   print C "\n";

   my $laststr = "";
   foreach my $entry (@{$log{$date}}) {
     my $str = $$entry{'msg'};
     next if ($str =~ /^HASH\(/);
     next if ($str =~ /^\*\*\* empty/);
     $str =~ s/\r//g;
     $str =~ s/\n+$//g;
#     $str =~ s/\n/\n\t  /g;
     next if ($str eq $laststr);
     $laststr = $str;

     my $html = $str;
     $html =~ s/\n/<br \/>/g;
     if ($$entry{'author'} ne "slicer") {
       $str .= " [".$$entry{'author'}."]";
     }
     print C wrap("\t* ","\t  ",$str);
     print C "\n";
   }
}

close(C);
