#! /usr/bin/perl
# For now, this is a CGI using Perl, as PHP has no SQLite3 support.
# It's been promised in PHP 5.1, but it's not out of beta yet.
#
# CGIs actually have an added bonus; with suexec they will run
# as their own user, meaning the database doesn't have to be 
# world-writable.
#

use warnings;
use strict;
use CGI;
use CGI::Carp 'fatalsToBrowser';
use Net::SMTP;
use Net::DNS;
use DBI;

## User configurable settings:

# What's the name of this server?
our $servername = "Mumble & Murmur Test Server";

# Where is the murmur.sqlite database to be found?
our $dbpath = "murmur.sqlite";

# Who should outgoing authentication emails be from?
our $emailfrom = "";

# And what server should be used?
our $emailserver = "localhost";

## End of user configurable data

sub randomCode($) {
  my ($length) = @_;
  my $ret;
  my $chars="0123456789abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
 
  for(my $i=0;$i<$length;$i++) {
    $ret .= substr($chars, rand(int(length($chars))), 1);
  }
  return $ret;
}

if ($emailfrom eq "") {
  croak("Missing configuration");
}

my $dbh = DBI->connect("dbi:SQLite:dbname=${dbpath}", "", "") or croak $DBI::errstr;
my $showit = 1;

my $q = new CGI();
print $q->header();
print $q->start_html(-title=>"Registration");

my $auth = $q->param('auth');
my $name = $q->param('name');
my $pw = $q->param('pw');
my $email = $q->param('email');

if ($auth) {
   my $sth = $dbh->prepare("SELECT * FROM player_auth WHERE authcode = ?");
   $sth->execute($q->param('auth'));
   if (my $r = $sth->fetchrow_hashref()) {
     my $ins = $dbh->prepare("INSERT INTO players (name, email, pw) VALUES (?,?,?)");
     $ins->execute($$r{'name'}, $$r{'email'}, $$r{'pw'});
     $ins->finish();
     print "<h1>Succeeded</h1><p>Thank you for registering.</p>";
   } else {
     print "<h1>Tsk tsk</h1><p>Now, that's not a valid auth code, is it?</p>";
   }
   $sth->finish();
   $sth = $dbh->prepare("DELETE FROM player_auth WHERE authcode = ?");
   $sth->execute($q->param('auth'));
   $sth->finish();
   $showit = 0;
} elsif (defined($name) && defined($pw) && defined($email)) {
  my @errors;

  if (length($name) < 4) {
    push @errors, "Username is too short.";
  }
  if (length($pw) < 8) {
    push @errors, "Password is too short.";
  }
  if ($name !~ /^[0-9a-zA-Z\(\)\[\]\-\_]+$/) {
    push @errors, "Username contains illegal characters.";
  }
  
  if ($email !~ /^[0-9a-zA-Z\.\-]+\@(.+)$/) {
    push @errors, "That doesn't even look like an email adddress.";
  } else {
    my @mx = mx($1); 
    if ($#mx == -1) {
      push @errors, "And how am I supposed to send email there?";
    }
  }
  
  my $sth=$dbh->prepare("SELECT name FROM players WHERE name=?");
  $sth->execute($name);
  if (my $r= $sth->fetchrow_hashref()) {
    push @errors, "Name is already taken";
  }
  
  if ($#errors == -1) {
    my $code = randomCode(10);
    
    my $sth;
    $sth=$dbh->prepare("DELETE FROM player_auth WHERE name like ?");
    $sth->execute($name);
    $sth->finish();
    $sth=$dbh->prepare("INSERT INTO player_auth (name, pw, email, authcode) VALUES (?,?,?,?)");
    $sth->execute($name, $pw, $email, $code);
    $sth->finish();
    $showit = 0;
    
    my $smtp = new Net::SMTP($emailserver);
    $smtp->mail($emailfrom);
    $smtp->to($email);
    $smtp->data();
    $smtp->datasend("From: $emailfrom\n");
    $smtp->datasend("To: $email\n");
    $smtp->datasend("Subject: Murmur registration\n");
    $smtp->datasend("\n");
    $smtp->datasend("A user from $ENV{'REMOTE_ADDR'} registered the username $name\n");
    $smtp->datasend("on \"${servername}\".\n\n");
    $smtp->datasend("If this was you, please visit the following url to activate your account:\n");
    $q->delete_all();
    $q->param('auth', $code);
    $smtp->datasend($q->url(-query=>1));
    $smtp->datasend("\n\n");
    $smtp->datasend("If you have no idea what this is about, just disregard this message.");
    $smtp->dataend();
    
    print '<h1>Registration complete</h1><p>Thank you for registering. An email has been sent to you with ';
    print 'an activation code.</p>';
  } else {
    print '<ul>';
    foreach my $error (@errors) {
      print "<li>$error</li>";
    }
    print '</ul>';
  }
} 

if ($showit) {
  print '<h1>Registration form</h1>';
  print '<p>';
  print $q->start_form(-method=>'POST');
  print "Username ";
  print $q->textfield(-name=>'name', -size=>'10');
  print "<br />\n";
  print "Email ";
  print $q->textfield(-name=>'email', -size=>'30');
  print "<br />\n";
  print "Password ";
  print $q->password_field(-name=>'pw', -size=>'10');
  print "<br />\n";
  print $q->submit(-value=>'Register');
  print $q->end_form();
  print '</p>';
}

print $q->end_html();
  
