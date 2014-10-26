#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Generates the Mumble palette generation class from
# template_source and writes it into target after
# template expansion.

template_source = "ApplicationPaletteTemplate.h"
target = "ApplicationPalette.h"

color_group = ["Active", "Disabled", "Inactive"]
color_role = [ "WindowText", "Button", "Light", "Midlight", "Dark", "Mid",
              "Text", "BrightText", "ButtonText", "Base", "Window", "Shadow",
              "Highlight", "HighlightedText",
              "Link", "LinkVisited",
              "AlternateBase",
              "ToolTipBase", "ToolTipText"]



template = open(template_source, "r").read()

variables = {"warning": "// Auto-generated from %s . Do not edit manually." % template_source,
             "properties": "",
             "getterssetters": "",
             "paletteupdates": "",
             "variables": ""}

property_template = """		Q_PROPERTY(QBrush %(prop)s READ get_%(prop)s WRITE set_%(prop)s)
"""

gettersetter_template = """
		QBrush get_%(prop)s() {
			if (!m_%(prop)s)
				return QBrush();

			return *m_%(prop)s;
		}

		void set_%(prop)s(const QBrush& brush) {
			m_%(prop)s = brush;
		}
"""

paletteupdate_template ="""
			if (m_%(prop)s) {
				palette.setBrush(QPalette::%(group)s, QPalette::%(role)s, *m_%(prop)s);
			}
"""

variable_template = """		boost::optional<QBrush> m_%(prop)s;
"""


for group in color_group:
    for role in color_role:

        vars = {"prop" : group.lower() + "_" + role.lower(),
                "group" : group,
                "role" : role}

        variables["properties"] += property_template % vars
        variables["getterssetters"] += gettersetter_template % vars
        variables["paletteupdates"] += paletteupdate_template % vars
        variables["variables"] += variable_template % vars


open(target, "w").write(template % variables)