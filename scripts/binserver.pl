#! /usr/bin/perl -w

use strict;
use warnings;
use Carp;
use Switch;
use Archive::Tar;
use Compress::Bzip2;

my %files;
my $ver;

system("rm murmur-*");

foreach my $pro ("main.pro", "speexbuild/speexbuild.pro", "src/mumble/mumble.pro", "src/murmur/murmur.pro", "src/mumble.pri") {
  open(F, $pro) or croak "Failed to open $pro";
  print "Processing $pro\n";
  while(<F>) {
    chomp();
    if (/^\s*(\w+)\s*?[\+\-\s]=\s*(.+)$/) {
      my ($var,$value)=(lc $1,$2);
      switch ($var) {
        case "version" {
          croak "Versions don't match" if (defined($ver) && ($ver ne $value));
          $ver=$value;
        }
      }
    }
  }
  close(F);
}

chdir("src/murmur");
system("/usr/local/Trolltech/Qt-4.3.0/bin/qmake CONFIG+=static");
system("make distclean");
system("/usr/local/Trolltech/Qt-4.3.0/bin/qmake CONFIG+=static");
system("make");
chdir("../..");

$files{"murmur.x86"}="release/murmur";
$files{"LICENSE"}="installer/gpl.txt";
$files{"README"}="README";
$files{"CHANGES"}="CHANGES";
$files{"INSTALL"}="INSTALL";
$files{"murmur.pl"}="scripts/murmur.pl";
$files{"murmur.ini"}="scripts/murmur.ini";

my $tar = new Archive::Tar();
my $blob;
my $dir="murmur-static_x86-$ver/";

foreach my $file (sort keys %files) {
  print "Adding $file\n";
  open(F, $files{$file}) or croak "Missing $file";
  sysread(F, $blob, 1000000000);
  $tar->add_data($dir . $file, $blob);
  close(F);
}

my $bz=bzopen("murmur-static_x86-${ver}.tar.bz2", "w");
$bz->bzwrite($tar->write());
$bz->bzclose();
