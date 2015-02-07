#!/usr/bin/perl
# perl avail_mac.pl < overlay.c init_mac.c > avail_mac.h

%funcs = ();
while (<STDIN>) {
	foreach (split(/([ \t])/, $_)) {
		@glfunc = ($_ =~ /(gl[A-VY-Z0-9][a-zA-Z0-9]+)/);
		foreach $func(@glfunc) {
			$funcs{$func} = 1;
		}
	}
}

print "#define AVAIL_ALL_GLSYM ( \\\n";
foreach my $key (keys %funcs) {
	print "\tAVAIL($key) && \\\n";
}
print "\t1)\n";
