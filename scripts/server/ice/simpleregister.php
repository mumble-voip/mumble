<?php
// Really simple script to add users to your mumble server.
// Make sure to select the server this script should be used for first:
$serverId = 1;

// Requires a correctly set up PHP-ICE Module and mumble server running with ICE.
//  For instructions see http://mumble.sourceforge.net/ICE

// Credits
//  This script was created by Kissaki

// This is as simple as it can get. You really SHOULD NOT make the URL to this script all-public as it may be abused/spammed.


if (!extension_loaded('ice')) {
	exit('Error: This example script needs the PHP extension for Ice to be loaded.');
}
if ((!function_exists('Ice_intVersion') || Ice_intVersion() < 30400) || !function_exists('Ice_loadProfile')) {
	exit('Error: This example script is compatible only for php-ice versions prior to 3.4.');
}

?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
 "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
  <meta http-equiv="Content-Type" content="text/html;charset=utf-8" />
</head>
<body>
<?php

Ice_loadProfile();

if( isset($_GET['uname']) && $_GET['password'] && $_GET['email'] && $_GET['uname']!='' && $_GET['password']!='' )
{

  try {
    $base = $ICE->stringToProxy("Meta:tcp -h 127.0.0.1 -p 6502");
    $meta = $base->ice_checkedCast("::Murmur::Meta");

    $server = $meta->getServer($serverId);

		if(empty($server)){
			echo 'Server could not be found.';
		}else{
			$registrationId = $server->registerPlayer($_GET['uname']);

	    $registration = $server->getRegistration(intval($registrationId));
	    $registration->pw = $_GET['password'];
	    $registration->email = $_GET['email'];
	    $server->updateregistration($registration);
		}

	}catch(InvalidPlayerException $ex){
		echo 'InvalidPlayerException';
	}catch(ServerBootedException $ex){
    echo 'Server is not running';
	}catch(Ice_UnknownLocalException $exc){
		echo 'Ice could probably not be found';
	}catch (Ice_Exception $ex){
    echo "<p>\n<pre>\n";
    print_r($ex);
    echo "</pre>\n</p>\n";
  }
}

?>
  <form action="" method="get">
    <table>
      <tr><td>Username</td><td><input name="uname" type="text" value=""/></td></tr>
      <tr><td>Password</td><td><input name="password" type="password" value=""/></td>
      <tr><td>eMail</td><td><input name="email" type="text" value=""/></td></tr>
    </table>
    <input type="submit"/>
  </form>
</body>
</html>
