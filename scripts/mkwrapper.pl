#! /usr/bin/perl

use warnings;
use strict;

my $class;
my %return;
open(MI, "MurmurIce.cpp");
my @mi = <MI>;
close(MI);

open(I, ">MurmurIceWrapper.cpp");
open(B, ">BasicImpl.cpp");
open(MH, "Murmur.h");

sub func($$\@\@\@) {
  my ($class, $func, $wrapargs, $callargs, $implargs) = @_;

    print I "void ::Murmur::${class}I::${func}_async(". join(", ", @{$wrapargs}).") {\n";
#    print I "\tqWarning(\"CALL ${func}\");\n";
    print I "\tIceEvent *ie = new IceEvent(boost::bind(&impl_${class}_$func, " . join(", ", @${callargs})."));\n";
    print I "\tQCoreApplication::instance()->postEvent(mi, ie);\n";
    print I "};\n";

    if( ! grep(/impl_${class}_$func/,@mi)) {
      print B "static void impl_${class}_$func(".join(", ", @${implargs}). ") {}\n";
    }
}

while(<MH>) {
  chomp();
  if (/^class AMD_(.+) : virtual public ::IceUtil::Shared/) {
    $class = "AMD_".$1;
  }
  if (/virtual void ice_response\((.*)\) = 0;/) {
    $return{$class}=$1;
  }
  if (/virtual void (.+)_async\(const (.+?)&,(.*) const ::Ice::Current&/) {
    my $func=$1;
    my $obj=$2;
    my $args=$3;

    my $class="Meta";
    $class = "Server" if ($obj =~ /AMD_Server/);

    my @wrapargs;
    my @callargs;
    my @implargs;
    my $pc=0;
    push @wrapargs, "const $obj &cb";
    push @callargs, "cb";
    push @implargs, "const $obj cb";
    if ($class eq "Server") {
      push @callargs, "QString::fromStdString(current.id.name).toInt()";
      push @implargs, "int server_id";
    } else {
      push @callargs, "current.adapter";
      push @implargs, "const Ice::ObjectAdapterPtr adapter";
    }
    foreach my $p (split(/,/,$args)) {
      $pc++;
      push @wrapargs, "$p p$pc";
      push @callargs, "p$pc";
      push @implargs, "$p p$pc";
    }
#    if ($class eq "Server") {
      push @wrapargs, "const ::Ice::Current &current";
#    } else {
#      push @wrapargs, "const ::Ice::Current &";
#    }

    func($class,$func,@wrapargs,@callargs,@implargs);
  }
}

close(MH);
close(I);
close(B);

