#!/usr/bin/perl
#
# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

#
# Generate a Mac OS X Finder .DS_Store file for use on the Mumble disk image.
#

#
# Notes:
#
# The use of binary plists inside the DS_Store seems to be new for the Finder in Mac OS X 10.6 (Snow Leopard),
# but we'll go with this for now. It falls back somewhat gracefully for older versions OS X, since the icons
# will still be placed in their correct positions.
#

use Data::Plist::BinaryWriter;
use Mac::Finder::DSStore::BuddyAllocator;
use Mac::Finder::DSStore;
use IO::File;
use strict;

my $plist = Data::Plist::BinaryWriter->new(serialize => 0);

my $bwsp = $plist->write([dict => {
	"ShowPathbar" => [ false => 0 ],
	"ShowSidebar" => [ false => 0 ],
	"ShowStatusBar" => [ false => 0 ],
	"ShowToolbar" => [ false => 0 ],
	"SidebarWidth" => [ integer => 192 ],
	"WindowBounds" => [ string => "{{388, 166}, {530, 525}}" ],
}]);

my $icvp = $plist->write([ dict => {
	"arrangeBy" => [ string => "grid" ],

	# these are floats [0, 1]
	"backgroundColorBlue" => [ integer => 1 ],
	"backgroundColorGreen" => [ integer => 1 ],
	"backgroundColorRed" => [ integer => 1 ],

	# a mac os alias record or the background image.
	# fixme(mkrautz): add our pretty background. :)
	"backgroundImageAlias" => [ data => "" ],

	# background type. 1=color, 2=image
	"backgroundType" => [ integer => 1 ],

	"gridOffsetX" => [ integer => 0 ],
	"gridOffsetY" => [ integer => 0 ],
	"gridSpacing" => [ integer => 100 ],

	"iconSize" => [ integer => 80 ],
	"labelOnBottom" => [ true => 1 ],
	"showIconPreview" => [ true => 1 ],
	"showItemInfo" => [ false => 0 ],
	"textSize" => [ integer => 12 ],
	"viewOptionsVersion" => [ integer => 0 ],
}]);

my @entries = ();

# styling
my %styles = (
	'bwsp' => $bwsp,
	'icvp' => $icvp,
);

for my $key (keys %styles) {
	my $e = Mac::Finder::DSStore::Entry->new(".", $key);
	$e->value($styles{$key});
	push(@entries, $e);
}

# file info
my %iconpos = (
	"Mumble.app" => [ 54, 40 ],
	"Applications" => [ 466, 40 ],
	"ReadMe.txt" => [ 54, 416 ],
	"Changes.txt" => [ 157, 416 ],
	"Murmur Extras" => [ 363, 416 ],
	"Licenses" => [ 466, 416 ],
);

for my $key (keys %iconpos) {
	my $e = Mac::Finder::DSStore::Entry->new($key, 'Iloc');
	$e->value(pack('NNnnnn', $iconpos{$key}[0], $iconpos{$key}[1],  65536, 65536, 65536, 65536, 0));
	push(@entries, $e);
}

@entries = sort { $a->cmp($b) } @entries;

my $store = Mac::Finder::DSStore::BuddyAllocator->new(new IO::File "DS_Store", '>');
&Mac::Finder::DSStore::putDSDBEntries($store, \@entries);
$store->writeMetaData;
$store->close;
