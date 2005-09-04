#! /usr/bin/perl -w

use strict;
use warnings;
use Carp;
use Switch;
use Archive::Tar;
use Archive::Zip;
use Compress::Bzip2;

my %files;
my $ver;
my %filevars = ( 'sources' => 1, 'headers' => 1, 'rc_file' => 1, 'dist' => 1, 'forms' => 1);

system("rm mumble-*");

foreach my $pro ("mumble.pro", "murmur.pro", "mumble.pri") {
  open(F, $pro) or croak;
  while(<F>) {
    chomp();
    if (/^(\w+)\s*=\s*(.+)$/) {
      my ($var,$value)=(lc $1,$2);
      switch ($var) {
        case "version" {
          croak "Versions don't match" if (defined($ver) && ($ver ne $value));
          $ver=$value;
        }
        case %filevars {
          foreach my $f (split(/\s+/,$value)) {
            $files{$f}=1;
          }
        }
      }
    }
  }
  close(F);
}

delete($files{'LICENSE'});

my $tar = new Archive::Tar();
my $zip = new Archive::Zip();
my $blob;
my $dir="mumble-$ver/";

my $zipdir = $zip->addDirectory($dir);

foreach my $file ('LICENSE', sort keys %files) {
  print "Adding $file\n";
  open(F, $file);
  sysread(F, $blob, 1000000000);
  $tar->add_data($dir . $file, $blob);
  $zip->addString($blob, $dir . $file);
  close(F);
}

my $bz=bzopen("mumble-${ver}.tar.bz2", "w");
$bz->bzwrite($tar->write());
$bz->bzclose();

$zip->writeToFileNamed("mumble-${ver}.zip");
