<?php

# This shows how to use structs from PHP.

if (Ice_intversion() >= 30400) {
  require 'Ice.php';
  require 'Murmur.php';
} else {
  Ice_loadProfile();
}

try {
  if (Ice_intversion() >= 30400) {
    $ICE = Ice_initialize();
  }

  $base = $ICE->stringToProxy("Meta:tcp -h 127.0.0.1 -p 6502");
  $meta = $base->ice_checkedCast("::Murmur::Meta");

  $default = $meta->getDefaultConf();

  $server = $meta->getServer(1);
  

  $bans = $server->getBans();

  print_r($bans);
  foreach($bans as $ban) {
    for($i=0;$i<16;$i+=2) {
      echo sprintf("%02x%02x", $ban->address[$i], $ban->address[$i+1]);
      if ($i != 14) {
        echo ":";
      }
    }
    $mask = $ban->bits;
    echo "/$mask\n";
  }

  $ban = new Murmur_Ban();
  $ban->address = array(0,0,0,0,0,0,0,0,0,0,255,255,127,0,0,1);
  $ban->bits = 128;
  
  array_push($bans, $ban);

  $server->setBans($bans);

} catch (Ice_Exception $ex) {
  print_r($ex);
}
?>
