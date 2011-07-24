#! /usr/bin/perl -w

use strict;
use warnings;
use Carp;
use Switch;
use Archive::Tar;
use LWP::UserAgent;

my %files;
my $ver;

sub schroot($) {
  my ($cmd) = @_;
  system("schroot --chroot natty_i386 --preserve-environment -- $cmd");
}

system("rm murmur-*");
unlink("murmur.x86");

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

if ($#ARGV < 0) {
  open(F, "git describe origin/master|");
  $ver = "";
  while (<F>) {
    chomp();
    $ver .= $_;
  }
  close(F);
  print "REVISION $ver\n";
} else {
  $ver=$ARGV[0];
}

schroot("qmake CONFIG+=release CONFIG+=no-client CONFIG+=quiet-build-log -recursive");
schroot("make -j distclean");
unlink("src/murmur/Murmur.h");
unlink("src/murmur/Murmur.cpp");
unlink("src/murmur/Mumble.pb.h");
unlink("src/murmur/Mumble.pb.cc");
unlink("murmur.x86");
schroot("qmake CONFIG+=release CONFIG+=no-client CONFIG+=quiet-build-log DEFINES+=MUMBLE_VERSION=\"$ver-ermine\" -recursive");
schroot("make -j");
system("strip release/murmurd");
schroot("\$HOME/.ermine/ErminePro.i386 --ld_assume_kernel=2.6.15 --output=murmur.x86 --with-gconv=noentry --with-locale=noentry --with-xlocale=noentry --with-nss=internal --config=scripts/ermine.conf release/murmurd");

$files{"murmur.x86"}="murmur.x86";
$files{"LICENSE"}="installer/gpl.txt";
$files{"README"}="README";
$files{"CHANGES"}="CHANGES";
$files{"murmur.pl"}="scripts/murmur.pl";
$files{"weblist.pl"}="scripts/weblist.pl";
$files{"icedemo.php"}="scripts/icedemo.php";
$files{"weblist.php"}="scripts/weblist.php";
$files{"murmur.ini"}="scripts/murmur.ini";
$files{"Murmur.ice"}="src/murmur/Murmur.ice";

my $tar = new Archive::Tar();
my $blob;
my $dir="murmur-static_x86-$ver/";

foreach my $file (sort keys %files) {
  print "Adding $file\n";
  open(F, $files{$file}) or croak "Missing $file";
  sysread(F, $blob, 1000000000);
  my %opts;
  $opts{'uid'}=0;
  $opts{'gid'}=0;
  $opts{'uname'}='root';
  $opts{'gname'}='root';
  if (($file =~ /\.x86$/) || ($file =~ /\.pl$/)) {
    $opts{'mode'}=0555;
  } elsif (($file =~ /\.ini$/)) {
    $opts{'mode'}=0644;
  } else {
    $opts{'mode'}=0444;
  }
  $tar->add_data($dir . $file, $blob, \%opts);
  close(F);
}

$tar->write("murmur-static_x86-${ver}.tar");
system("bzip2 -9 murmur-static_x86-${ver}.tar");
system("/usr/bin/scp","-4","murmur-static_x86-${ver}.tar.bz2", "slicer\@mumble.hive.no:/var/www/snapshot/");
system("/usr/bin/ssh","-4","slicer\@mumble.hive.no","/mumble/snapshot.pl");
