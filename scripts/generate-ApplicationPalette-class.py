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

role_multigettersetter_template = """
	QBrush get_%(role)s() {
		qWarning("get_%(role)s called. This shouldn't happen. Cannot return meaningful value.");
		return QBrush();
	}

	void set_%(role)s(const QBrush& brush) {
%(setters)s
	}
"""

role_multisetter_template="""		setProperty("%(prop)s", brush);
"""


paletteupdate_template ="""
			if (m_%(prop)s) {
				palette.setBrush(QPalette::%(group)s, QPalette::%(role)s, *m_%(prop)s);
			}
"""

variable_template = """		boost::optional<QBrush> m_%(prop)s;
"""

def rolename(role):
    return role.lower()

def groupname(group):
    return group.lower()

def propname(role, group):
    return rolename(role) + "_" + groupname(group)

if __name__ == "__main__":
    template = open(template_source, "r").read()

    variables = {"warning": "// Auto-generated from %s . Do not edit manually." % template_source,
                 "properties": "",
                 "getterssetters": "",
                 "paletteupdates": "",
                 "variables": ""}

    for role in color_role:
        # Add a property that sets all color groups to the same QBrush using
        # the setters of the single group properties.
        variables["properties"] += property_template % {"prop" : rolename(role)}
        role_multisetters = "".join([role_multisetter_template % {"prop": propname(role, group)} for group in color_group])
        variables["getterssetters"] += role_multigettersetter_template % {"role" : rolename(role),
                                                                          "setters": role_multisetters}

        # Add separate group properties
        for group in color_group:
            vars = {"prop" : propname(role, group),
                    "group" : group,
                    "role" : role}

            variables["properties"] += property_template % vars
            variables["getterssetters"] += gettersetter_template % vars
            variables["paletteupdates"] += paletteupdate_template % vars
            variables["variables"] += variable_template % vars


    open(target, "w").write(template % variables)