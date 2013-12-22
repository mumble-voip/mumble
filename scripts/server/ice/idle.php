<?
# idle.php - Sample "auto-idle mover" script. Run in a cron job.
# Copyright (c) 2010, Jamie Fraser / MumbleDog/Sabrienix
# 
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 
#    * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above
#      copyright notice, this list of conditions and the following
#      disclaimer in the documentation and/or other materials provided
#      with the distribution.
#    * Neither the name of MumbleDog/Sabrienix nor the names of its
#      contributors may be used to endorse or promote products derived
#      from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.  
#
######################################################################
# 
# NOTE WELL: This is just an example. It has no error checking, left
# out both because of my own laziness and to keep the example simple.
# Steal the implementation and put it in your own script, or just use
# as-is under the full understanding it might end the world as you
# know it.

# "AFK" channel ID
$afk = 4;

# Time in seconds user must be AFK to be moved
$idle = 5000;

# Ice Server
$ice = 'Meta:tcp -h 127.0.0.1 -p 6502';

# Murmur ID #
$mid = 0;

######################################################################

if (!extension_loaded('ice')) {
	exit('Error: This example script needs the PHP extension for Ice to be loaded.');
}
if ((!function_exists('Ice_intVersion') || Ice_intVersion() < 30400) || !function_exists('Ice_loadProfile')) {
	exit('Error: This example script is compatible only for php-ice versions prior to 3.4.');
}

Ice_loadProfile();
$base = $ICE->stringToProxy($ice);
$murmur = $base->ice_checkedCast('::Murmur::Meta');

$m = $murmur->getServer($mid);

$users = $m->getUsers();

foreach ($users as $u) {
	printf("%s: %s\n", $u->name, $u->idlesecs);
	if ($u->idlesecs > $idle && $u->channel != $afk) {
		$state = $m->getState($u->session);
		if ($state) {
			$state->channel = $afk;
			$m->setState($state);
		}
	}
}
?>