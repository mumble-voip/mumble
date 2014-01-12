#!/usr/bin/env python
# Extracts the progress of translations from the compilation
# log in easily readable form. Make sure to delete all .qm
# files beforehand.
#
# Usage: cat log | .\transtate.py
#

import sys
import re

def n(val):
    return (int(val) if val else 0)

if __name__ == "__main__":
    
    #--Regex matches strings like
    #
    #Updating 'mumble_zh_CN.qm'...
    #    Generated 1421 translation(s) (1145 finished and 276 unfinished)
    #    Ignored 89 untranslated source text(s)
    #    s:\dev\QtMumble\bin\lrelease.exe mumble_zh_TW.ts
    #Updating 'mumble_zh_TW.qm'...
    #    Generated 664 translation(s) (594 finished and 70 unfinished)
    #    Ignored 846 untranslated source text(s)
    
    update = re.compile(r"Updating '([\w\.]+)'\.\.\.\s+Generated (\d+) translation\(s\) \((\d+) finished and (\d+) unfinished\)(?:\s+ Ignored (\d+) untranslated source text\(s\))?")
    
    langs = 0
    s = 's'
    
    sortedbyuntranslated = sorted(update.findall(sys.stdin.read()), key=lambda s: (float(s[2]) / (n(s[1]) + n(s[4]))) if n(s[1]) else 10, reverse=True)
    
    for lang, total, finished, unfinished, ignored in sortedbyuntranslated:
        print "%s:" % lang
        
        if int(total) == 0:
            print "  Source language"
        else:
            realtotal = n(total) + n(ignored)
            percent = float(finished) / realtotal * 100
            
            print "  %d marked unfinished" % (n(unfinished))
            
            if n(ignored):
                print "  %d untranslated." % (n(ignored))

            print "  => %d%% done (total %d + %d)." % (percent, n(total), n(ignored))
        print
        langs += 1
        
    print "Number of languages: %d" % langs
    


        