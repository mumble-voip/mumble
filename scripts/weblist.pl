#! /usr/bin/perl

use warnings;
use strict;

# If we're being run as a CGI in suexec, $HOME doesn't exist. Fake it.
my $home = (getpwuid($<))[7];

# This needs to be done before "use Net::DBus"
if (open(F, "$home/murmur/.dbus.sh")) {
  while(<F>) {
    chomp();
    if ($_ =~ /^(.+?)\='(.+)';$/) {
      $ENV{$1}=$2;
    }
  }
  close(F);
}

use Net::DBus;
use CGI::Carp qw(fatalsToBrowser);
use HTML::Template;

my $tmpl_fname = $1 if ($0 =~ /(.+)\.([a-z]+)$/);
my $tmpl;
if (-f "$tmpl_fname.tmpl") {
	$tmpl = HTML::Template->new(filename => "$tmpl_fname.tmpl");
} else {
	$tmpl = HTML::Template->new(filehandle => *DATA);
}
my $bus;
my $service;

# First try the system bus
eval {
  $bus=Net::DBus->system();
  $service = $bus->get_service("net.sourceforge.mumble.murmur");
};

# If that failed, the session bus
if (! $service) {
  eval {
    $bus = Net::DBus->session();
    $service = $bus->get_service("net.sourceforge.mumble.murmur");
  }
}

die "Murmur service not found" if (! $service);

# Fetch handle to remote object
my $object = $service->get_object("/");
# Call a function on the murmur object
my $servers = $object->getBootedServers();

my $params = [];
foreach my $server (@{$servers}) {
  my $name = $object->getConf($server, "registername");
  my $servobj = $service->get_object("/$server");

  my %users;
  # First, get channel names
  my $channels = $servobj->getChannels();
  my %channels;
  foreach my $c (@{$channels}) {
    my @c = @{$c};
    my $id = $c[0];
    my $name = $c[1];
    $channels{$id}=$name;
  }
  # Then, get and print the players
  my $players = $servobj->getPlayers();
  my $_total = 0;
  foreach my $p (@{$players}) {
    my @p = @{$p};
    my $chanid = $p[6];
    my $name = $p[8];
    my $chan = $channels{$chanid};
	$users{$chan} = [] unless $users{$chan};
	push @{$users{$chan}}, $name;
	$_total++;
  }
  my $_channels = [];
  for my $c (sort keys %users) {
	my $_users = [];
		for my $u (sort @{$users{$c}}) {
			push @{$_users}, {'user' => $u};
		}
	push @{$_channels}, {
		'channel' => $c,
		'users' => $_users
	};
  }
  push @{$params}, {'server' => $server, 'name' => $name, 'total' => $_total, 'channels' => $_channels};
}

$tmpl->param("servers", $params);
print "Content-type: text/html;charset=utf8\r\n\r\n",
		$tmpl->output;

__END__
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<HTML lang="ja">
	<HEAD>
		<Title>mumble server user list</title>
		<STYLE TYPE="text/css">
		<!--
			body {
				background-color: #ffffff;
				color: #4b4b4b;
				font-family: verdana, arial, sans-serif;
				font-size: 13px;
				margin: 10px 20px;
			}
			dl {
				margin-left: 5px;
			}
			dt {
				clear: both;
				font-weight: bold;
				padding-left: 5px;
				border-left: #4b4b4b 5px solid
			}
			dd {
				float: left;
				margin-left: 5px;
				margin-bottom: 5px;
				margin-top: 2px;
			}
		-->
		</STYLE>
	</HEAD>
	<BODY bgcolor="#ffffff" text="#4b4b4b" link="#3399ff" alink="#0099cc" vlink="#006666">
		<TMPL_LOOP NAME="servers">
			<H1>Server #<TMPL_VAR NAME="server"> <TMPL_VAR NAME="name"></H1>
			total: <TMPL_VAR NAME="total">
			<dl>
				<TMPL_LOOP NAME="channels">
				<dt><TMPL_VAR NAME="channel">
				<dd>
					<TMPL_LOOP NAME="users">
						<TMPL_VAR NAME="user">,
					</TMPL_LOOP>
				</TMPL_LOOP>
			</dl>
		</TMPL_LOOP>
	</BODY>
</HTML>

