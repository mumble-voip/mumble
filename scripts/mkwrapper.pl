#! /usr/bin/perl

use warnings;
use strict;

my $class;
my %return;
open(MI, "MurmurIce.cpp");
my @mi = <MI>;
close(MI);

my $ice;
open(ICE, "Murmur.ice");
my @ice=<ICE>;
close(ICE);
$ice = join("", @ice);

$ice =~ s/\/\*.+?\*\///gms;
$ice =~ s/^\t+//gm;
$ice =~ s/\n+/\n/gs;
$ice =~ s/^\n+//s;

$ice =~ s/"/\\"/g;
$ice =~ s/\n/\\n/g;

open(I, ">MurmurIceWrapper.cpp");
open(B, ">BasicImpl.cpp");
open(MH, "Murmur.h");

sub func($$\@\@\@) {
  my ($class, $func, $wrapargs, $callargs, $implargs) = @_;
  

  print I qq'
void ::Murmur::${class}I::${func}_async('. join(", ", @{$wrapargs}).qq') {
	// qWarning() << "${func}" << meta->mp.qsIceSecretRead.isNull() << meta->mp.qsIceSecretRead.isEmpty();
#ifndef ACCESS_${class}_${func}_ALL
#ifdef ACCESS_${class}_${func}_READ
	if (! meta->mp.qsIceSecretRead.isNull()) {
		bool ok = ! meta->mp.qsIceSecretRead.isEmpty();
#else
	if (! meta->mp.qsIceSecretRead.isNull() || ! meta->mp.qsIceSecretWrite.isNull()) {
		bool ok = ! meta->mp.qsIceSecretWrite.isEmpty();
#endif
		::Ice::Context::const_iterator i = current.ctx.find("secret");
		ok = ok && (i != current.ctx.end());
		if (ok) {
			const QString &secret = u8((*i).second);
#ifdef ACCESS_${class}_${func}_READ
			ok = ((secret == meta->mp.qsIceSecretRead) || (secret == meta->mp.qsIceSecretWrite));
#else
			ok = (secret == meta->mp.qsIceSecretWrite);
#endif
		}
		if (! ok) {
			cb->ice_exception(InvalidSecretException());
			return;
		}
	}
#endif
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_${class}_$func, ' . join(", ", @${callargs}).qq'));
	QCoreApplication::instance()->postEvent(mi, ie);
}
';

  if( ! grep(/impl_${class}_$func/,@mi)) {
    print B "static void impl_${class}_$func(".join(", ", @${implargs}). ") {}\n";
  }
}


while(<MH>) {
  chomp();
  if (/^class AMD_(.+) : virtual public ::Ice(?:::AMDCallback|Util::Shared)/) {
    $class = "AMD_".$1;
  }
  if (/virtual void ice_response\((.*)\) = 0;/) {
    $return{$class}=$1;
  }
  if (/virtual void (.+)_async\(const (.+?)&,(.*) const ::Ice::Current&/) {
    my $func=$1;
    my $obj=$2;
    my $args=$3;
    
    next if ($func eq "getSlice");

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
                                                                             
print I qq'
void ::Murmur::MetaI::getSlice_async(const ::Murmur::AMD_Meta_getSlicePtr& cb, const Ice::Current&) {
	cb->ice_response(std::string("$ice"));
}
';

close(MH);
close(I);
close(B);

