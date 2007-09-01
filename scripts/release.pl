#! /usr/bin/perl -w

use strict;
use warnings;
use Carp;
use Switch;
use Archive::Tar;
use Archive::Zip qw( :ERROR_CODES :CONSTANTS );
use Compress::Bzip2;

my %files;
my $ver;
my %filevars = ( 'sources' => 1, 'headers' => 1, 'rc_file' => 1, 'dist' => 1, 'forms' => 1, 'resources' => 1, 'precompiled_header' => 1, 'translations' => 1);

system("rm mumble-*");

foreach my $pro ("main.pro", "overlay/overlay.pro", "overlay_gl/overlay_gl.pro", "speexbuild/speexbuild.pro", "src/mumble/mumble.pro", "src/murmur/murmur.pro", "src/mumble.pri") {
  open(F, $pro) or croak "Failed to open $pro";
  print "Processing $pro\n";
  $files{$pro}=1;
  my $basedir=$pro;
  $basedir =~ s/[^\/]+\Z//g;
  while(<F>) {
    chomp();
    if (/^\s*(\w+)\s*?[\+\-]{0,1}=\s*(.+)$/) {
      my ($var,$value)=(lc $1,$2);
      switch ($var) {
        case "version" {
          croak "Versions don't match" if (defined($ver) && ($ver ne $value));
          $ver=$value;
        }
        case %filevars {
          foreach my $f (split(/\s+/,$value)) {
            $f =~ s/^.+\///g;
            if (-f "${basedir}$f") {
              $files{$basedir.$f}=1;
            } else {
              my $ok = 0;
              foreach my $d ("", "speexbuild/", "speexbuild/speex/", "src/", "src/mumble/", "src/murmur/", "icons/", "scripts/", "plugins/", "overlay/", "overlay_gl/", "speex/libspeex/") {
                if (-f "$d$f") {
                  $files{$d.$f}=1;
                  $ok = 1;
                }
              }
              if (! $ok) {
                croak "Failed to find $f";
              }
            }
          }
        }
      }
    }
  }
  close(F);
}

open(F, "src/mumble/mumble.qrc");
while(<F>) {
  chomp();
  if (/\<file\>(.+)<\/file\>/) {
    $files{$1}=1;
  } elsif (/\<file alias=\"(.+)\"\>/) {
    $files{"icons/$1"}=1;
  }
}
close(F);

foreach my $dir ('speex','speex/include/speex','speex/libspeex') {
  opendir(D, $dir) or croak "Could not open $dir";
  foreach my $f (grep(! /^\./,readdir(D))) {
    next if ($f =~ /\~$/);
    my $ff=$dir . '/' . $f;
    if (-f $ff) {
      $files{$ff}=1;
    }
  }
  closedir(D);
}

delete($files{'LICENSE'});

my $tar = new Archive::Tar();
my $zip = new Archive::Zip();
my $blob;
my $dir="mumble-$ver/";

my $zipdir = $zip->addDirectory($dir);

foreach my $file ('LICENSE', sort keys %files) {
  if ($file =~ /\.qm$/) {
    print "Skipping $file\n";
    next;
  }
  print "Adding $file\n";
  open(F, $file) or croak "Missing $file";
  sysread(F, $blob, 1000000000);
  $tar->add_data($dir . $file, $blob);
  my $zipmember=$zip->addString($blob, $dir . $file);
  $zipmember->desiredCompressionMethod( COMPRESSION_DEFLATED );
  $zipmember->desiredCompressionLevel( 9 );
  close(F);
}

my $bz=bzopen("mumble-${ver}.tar.bz2", "w");
$bz->bzwrite($tar->write());
$bz->bzclose();

$zip->writeToFileNamed("mumble-${ver}.zip");
