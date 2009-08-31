#!/usr/bin/env python
# update license.html from the template

license = open('../LICENSE').read()
tmpl = open('license.tmpl').read()
s = tmpl.replace('{{LICENSE}}', license)
out = open('license.html', 'w').write(s)
