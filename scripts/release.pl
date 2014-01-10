#! /usr/bin/perl -w

use strict;
use warnings;
use Carp;
use Switch;
use Archive::Tar;
use Archive::Tar::Constant;
use Archive::Zip qw( :ERROR_CODES :CONSTANTS );
use Compress::Zlib;
use POSIX;
use File::Copy;
use File::stat;

sub adddir($$) {
  my ($dir, $ref) = @_;
  $dir =~ s/^\.\///;
  my $base = $dir;
  $base =~ s/^.+\/([^\/]+)$/$1/;
  if (-f "$dir/$base.pro") {
    push @{$ref}, "$dir/$base.pro";
  }
  opendir(DIR, $dir);
  foreach my $e (grep { ! /^\./ } readdir(DIR)) {
    if (-d "$dir/$e") {
      adddir("$dir/$e", $ref);
    }
  }
}

my %files;
my $ver;
my %filevars = ( 'sources' => 1, 'objective_sources' => 1, 'headers' => 1, 'rc_file' => 1, 'dist' => 1, 'forms' => 1, 'resources' => 1, 'precompiled_header' => 1, 'translations' => 1);

system("rm mumble-*");
chdir("scripts");
system("bash mkini.sh");
chdir("..");

my @pro = ("main.pro");
#, "src/mumble.pri");
#adddir(".", \@pro);

my @resources;

