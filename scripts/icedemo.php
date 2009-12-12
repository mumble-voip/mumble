<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
 "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
<meta http-equiv="Content-Type" content="text/html;charset=utf-8" />
<title>Userlist</title>
</head>
<body>
<?php

Ice_loadProfile();

if ($argc > 0) {
  for($i=1;$i<$argc;$i++) {
    parse_str($argv[$i], $tmp);
    $_REQUEST=array_merge($_REQUEST, $tmp);
  }
}

function ucmp($a, $b) {
  if (($a->playerid == 0) || ($b->playerid == 0)) {
    return ($a->playerid - $b->playerid);
  }
  return strcasecmp($a->name, $b->name);
}

$confitems = array();
$confbig = array();
$confitems['host']="IP Address to bind to";
$confitems['port']="Port to use";
$confitems['password']="Password for unregistered users";
$confitems['timeout']="Timeout before kicking dead connections";
$confitems['bandwidth']="Maximum bandwidth in bytes/sec";
$confitems['users']="Maximum number of users on server";
$confitems['welcometext']="Welcome message";
$confbig['welcometext']=1;
$confitems['registername']="Server Name";
$confitems['registerpassword']="Password for Global Server List";
$confitems['registerhostname']="Hostname for Global Server List";
$confitems['registerurl']="HTTP URL for Global Server List";
$confitems['certificate']="PEM Encoded SSL Certificate";
$confbig['certificate']=1;
$confitems['key']="PEM Encoded SSL Key";
$confbig['key']=1;

