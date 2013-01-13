#! /usr/bin/perl

# This is a small example script of how to set up murmur to authenticate through
# phpBB3. To use it, you'll have to have started murmur with DBus, and use the
# same session for this script.
#

use warnings;
use strict;

# Replace these with whatever is correct for you
our $dbname="phpbb3";
our $dbuser="phpbb3";
our $dbpass="uhduqw1237a";
our $dbprefix="phpbb_";
our $dbhost="localhost";

# Assign user id as phpbb3 user_id plus this, to avoid clashing
# with local murmur users. If you're going to use ONLY external
# authentication, you can set this to 1, but there's no real point.
# Note that Mumble ignores values above 1000000000 when allocating
# player IDs on its own, so you probably want to leave this alone.
our $id_offset = 1000000000;

# Path to phpBB user avatars. If you want to disable avatar support, set
# this blank. This can be either a directory path or a full URL.

our $avatar_path = "http://xeno.stud.hive.no/phpBB3/download.php?avatar=";

#
# No user servicable parts below this point.
#

use DBI;
use Net::DBus;
use Data::Dumper;
use Net::DBus::Reactor;
use LWP::UserAgent;
use Carp;

our %texturecache;

our @dbhparams=("dbi:mysql:dbname=${dbname};host=${dbhost}", $dbuser, $dbpass);
our $agent=new LWP::UserAgent;
$agent->timeout(5);

our ($bus, $service);

our $r = Net::DBus::Reactor->main;
eval {
  $bus = Net::DBus->system();
  $service = $bus->get_service("net.sourceforge.mumble.murmur");
};

if (! $service) {
  eval {
    $bus = Net::DBus->session();
    $service = $bus->get_service("net.sourceforge.mumble.murmur");
  };
}

die "Murmur service not found" if (! $service);

my $dbh=DBI->connect_cached(@dbhparams);
if (! $dbh) {
  die $DBI::errstr;
}

our $object = $service->get_object("/1");

our $rservice = $bus->export_service("net.sourceforge.mumble.phpbb");
our $robject = Mumble::Auth->new($rservice);


package Mumble::Auth;

use Data::Dumper;
use Image::Magick;
use Digest::MD5 qw(md5);
use Net::DBus::Exporter qw(net.sourceforge.mumble.auther);
use base qw(Net::DBus::Object);
dbus_method("authenticate", ["string","string"], ["int32","string",["array","string"]]);
dbus_method("getUserName", ["int32"], ["string"]);
dbus_method("getUserId", ["string"], ["int32"]);
dbus_method("getUserTexture", ["int32"], [["array", "byte"]]);

sub new {
  my $class = shift;
  my $service = shift;
  my $self = $class->SUPER::new($service, "/authority");
  bless $self, $class;
  return $self;
}

