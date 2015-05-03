# -*- coding: utf-8 -*-

import sys, os

# on_rtd is whether we are on readthedocs.org
on_rtd = os.environ.get('READTHEDOCS', None) == 'True'

extensions = [
	'sphinx.ext.pngmath',
]

pngmath_latex_preamble = r'''
\usepackage{mathtools}
\usepackage{amssymb}

\usepackage{float}

\DeclareMathOperator{\bit}{bit}
\DeclareMathOperator{\band}{and}
\DeclareMathOperator{\lshift}{<<}
\DeclareMathOperator{\rshift}{>>}
\DeclareMathOperator{\append}{\triangleright}
'''

templates_path = ['_templates']
source_suffix = '.rst'
master_doc = 'index'

project = u'Mumble Protocol'
copyright = u'2012, Mumble Project'
version = '1.2.5'
release = '1.2.5-alpha'

exclude_patterns = ['_build']

pygments_style = 'sphinx'

if not on_rtd:  # only import and set the theme if we're building docs locally
    import sphinx_rtd_theme
    html_theme = 'sphinx_rtd_theme'
    html_theme_path = [sphinx_rtd_theme.get_html_theme_path()]
else:
    html_theme = 'default'

    html_theme_options = {
            'footerbgcolor':    '#555555',
            'relbarbgcolor':    '#222222',
            'sidebarbgcolor':   '#333333',
            'linkcolor':        '#696969',
            'visitedlinkcolor': '#adadad',
            'sidebarlinkcolor': '#cacaca',
            'headtextcolor':    '#000000',
    }

# Add any paths that contain custom themes here, relative to this directory.
#html_theme_path = []

html_title = project + " " + release

# The name of an image file (relative to this directory) to place at the top
# of the sidebar.
html_logo = '_static/mumble.png'

# The name of an image file (within the static path) to use as favicon of the
# docs.  This file should be a Windows icon file (.ico) being 16x16 or 32x32
# pixels large.
html_favicon = 'favicon.ico'

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

# If not '', a 'Last updated on:' timestamp is inserted at every page bottom,
# using the given strftime format.
html_last_updated_fmt = '%b %d, %Y'

# If true, SmartyPants will be used to convert quotes and dashes to
# typographically correct entities.
html_use_smartypants = True

# Custom sidebar templates, maps document names to template names.
#html_sidebars = {}

# Additional templates that should be rendered to pages, maps page names to
# template names.
#html_additional_pages = {}

html_domain_indices = False

html_show_sphinx = True
html_show_copyright = True
