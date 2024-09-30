#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# Generates the Mumble palette generation class from
# template_source and writes it into target after
# template expansion.

import argparse
import os

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
		if (%(comparators)s) {
			return %(representative)s;
		}
		return QBrush();
	}

	void set_%(role)s(const QBrush& brush) {
%(setters)s
	}
"""

role_multisetter_template="""		m_%(prop)s = brush;
"""


paletteupdate_template ="""
			if (m_%(prop)s) {
				newPalette.setBrush(QPalette::%(group)s, QPalette::%(role)s, *m_%(prop)s);
			}
"""

variable_template = """		boost::optional<QBrush> m_%(prop)s;
"""

reset_template = """			m_%(prop)s = boost::none;
"""

def rolename(role):
    return role.lower()

def groupname(group):
    return group.lower()

def propname(role, group):
    return rolename(role) + "_" + groupname(group)


def add_role_property(variables, role):
    """
    Add a property that sets all color groups to the same QBrush using
    the setters of the single group properties.
    """
    variables["properties"] += property_template % {"prop": rolename(role)}
    # Build a comparator that checks whether all properties
    # are equal and can be represented as one QBrush.
    all_groups_equal = " && ".join(
        ['property("%s") == property("%s")' % (propname(role, color_group[0]), propname(role, color_group[i])) for i in
         range(1, len(color_group))])
    role_representative = 'qvariant_cast<QBrush>(property("%s"))' % propname(role, color_group[0])
    role_multisetters = "".join([role_multisetter_template % {"prop": propname(role, group)} for group in color_group])
    variables["getterssetters"] += role_multigettersetter_template % {"role": rolename(role),
                                                                      "comparators": all_groups_equal,
                                                                      "representative": role_representative,
                                                                      "setters": role_multisetters}

def add_role_group_property(variables, role, group):
    """
    Add separate group properties.
    """
    vars = {"prop" : propname(role, group),
            "group" : group,
            "role" : role}

    variables["properties"] += property_template % vars
    variables["getterssetters"] += gettersetter_template % vars
    variables["paletteupdates"] += paletteupdate_template % vars
    variables["variables"] += variable_template % vars
    variables["propertyresets"] += reset_template % vars


if __name__ == "__main__":
    parser = argparse.ArgumentParser("Turns the ApplicationPalette template into an actually usable source file")
    parser.add_argument("--template", help="Path to the template file", metavar="PATH", required=True)
    parser.add_argument("--output", help="Path to the file to which to write the generated source file", metavar="PATH", default=None)

    args = parser.parse_args()

    template = open(args.template, "r").read()

    variables = {"warning": "// Auto-generated from %s . Do not edit manually." % os.path.basename(args.template),
                 "properties": "",
                 "propertyresets": "",
                 "getterssetters": "",
                 "paletteupdates": "",
                 "variables": ""}

    for role in color_role:
        add_role_property(variables, role)

        for group in color_group:
            add_role_group_property(variables, role, group)


    generatedSource = template % variables

    if args.output:
        open(args.output, "w").write(generatedSource)
    else:
        print(generatedSource)