try {
  $base = $ICE->stringToProxy("Meta:tcp -h 127.0.0.1 -p 6502");
  $meta = $base->ice_checkedCast("::Murmur::Meta");

  $default = $meta->getDefaultConf();

  if (! is_null($_REQUEST['newserver'])) {
    $meta->newServer();
  } else if (! is_null($_REQUEST['delserver'])) {
    $meta->getServer((int)$_REQUEST['delserver'])->delete();
  } else if (! is_null($_REQUEST['stop'])) {
    $meta->getServer((int) $_REQUEST['stop'])->stop();
  } else if (! is_null($_REQUEST['start'])) {
    $meta->getServer((int) $_REQUEST['start'])->start();
  } else if (! is_null($_REQUEST['action'])) {
    $server = $meta->getServer((int) $_REQUEST['action']);
    if (!is_null($_REQUEST['kick'])) {
      $server->kickPlayer((int) $_REQUEST['kick'], "Mushroom");
    }
  } else if (! is_null($_REQUEST['uedit'])) {
    $server = $meta->getServer((int) $_REQUEST['uedit']);
    if (isset($_REQUEST['newplayer'])) {
      $reg = array();
      $reg[Murmur_UserInfo::UserName] = $_REQUEST['newplayer'];
	  if ($_REQUEST['password']) $reg[Murmur_UserInfo::UserPassword] = $_REQUEST['password']; // This line is added for password at user creation
      $_REQUEST['uid'] = $server->registerUser($reg);
    }
    if (! is_null($_REQUEST['deleteplayer'])) {
      $server->unregisterUser((int) $_REQUEST['deleteplayer']);
    }
    if (! is_null($_REQUEST['uid'])) {
      $uid = (int) $_REQUEST['uid'];
      $user = $server->getRegistration($uid);
      if (! is_null($_REQUEST['set'])) {
        $user[Murmur_UserInfo::UserEmail] = $_REQUEST['email'];
        $user[Murmur_UserInfo::UserPassword] = $_REQUEST['pw'];
        $server->updateRegistration($uid,$user);
      } else {
        echo "<form method=\"post\" action=\"".$_SERVER['PHP_SELF']."\">\n";
        echo "<p>\n";
        echo "<b>Name:</b> ".$user[Murmur_UserInfo::UserName]."<br />\n";
        echo "<input type=\"hidden\" name=\"set\" value=\"1\" />\n";
        echo "<input type=\"hidden\" name=\"uedit\" value=\"".$server->id()."\" />\n";
        echo "<input type=\"hidden\" name=\"uid\" value=\"$uid\" />\n";
        echo "<b>Email:</b> <input type=\"text\" name=\"email\" size=\"30\" maxlength=\"128\" value=\"".htmlspecialchars($user[Murmur_UserInfo::UserEmail])."\" /><br />\n";
        echo "<b>New password:</b> <input type=\"password\" name=\"pw\" size=\"30\" maxlength=\"128\" /><br />\n";
        echo "<input type=\"submit\" />\n";
        echo "</p>\n";
        echo "</form>\n";
      }
    }
    echo "<h1>Registered User List</h1>\n";
    echo "<form method=\"post\" action=\"".$_SERVER['PHP_SELF']."\">\n";
    echo "<p>\n";
    echo "<input type=\"hidden\" name=\"uedit\" value=\"".$server->id()."\" />\n";
    echo "<b>New User:</b>";
    echo "<input type=\"text\" name=\"newplayer\" size=\"30\" maxlength=\"60\" />";
	echo "<b>User Password:</b>";
    echo "<input type=\"text\" name=\"password\" size=\"30\" maxlength=\"60\" />";
    echo "<input type=\"submit\" />\n";
    echo "</p>\n";
    echo "</form>\n";
    echo "<table>\n";
    echo "<tr><th>UserName</th><th>Email</th><th></th></tr>\n";
    $users = $server->getRegisteredUsers("");
    foreach($users as $id=>$name) {
      $u = $server->getRegistration($id);
      echo "<tr><td>$name $id</td><td>".$u[Murmur_UserInfo::UserEmail]."</td><td>";
      echo "<a href=\"?uedit=".$server->id()."&amp;uid=".$id."\">[Edit]</a> ";
      echo "<a href=\"?uedit=".$server->id()."&amp;deleteplayer=".$id."\">[Unregister]</a> ";
      echo "</td></tr>\n";
    }
    echo "</table>\n";
  } else if (! is_null($_REQUEST['server'])) {
    $server = $meta->getServer($_REQUEST['server'] + 0);
    if (! is_null($_REQUEST['set'])) {
      foreach($confitems as $key=>$desc) {
        $server->setConf($key, $_REQUEST[$key]);
      }
    } else {
      $conf = $server->getAllConf();
      $default['port'] += $server->id() - 1;
      echo "<h1>Server Configuration</h1>\n";
      echo "<form method=\"post\" action=\"".$_SERVER['PHP_SELF']."\">\n";
      echo "<div>\n";
      echo "<input type=\"hidden\" name=\"server\" value=\"".$server->id()."\" />\n";
      echo "<input type=\"hidden\" name=\"set\" value=\"1\" />\n";
      echo "</div>\n";
      echo "<table><tr><th>Description</th><th>Default</th><th>Value</th></tr>\n";
      foreach($confitems as $key=>$desc) {
        if (! isset($confbig[$key])) {
          echo "<tr><td>$desc</td><td>".htmlspecialchars($default[$key])."</td><td>";
          echo "<input type=\"text\" name=\"$key\" size=\"30\" maxlength=\"64000\" value=\"".htmlspecialchars($conf[$key])."\" />";
          echo "</td></tr>\n";
        } else {
          echo "<tr><td>$desc</td><td colspan=\"2\">".htmlspecialchars($default[$key])."</td></tr>";
          echo "<tr><td>&nbsp;</td><td colspan=\"2\"><textarea rows=\"5\" cols=\"80\" name=\"$key\">".htmlspecialchars($conf[$key])."</textarea>";
          echo "</td></tr>\n";
        }
      }
      echo "</table>\n";
      echo "<div><input type=\"submit\" /></div>\n";
      echo "</form>\n";
    }
  }

  $servers = $meta->getAllServers();
  $booted = $meta->getBootedServers();

  echo "<h1>Murmur ICE Interface demo</h1>\n";
  echo "<p><b>Warning:</b> This is not intended as a full management suite. It's an example. It has bugs. It has security holes. ";
  echo "It might make precious body parts fall off. <b>Do not run it on a production server</b>.\n";
  echo "</p>\n";
  echo "<p>\n";
  echo "<a href=\"?newserver\">[New Server]</a>\n";
  echo "</p>\n";
  foreach($servers as $s) {
    $name = $s->getConf("registername");
    if (! $name) {
      $name =  $default["registername"];
    }
    $id = $s->id();
    echo "<h1>SERVER #" . $id . " " .$name ."</h1>\n";
    echo "<p>\n";
    echo "<a href=\"?server=".$id."\">[Config]</a> ";
    if (in_array($s, $booted)) {
      echo "<a href=\"?uedit=".$id."\">[Manage Users]</a> ";
      echo "<a href=\"?stop=".$id."\">[Stop]</a> ";
    } else {
      echo "<a href=\"?delserver=".$id."\">[Delete Server]</a> ";
      echo "<a href=\"?start=".$id."\">[Start]</a> ";
    }
    echo "</p>\n";
    if (in_array($s, $booted)) {
      echo "<table><tr><th>Name</th><th>Channel</th><th>Actions</th></tr>\n";

      $channels = $s->getChannels();
      $players = $s->getUsers();

      foreach($players as $pid => $state) {
        $chan = $channels[$state->channel];
        echo "<tr><td>".$state->name."</td><td>".$chan->name."</td><td>";
        echo "<a href=\"?action=".$id."&amp;kick=$state->session\">[Kick]</a>";
        echo "</td></tr>\n";
      }
      echo "</table>\n";
    }
  }
} catch (Ice_Exception $ex) {
  echo "<p>\n<pre>\n";
  print_r($ex);
  echo "</pre>\n</p>\n";
}

?>
</body>
</html>
