#! /usr/bin/perl -w

use strict;
use warnings;
use Carp;
use DBI;
use XML::LibXML;

# BF2 Link Script
#
# Links murmur to one or more BF2 servers, see the wiki for details.
#

our $dbpath = "murmur.sqlite";
our $basechannel = "Servers";

my %channame;
my %chanparent;
my %conid;
my %conplayerid;
my %conchannel;

sub findchannel($$) {
  my ($parent, $name) = @_;
  foreach my $id (keys %channame) {
    if (($chanparent{$id} == $parent) && ($channame{$id} eq $name)) {
      return $id;
    }
  }
  return -1;
}

my $parser = new XML::LibXML();
my $doc = $parser->parse_file($ARGV[0]) or croak "failed read XML";
$doc->indexElements();

my $dbh = DBI->connect("dbi:SQLite:dbname=${dbpath}", "", "") or croak $DBI::errstr;
$dbh->{AutoCommit} = 0;

my $sth;
$sth = $dbh->prepare("SELECT count(*) AS num FROM commands");
$sth->execute();
my $r = $sth->fetchrow_hashref();
my $inlist = $$r{'num'};
$sth->finish();
croak "Unfinished commands in queue" if ($inlist > 0);


$sth = $dbh->prepare("SELECT channel_id, parent_id, name FROM channels");
$sth->execute();
while((my $r=$sth->fetchrow_hashref())) {
  $channame{$$r{'channel_id'}}=$$r{'name'};
  $chanparent{$$r{'channel_id'}}=$$r{'parent_id'};
}
$sth->finish();


$sth = $dbh->prepare("SELECT con_id, player_id, channel_id, player_name FROM connections");
$sth->execute();
while((my $r=$sth->fetchrow_hashref())) {
  my $name = lc $$r{'player_name'};
  $conid{$name} = $$r{'con_id'};
  $conplayerid{$name} = $$r{'player_id'};
  $conchannel{$name} = $$r{'channel_id'};
}
$sth->finish();
$dbh->commit();

my $parent = findchannel(0, $basechannel);
croak "Missing basechannel" if ($parent == -1);

$sth=$dbh->prepare("INSERT INTO commands (command,arg1,arg2,arg3) VALUES (?,?,?,?)");

our %watched;
our %checked;

foreach my $context ($doc->findnodes('qstat/server')) {
  my $servname = $context->findvalue("name");

  next if (! $servname);

  my %rule;

  foreach my $rulectx ($context->findnodes('rules/rule')) {
    my @attr = $rulectx->attributes();
    my $name=$attr[0]->getValue();
    $rule{$name} = $rulectx->textContent();
  }

  if ($rule{'gamename'} ne "battlefield2") {
     print "$servname: Skipping unknown game type";
     next;
  }

  my $t1map = $rule{'team_t0'};
  my $t2map = $rule{'team_t1'};

  if (! $t1map || ! $t2map) {
     print "$servname: Failed to find team mapping";
     next;
  }

  my $chan = findchannel($parent, $servname);
  if ($chan == -1 ) {
    print "$servname: Missing server channel -- creating\n";
    $sth->execute("createchannel", $parent, $servname, undef);
    next;
  }

  my $team1 = findchannel($chan, "Team 1");
  my $team2 = findchannel($chan, "Team 2");

  if ($team1 == -1) {
    print "$servname: Missing Team 1 channel -- creating\n";
    $sth->execute("createchannel", $chan, "Team 1", undef);
    next;
  }

  if ($team2 == -1) {
    print "$servname: Missing Team 2 channel -- creating\n";
    $sth->execute("createchannel", $chan, "Team 2", undef);
    next;
  }

  my $missing = 0;
  my @subchans;

  foreach my $cname (1 .. 9, "Unassigned") {
    my $id;
    $id=findchannel($team1, $cname);
    push @subchans, $id;
    if ($id == -1) {
      $sth->execute("createchannel", $team1, $cname, undef);
      $missing = 1;
    }
    $id=findchannel($team2, $cname);
    push @subchans, $id;
    if ($id == -1) {
      $sth->execute("createchannel", $team2, $cname, undef);
      $missing = 1;
    }
  }

  if ($missing) {
    print "$servname: Created missing squad channels\n";
    next;
  }

  foreach my $w ($chan,$team1,$team2,@subchans) {
    $watched{$w} = 1;
  }

  my @t1list;
  my @t2list;

  foreach my $playerctx ($context->findnodes("players/player")) {
    my $name = lc $playerctx->findvalue('name');
    my $team = $playerctx->findvalue('team');
    next if (! $name || ! $team);
    next if (! defined($conid{$name}));
    my $conid = $conid{$name};
    my $chan = $conchannel{$name};
    my $pchan = $chanparent{$chan};
    my $id = $conplayerid{$name};

    $checked{$name} = 1;

    my $wchan;
    if ($team eq $t1map) {
      $wchan = $team1;
    } elsif ($team eq $t2map) {
      $wchan = $team2;
    }
    next if (! $wchan);

    if (($wchan != $chan) && ($wchan != $pchan)) {
      $sth->execute("moveplayer", $conid, findchannel($wchan, "Unassigned"), undef);
    }
    if ($id >= 0) {
      if ($wchan == $team1) {
         push @t1list, $id;
      } else {
         push @t2list, $id;
      }
    }
  }
  $sth->execute("setgroup", $team1, "players", join(",",@t1list));
  $sth->execute("setgroup", $team2, "players", join(",",@t2list));
}

foreach my $name (keys %conid) {
  next if $checked{$name};
  if (defined($watched{$conchannel{$name}})) {
    $sth->execute("moveplayer", $conid{$name}, 0, undef);
  }
}

$sth->finish();
$dbh->commit();
$dbh->disconnect();
