# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

import re

class CommitFormatError(Exception):

    def __init__(self, msg):
        Exception(msg)

class CommitMessage:
    knownTypes = ["BREAK", "FEAT", "FIX", "FORMAT", "DOCS", "TEST", "MAINT", "CI", "REFAC", "BUILD", "TRANSLATION", "CHANGE", "REVERT"]

    def __init__(self, commitString):
        lines = commitString.strip().split("\n")

        if len(lines) < 1 or lines[0].strip() == "":
            raise CommitFormatError("Invalid commit content")

        subjectPattern = re.compile("^([A-Z\-_/]+)(?:\(([0-9a-zA-Z\-_, ]+)\))?:\s*(.+)$")

        match = re.match(subjectPattern, lines[0])

        if not match:
            raise CommitFormatError("The provided commit's subject line does not follow the required pattern")

        types = match.group(1).split("/") if not match.group(1) is None else []
        scopes = match.group(2).split(",") if not match.group(2) is None else []
        summary = match.group(3).strip() if not match.group(3) is None else ""

        if len(types) == 0:
            raise CommitFormatError("Missing type")

        if len(summary) == 0:
            raise CommitFormatError("Missing summary")

        self.m_summary = summary

        types = [x.strip() for x in types]
        scopes = [x.strip().lower() for x in scopes]

        for currentType in types:
            if currentType == "":
                raise CommitFormatError("Subsequent \"/\" not allowed for separating types")

            if not currentType in self.knownTypes:
                raise CommitFormatError("Unknown type \"%s\" (or incorrect spelling)" % currentType)

        self.m_types = types

        for currentScope in scopes:
            if currentScope == "":
                raise CommitFormatError("Empty scope not allowed")
        
        self.m_scopes = scopes

        self.m_body = "\n".join(lines[1 : ]) if len(lines) > 1 else ""


