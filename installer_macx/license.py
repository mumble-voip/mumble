#!/usr/bin/env python
# update license.html from the template

l = open('../LICENSE').read()
l = l.replace('<', '&lt;').replace('>', '&gt;')
tmpl = open('license.tmpl').read()
s = tmpl.replace('{{LICENSE}}', l)
out = open('license.html', 'w').write(s)
