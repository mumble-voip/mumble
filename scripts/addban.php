<?php

# This shows how to use structs from PHP.

Ice_loadProfile();

try {
  $base = $ICE->stringToProxy("Meta:tcp -h 127.0.0.1 -p 6502");
  $meta = $base->ice_checkedCast("::Murmur::Meta");
  
  $default = $meta->getDefaultConf();
  
  $server = $meta->getServer(1);
  
  $bans = $server->getBans();
  foreach($bans as $ban) {
    $a = ($ban->address >> 24) & 0xFF;
    $b = ($ban->address >> 16) & 0xFF;
    $c = ($ban->address >> 8) & 0xFF;
    $d = ($ban->address >> 0) & 0xFF;
    $mask = $ban->bits;
    echo "$a.$b.$c.$d/$mask\n";
  }
  
  $ban = new Murmur_Ban();
  $ban->address = (127 << 24) | (0 << 16) | (0 << 8) | 1;
  $ban->bits = 24;
  
  array_push($bans, $ban);
  
  $server->setBans($bans);
  
} catch (Ice_Exception $ex) {
  print_r($ex);
}
?>
