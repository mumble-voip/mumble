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
use DBI qw(:sql_types);
use Image::Magick;

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
my $forgot = $q->param('forgot');
my $image = $q->upload('image');

if ($forgot) {
  print "<h1>Resent<</h1><p>Any usernames/passwords associated with that email have been resent.</p>";
  my $sth = $dbh->prepare("SELECT COUNT(*) AS num FROM players WHERE email = ?");
  $sth->execute($forgot);
  my $r=$sth->fetchrow_hashref();
  $sth->finish();
  if ($$r{'num'} > 0) {
    my $smtp = new Net::SMTP($emailserver);
    $smtp->mail($emailfrom);
    $smtp->to($forgot);
    $smtp->data();
    $smtp->datasend("From: $emailfrom\n");
    $smtp->datasend("To: $forgot\n");
    $smtp->datasend("Subject: Murmur password reminder\n");
    $smtp->datasend("\n");
    $smtp->datasend("A user from $ENV{'REMOTE_ADDR'} requested murmur usernames\n");
    $smtp->datasend("on \"${servername}\" be resent.\n\n");
    $smtp->datasend(sprintf("%20s %s\n","Username","Password"));
    $smtp->datasend(sprintf("%20s %s\n","--------","--------"));

    $sth = $dbh->prepare("SELECT * FROM players WHERE email = ?");
    $sth->execute($forgot);
    while(($r=$sth->fetchrow_hashref())) {
      $smtp->datasend(sprintf("%20s %s\n",$$r{'name'},$$r{'pw'}));
    }
    $sth->finish();

    $smtp->datasend("\nNow try to remember them, please ;)\n");
    $smtp->dataend();
  }
  $showit = 0;
} elsif ($auth) {
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
} elsif (defined($name) && defined($pw) && defined($image)) {
   my $sth = $dbh->prepare("SELECT * FROM players WHERE name = ? AND pw = ?");
   $sth->execute($name,$pw);
   my $r = $sth->fetchrow_hashref();
   $sth->finish();
   if (! $r) {
     print "<h1>Tsk tsk</h1><p>Now, that's not a valid user and password, is it?</p>";
   } else {
     my $blob;
     sysread($image,$blob,1000000);
     my $image=Image::Magick->new();
     $r=$image->BlobToImage($blob);
     if (! $r) {
#       $image->Crop(x => 0, y => 0, width => 600, height => 60);
       $image->Extent(x => 0, y => 0, width => 600, height => 60);
       my $out=$image->ImageToBlob(magick => 'rgba', depth => 8);
       if (length($out) == (600*60*4)) {
         # We need BGRA, AKA ARGB inverse
         my @a=unpack("C*", $out);
         for(my $i=0;$i<600*60;$i++) {
           my $red=$a[$i*4];
           my $blue=$a[$i*4+2];
           $a[$i*4]=$blue;
           $a[$i*4+2]=$red;
         }
         $out=pack("C*",@a);   
         $sth=$dbh->prepare("UPDATE players SET texture=? WHERE name=?");
         $sth->bind_param(1, $out, SQL_BLOB);
         $sth->bind_param(2, $name);
         $sth->execute();
         $sth->finish();
       } else {
         $r=1;
       }
     }
     if ($r) {
        print "<h1>Image failure</h1><p>Failed to convert that to a proper image.</p>";
     } else {
        print "<h1>Succeeded</h1><p>Reconnect to use the new image.</p>";
     }
   }
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
  
  if ($email !~ /^[0-9a-zA-Z\.\-\_]+\@(.+)$/) {
    push @errors, "That doesn't even look like an email adddress.";
  } else {
    my @mx = mx($1); 
    if ($#mx == -1) {
      push @errors, "And how am I supposed to send email there?";
    }
  }
  
  my $sth=$dbh->prepare("SELECT name FROM players WHERE name like ?");
  $sth->execute($name);
  if (my $r=$sth->fetchrow_hashref()) {
    push @errors, "Name is already taken";
  }
  
  if ($#errors == -1) {
    my $code = randomCode(10);
    
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

  print '<h1>Forgot your pw?</h1>';
  print '<p>';
  print $q->start_form(-method=>'POST');
  print "Email ";
  print $q->textfield(-name=>'forgot', -size=>'30');
  print "<br />\n";
  print $q->submit(-value=>'Resend passwords');
  print $q->end_form();
  print '</p>';

  print '<h1>Upload custom texture?</h1>';
  print '<p>';
  print 'Remember that the image must be 600 by 60 pixels, and must have an alpha channel.<br />';
  print "\n";
  print $q->start_form(-method=>'POST');
  print "Username ";
  print $q->textfield(-name=>'name', -size=>'10');
  print "<br />\n";
  print "Password ";
  print $q->password_field(-name=>'pw', -size=>'10');
  print "<br />\n";
  print "Image ";
  print $q->filefield(-name=>'image', -size=>'30');
  print "<br />\n";
  print $q->submit(-value=>'Upload Image');
  print $q->end_form();
  print '</p>';
}

print $q->end_html();