sub hash {
  my $self = shift;
  my $pw = shift;
  my $hash = shift;
  my $itoa64 = './0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz';
  my @itoa64 = split(//,$itoa64);
  my $count_log2 = index($itoa64, substr($hash,3,1));
  my $count = 1 << $count_log2;
  my $salt = substr($hash, 4, 8);

  my $nhash = $salt;

  do {
    $nhash = md5($nhash . $pw);
  } while ($count--);

  my $output = substr($hash, 0, 12);

  my $i = 0;
  my @input = split(//,$nhash);
  while ($i < 16) {
    my $value;
    $value = ord($input[$i++]);
    $output .= $itoa64[$value & 0x3f];
    if ($i < 16) {
      $value |= ord($input[$i]) << 8;
    }
    $output .= $itoa64[($value >> 6) & 0x3f];
    last if ($i++ >= 16);

    if ($i < 16) {
      $value |= ord($input[$i]) << 16;
    }
    $output .= $itoa64[($value >> 12) & 0x3f];

    last if ($i++ >= 16);
    $output .= $itoa64[($value >> 18) & 0x3f];
  };

  return $output;
}

# Possible responses are:
# >0 ID of user
# 0 SuperUser
# -1 Wrong password
# -2 Unknown user -- fall back to builtin database

sub authenticate {
  my $self = shift;
  my $uname = shift;
  my $pw = shift;

  my $dbh=DBI->connect_cached(@dbhparams);
  if (! $dbh) {
    carp $DBI::errstr;
    return -2,'',undef;
  }
  $dbh->do("SET names utf8");
  my $sth=$dbh->prepare("SELECT user_id, user_password, user_type, username FROM ${dbprefix}users WHERE LOWER(username) = LOWER(?)");
  $sth->execute($uname);
  if ((my $r=$sth->fetchrow_hashref())) {
    if ($$r{'user_password'} ne $self->hash($pw,$$r{'user_password'})) {
      print "Wrong password for $uname\n";
      return -1,'',undef;
    }
    if (($$r{'user_type'} != 0) && ($$r{'user_type'} != 3)) {
      return -1,'',undef;
    }
    my $id = $$r{'user_id'} + $id_offset;
    my $name = $$r{'username'};
    $sth->finish();

    my @groups;
    $sth=$dbh->prepare("SELECT group_name FROM ${dbprefix}user_group LEFT JOIN ${dbprefix}groups USING (group_id) WHERE user_id = ?");
    $sth->execute($$r{'user_id'});
    while ((my $g=$sth->fetchrow_hashref())) {
      push @groups, lc $$g{'group_name'};
    }
    #my $response = $object->setTemporaryGroups(0, $id, \@groups);
    #Dumper($response);
    print "Authenticated $uname as ID $id with groups ".join(" ",@groups)."\n";
    return $id,$name,\@groups;
  } else {
    print "Unknown user $uname\n";
    return -2,'',undef;
  }
}

# Possible responses are:
# string Name of user
# empty  Unknown user
# undef  Fall back to builting database

sub getUserName {
  my $self = shift;
  my $id = shift;

  my $dbh=DBI->connect_cached(@dbhparams);
  if (! $dbh) {
    carp $DBI::errstr;
    return undef;
  }
  $dbh->do("SET names utf8");
  my $sth=$dbh->prepare("SELECT username FROM ${dbprefix}users WHERE user_id = ?");
  $sth->execute($id - $id_offset);
  if ((my $r=$sth->fetchrow_hashref())) {
    print "UID $id :: " .$$r{'username'}."\n";
    return $$r{'username'};
  }
  print "No match for id $id\n";
  return undef;
}

# Same response as authenticate

sub getUserId {
  my $self = shift;
  my $name = shift;

  my $dbh=DBI->connect_cached(@dbhparams);
  if (! $dbh) {
    carp $DBI::errstr;
    return -2;
  }
  $dbh->do("SET names utf8");
  my $sth=$dbh->prepare("SELECT user_id FROM ${dbprefix}users WHERE username = ?");
  $sth->execute($name);
  if ((my $r=$sth->fetchrow_hashref())) {
    return $$r{'user_id'} + $id_offset;
  }
  return -2;
}

# Grab a user texture.

sub getUserTexture {
  my $self = shift;
  my $uid = shift;

  my @a;

  my $dbh=DBI->connect_cached(@dbhparams);
  if (! $dbh) {
    carp $DBI::errstr;
    return undef;
  }
  $dbh->do("SET names utf8");
  my $sth=$dbh->prepare("SELECT user_avatar, user_avatar_type FROM ${dbprefix}users WHERE user_id = ?");
  $sth->execute($uid - $id_offset);
  if ((my $r=$sth->fetchrow_hashref())) {
    my $file = $$r{'user_avatar'};
    my $type = $$r{'user_avatar_type'};
    if (($type != 1) && ($type != 2)) {
      print "Request for texture $uid :: not uploaded texture ($type)\n";
      return \@a;
    }
    if (exists $texturecache{$file}) {
      return $texturecache{$file};
    }

    my $url = (($type == 1) ? $avatar_path : '') . $file;
    my $response = $agent->get($url);
    if (! $response->is_success) {
      print "Request for texture $uid :: Fetch $url failed: ". $response->status_line . "\n";
    } else {
      my $image = new Image::Magick();
      my $r = $image->BlobToImage($response->content);
      if ($r) {
        print "Request for texture $uid :: Image $url load failed: $r\n";
      } else {
        $image->Extent(x => 0, y => 0, width => 600, height => 60);
        my $out=$image->ImageToBlob(magick => 'rgba', depth => 8);
        if (length($out) != (600*60*4)) {
          print "Request for texture $uid :: Failed resize\n";
        } else {
          @a = unpack("C*", $out);
          for(my $i=0;$i<600*60;$i++) {
            my $red=$a[$i*4];
            my $blue=$a[$i*4+2];
            $a[$i*4]=$blue;
            $a[$i*4+2]=$red;
          }
          print "Request for texture $uid :: $url :: Success\n";
        }
      }
    }
    $texturecache{$file} = \@a;
    return $texturecache{$file};
  }
  return undef;
}


package main;

my $response = $object->setAuthenticator("/authority", 0);

print "Entering main DBus loop...\n";
$r->run();
