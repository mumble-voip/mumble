<html>
<head>
<title>Userlist</title>
</head>
<body>
<?php

Ice_loadProfile();

try {
  # Replace IP address with the one your glacier2 is running on
  $router = $ICE->stringToProxy("Glacier2/router:tcp -p 4063 -h 128.39.114.1");
  $router = $router->ice_uncheckedCast("::Glacier2::Router")->ice_router(null);
  $session = $router->createSession("testuser", "testpass");
      
  $base = $ICE->stringToProxy("Meta:tcp -h 127.0.0.1 -p 6502")->ice_router($router);
  $meta = $base->ice_checkedCast("::Murmur::Meta");

  $servers = $meta->getBootedServers();
  $default = $meta->getDefaultConf();
  foreach($servers as $s) {
    $s = $s->ice_router($router);
    $name = $s->getConf("registername");
    if (! $name) {
      $name =  $default["registername"];
    }
    echo "<h1>SERVER #" . $s->id() . " " .$name ."</h1>\n";
    echo "<table><tr><th>Name</th><th>Channel</th></tr>\n";

    $channels = $s->getChannels();
    $players = $s->getUsers();

    foreach($players as $id => $state) {
      $chan = $channels[$state->channel];
      echo "<tr><td>".$state->name."</td><td>".$chan->name."</td></tr>\n";
    }
    echo "</table>\n";
  }
} catch (Ice_LocalException $ex) {
  print_r($ex);
}

?>
</body>
</html>