while (my $pro = shift @pro) {
  open(F, $pro) or croak "Failed to open $pro";
  print "Processing $pro\n";
  $files{$pro}=1;
  my $basedir=$pro;
  $basedir =~ s/[^\/]+\Z//g;
  my @vpath = ($basedir);

  my $file = join("", <F>);
  $file =~ s/\\\n/\n/g;
  print $file;

  foreach my $line (split(/\n/, $file)) {
    $_ = $line;
    chomp();
    if (/^include\((.+)\)/) {
      my $f = $basedir . $1;
      while ($f =~ /\.\./) {
        $f =~ s/(\/|\A)[^\/]+\/\.\.\//$1/g;
      }
      push @pro, $f;
    } elsif (/^\s*(\w+)\s*?[\+\-\*]{0,1}=\s*(.+)$/) {
      my ($var,$value)=(lc $1,$2);
      switch ($var) {
        case "version" {
          if ($value !~ /\$\$/) {
              croak "Versions don't match: $ver vs $value" if (defined($ver) && ($ver ne $value));
              $ver=$value;
          }
        }
        case "vpath" {
          my $vdir = $basedir;
          if ($value eq '../$$SOURCEDIR/libcelt') {
            $vdir =~ s/-build/-src/;
            push @vpath, $vdir.'libcelt/';
          } elsif ($value eq '../$$SOURCEDIR/lib') { # sbcelt lib
            $vdir =~ s/-lib-build/-src/;
            push @vpath, $vdir.'lib/';
          } elsif ($value eq '../$$SOURCEDIR/helper') { # sbcelt helper
            $vdir =~ s/-helper-build/-src/;
            push @vpath, $vdir.'helper/';
          } elsif ($vdir eq '3rdparty/fx11-build/' and $value eq '../$$SOURCEDIR') { # fx11
            $vdir =~ s/-build/-src/;
            push @vpath, $vdir;
          } else {
            push @vpath,map { "$basedir$_/"} map { s/\$\$PWD/./; $_;} split(/\s/, $value);
          }
        }
        case "subdirs" {
          push @pro,map { my ($b,$p) = ($_,$_); $p =~ s/^.+\///g; "$basedir$b/$p.pro" } split(/\s/, $value);
        }
        case %filevars {
          foreach my $f (split(/\s+/,$value)) {
              next if ($f =~ /^Murmur\.(h|cpp)$/);
              next if ($f =~ /^Mumble\.pb\.(h|cc)$/);
              my $ok = 0;
              foreach my $d (@vpath) {
                if (-f "$d$f") {
                  $f = $d.$f;
                  $ok = 1;
                  last;
                }
              }
              if (! $ok) {
                croak "Failed to find $f in ".join(" ",@vpath);
              } else {
                while ($f =~ /\.\./) {
                  $f =~ s/(\/|\A)[^\/]+\/\.\.\//$1/g;
                }
                $files{$f}=1;
                if ($var eq "resources") {
                  push @resources,$f;
                }
              }
          }
        }
      }
    }
  }
  close(F);
}

foreach my $resfile (@resources) {
  open(F, $resfile);
  my $basedir=$resfile;
  $basedir =~ s/[^\/]+\Z//g;
  while(<F>) {
    chomp();
    if (/\>(.+)<\/file\>/) {
      my $f = $basedir.$1;
      next if $f =~ /\.qm$/;
      while ($f =~ /\.\./) {
                  $f =~ s/(\/|\A)[^\/]+\/\.\.\//$1/g;
      }
      
      $files{$f}=1;
    }
  }
  close(F);
}

my @fulldirs = ('speex','speex/include/speex','speex/libspeex','man');
foreach my $cver ('0.7.0', '0.11.0') {
  push @fulldirs, "celt-$cver-src";
  push @fulldirs, "celt-$cver-src/libcelt";
}
push @fulldirs, "sbcelt-src";
push @fulldirs, "sbcelt-src/helper";
push @fulldirs, "sbcelt-src/lib";
push @fulldirs, "opus-src";
push @fulldirs, "opus-src/celt";
push @fulldirs, "opus-src/celt/arm";
push @fulldirs, "opus-src/celt/x86";
push @fulldirs, "opus-src/silk";
push @fulldirs, "opus-src/silk/arm";
push @fulldirs, "opus-src/silk/float";
push @fulldirs, "opus-src/silk/fixed";
push @fulldirs, "opus-src/src";
push @fulldirs, "opus-src/include";

foreach my $dir (@fulldirs) {
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

if ($#ARGV < 0) {
  open(F, "git describe origin/master|");
  while (<F>) {
    chomp();
    s/^(.+)-([0-9]+)-(g.+)$/$1|$2|$3/;
    s/-/~/;
    s/\|/-/g;
    $ver = $_;
  }
  close(F);
  print "REVISION $ver\n";
} elsif ($#ARGV == 0) {
  $ver = $ARGV[0];
}

my $tar = new Archive::Tar();
my $zip = new Archive::Zip();
my $blob;
my $dir="mumble-$ver/";

my $zipdir = $zip->addDirectory($dir);

foreach my $file ('LICENSE', sort keys %files) {
  print "Adding $file\n";
  open(F, $file) or croak "Missing $file";
  sysread(F, $blob, stat($file)->size);

  if ($file eq "src/Version.h") {
    $blob =~ s/(\#ifndef MUMBLE_VERSION)/$1\n\#define MUMBLE_VERSION $ver\n\#endif\n$1/;
  }

  if (-l $file) {
    $tar->add_data($dir . $file, "", { linkname => readlink($file), type => Archive::Tar::Constant::SYMLINK });
  } else {
    $tar->add_data($dir . $file, $blob);
  }
  my $zipmember=$zip->addString($blob, $dir . $file);
  $zipmember->desiredCompressionMethod( COMPRESSION_DEFLATED );
  $zipmember->desiredCompressionLevel( 9 );
  close(F);
}

my $gz=gzopen("mumble-${ver}.tar.gz", "w");
$gz->gzwrite($tar->write());
$gz->gzclose();
$zip->writeToFileNamed("mumble-${ver}.zip");

copy("mumble-${ver}.tar.gz", "../deb-mumble/tarballs/mumble_${ver}.orig.tar.gz");
system("/usr/bin/gpg", "--armor", "--default-key", "DEBA6F3E", "--sign", "--detach-sign", "--output", "mumble-${ver}.tar.gz.sig", "mumble-${ver}.tar.gz");
system("/usr/bin/scp", "-4", "mumble-${ver}.tar.gz", "mumble-${ver}.tar.gz.sig", "slicer\@mumble.hive.no:/var/www/snapshot/");
