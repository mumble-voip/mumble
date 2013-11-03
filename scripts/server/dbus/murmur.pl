#! /usr/bin/perl
#
# For now, this is a CGI using Perl.
#

use warnings;
use strict;

## User configurable settings:

# What's the name of this server?
our $servername = "Mumble & Murmur Test Server";

# Who should outgoing authentication emails be from?
our $emailfrom = "";

# And what server should be used?
our $emailserver = "localhost";

# Which server to add to? Unless you have multiple virtual servers,
# this is always 1
our $serverid = 1;

## End of user configurable data
##
## Really. You shouldn't touch anything below this point.

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

use CGI;
use CGI::Carp 'fatalsToBrowser';
use CGI::Session;
use Net::SMTP;
use Net::DNS;
use Net::DBus;
use Image::Magick;
use Compress::Zlib;
use Config::Simple;

sub randomCode($) {
  my ($length) = @_;
  my $ret;
  my $chars="0123456789abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

  for(my $i=0;$i<$length;$i++) {
    $ret .= substr($chars, rand(int(length($chars))), 1);
  }
  return $ret;
}

my $showit = 1;

CGI::Session->find( sub { } );

my $q = new CGI();
my $s = new CGI::Session();

$s->expire('+1d');

print $s->header();
print $q->start_html(-title=>"Registration");

my $bus;
my $service;

# First try the system bus
eval {
  $bus=Net::DBus->system();
  $service = $bus->get_service("net.sourceforge.mumble.murmur");

  my $cfg = new Config::Simple(filename => '/etc/mumble-server.ini', syntax => 'simple');
  $servername = $cfg->param("registerName") || $servername;
  $emailfrom = $cfg->param("emailfrom") || $emailfrom;
};

# If that failed, the session bus
if (! $service) {
  eval {
    $bus = Net::DBus->session();
    $service = $bus->get_service("net.sourceforge.mumble.murmur");
  }
}

die "Murmur service not found" if (! $service);

if (! defined($emailfrom) || ($emailfrom eq "")) {
  croak(qq{Missing configuration.
  Please edit either /etc/mumble-server.ini for systemwide installations,
  or murmur.pl for a personal one.
  });
}


# Fetch handle to remote object
my $object = $service->get_object("/$serverid");
my $res;

my $auth = $q->param('auth');
my $name = $q->param('name');
my $pw = $q->param('pw');
my $email = $q->param('email');
my $image = $q->upload('image');

if (defined($s->param('auth')) && ($auth eq $s->param('auth'))) {
  $res = $object->getRegisteredPlayers($s->param('name'));
  if ($#{$res} == 0) {
    my $aref = $$res[0];
    if ($email ne $$aref[2]) {
      $$aref[3] = $s->param('pw');
      $object->updateRegistration($aref);
      print "<h1>Updated password</h1><p>Your password has been reset.</p>";
      $showit = 0;
    } else {
      print "<h1>Apologies</h1><p>Someone has already registered that name in the meantime.</p>";
    }
  } else {
    $res = $object->registerPlayer($s->param('name'));
    if (($res != 0) && ($res != "0")) {
      my @array = ($res, $s->param('name'), $s->param('email'), $s->param('pw'));
      $object->updateRegistration(\@array);
      print "<h1>Succeeded</h1><p>Thank you for registering.</p>";
      $showit = 0;
    } else {
      print "<h1>Failed</h1><p>Username rejected by server.</p>";
    }
  }
  $s->clear();
} elsif (defined($name) && defined($pw) && defined($image)) {
   my $id = $object->getPlayerIds( [ $name ] );
   $res = $object->verifyPassword($$id[0], $pw);
   if (! $res) {
     print "<h1>Tsk tsk</h1><p>Now, that's not a valid user and password, is it?</p>";
   } else {
     my $blob;
     sysread($image,$blob,1000000);
     my $image=Image::Magick->new();
     my $r=$image->BlobToImage($blob);
     if (! $r) {
       $image->Extent(x => 0, y => 0, width => 600, height => 60, background => "transparent");
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
         @a=unpack("C*", pack("N", $#a + 1) . compress(pack("C*",@a)));
         $res = $object->setTexture($$id[0], \@a);
       } else {
         $r=1;
       }
     }
     if ($r) {
        print "<h1>Image failure</h1><p>Failed to convert that to a proper image.</p>";
     } else {
        print "<h1>Succeeded</h1><p>Reconnect to use the new image.</p>";
        $showit = 0;
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

  $res=$object->getRegisteredPlayers($name);
  if ( $#{$res} == 0 ) {
    my $aref = $$res[0];
    if ($email ne $$aref[2]) {
      push @errors, "Name is already taken";
    }
  }

  if ($#errors == -1) {
    my $code = randomCode(10);

    $s->param('name', $name);
    $s->param('pw', $pw);
    $s->param('email', $email);
    $s->param('auth', $code);

    my $smtp = new Net::SMTP($emailserver);
    if (! $smtp) {
      croak(qq{Failed to connect to SMTP server "$emailserver". This is most likely a configuration problem.\n});
    }
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
    $showit = 0;
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
  print '<p>Fill out your desired username, password and your current email address. A mail will be sent to you ';
  print 'with an authentication code.</p>';
  print '<p>If you\'ve forgotten your password, just reregister with the same name and email address. A new ';
  print 'confirmation email will be sent.</p>';
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
