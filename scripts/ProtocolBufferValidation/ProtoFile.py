# Copyright 2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

import re
import os

from ProtoMessage import ProtoMessage, ProtoEnum
from FieldAnnotation import FieldAnnotation

class ProtoFile:
    def __init__(self, name, protoVersion, package):
        self.name = name
        self.protoVersion = protoVersion
        self.package = package
        self.messages = []
        self.enums = []

    def __eq__(self, other):
        if isinstance(other, self.__class__):
            return self.name == other.name and self.protoVersion == other.protoVersion and self.package == other.package \
                    and self.messages == other.messages and self.enums == other.enums
        else:
            return False

    def __str__(self):
        return "ProtoFile \"" + self.name + "\" { syntaxVersion=" + str(self.protoVersion) + " package=" + self.package + " messageCount=" \
            + str(len(self.messages)) + " enumCount=" + str(len(self.enums)) + " }"


    def getAnnotatedFields(self, annotationClass = None):
        """Gets all MessageFields that have an annotation set. If the annotationClass is specified,
        only those fields are returned that have an annotation of that class."""

        annotatedFields = []

        for currentMsg in self.messages:
            annotatedFields += currentMsg.getAnnotatedFields(annotationClass)

        return annotatedFields




    @staticmethod
    def parse(content, name):
        """Parses the given content as a protocol buffer specification and returns the created ProtoFile"""

        if not isinstance(content, str):
            raise RuntimeError("Expected string but got %s" % content.__class__.__name__)

        # get the annotations out of the comments in order to
        # not remove them with the content. Also make sure that the annotations
        # are on their own line
        content = re.sub("// (@.*)", "\n\\g<1>\n", content)
        # strip out comments
        content = re.sub("//.*", "", content)

        preambleStart = content.find("message")

        if preambleStart < 0:
            raise RuntimeError("Invalid ProtoFile format (can't find message decl)")

        preamble = content[:preambleStart]
        content = content[preambleStart:]

        preambleParts = preamble.split(";")

        # remove empty segments
        preambleParts = [x.strip() for x in preambleParts if x.strip()]

        packageName = None
        protoSyntax = None

        for currentStatement in preambleParts:
            if currentStatement.startswith("option"):
                # We don't care about options
                continue

            # Make sure the "=" will be its own part
            currentStatement = currentStatement.replace("=", " = ")
            parts = [x.strip() for x in currentStatement.split() if x.strip()]

            if len(parts) == 0:
                continue

            if parts[0] == "syntax" and len(parts) == 3:
                protoSyntax = parts[2]
            elif parts[0] == "package":
                packageName = parts[1]
            else:
                raise RuntimeError("Unexpected preamble segment in ProtoFile: %s" % currentStatement)

        if protoSyntax is None:
            # proto2 is the default
            protoSyntax = "\"proto2\""

        if packageName is None:
            raise RuntimeError("Can't find package name in ProtoFile")
        
        if protoSyntax.startswith("\"proto") and protoSyntax.endswith("\""):
            protoSyntaxVersion = int(protoSyntax[len("\"proto"):-1])
        else:
            raise RuntimeError("Invalid proto sytax spec: %s" % protoSyntax)
        
        protoFile = ProtoFile(name=name, protoVersion=protoSyntaxVersion, package=packageName)

        unclosedBraces = 0
        for i in range(len(content)):
            if unclosedBraces == 0 and content[i:i + len("message")] == "message":
                msg = ProtoMessage.parse(content, i)

                protoFile.messages.append(msg)
            elif unclosedBraces == 0 and content[i : i + len("enum")] == "enum":
                segments = [x.strip() for x in content[i : content.find("{", i)].split() if x.strip()]

                if not len(segments) == 2:
                    print(segments)
                    raise RuntimeError("Invalid enum declaration (found %d segments instead of 2)" % len(segments))

                # Segments is now ["enum", <enumName>]
                protoFile.enums.append(ProtoEnum(name=segments[1]))
            elif content[i] == "{":
                unclosedBraces += 1
            elif content[i] == "}":
                unclosedBraces -= 1

                if unclosedBraces < 0:
                    raise RuntimeError("Found closing brace that didn't match a previous opening one at offset" % i)


        return protoFile


    @staticmethod
    def fromFile(fileName):
        """Parses a ProtoFile from the contents of the file with the given name / at the given
        location. If this is not possible, this functionr returns None. Otherwise it'll return
        the parsed ProtoFile object."""

        if not os.path.exists(fileName):
            print("[ERROR]: File \"%s\" does not exist" % fileName)
            return None

        if not os.path.isfile(fileName):
            print("[ERROR]: File \"%s\" is not a file" % fileName)
            return None

        with open(fileName, "r") as content_file:
            return ProtoFile.parse(content_file.read(), os.path.basename(fileName))
