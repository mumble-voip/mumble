# Copyright 2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

from enum import Enum
import re

class FieldRule(Enum):
    """Enum represening the different field rules possible in a protocol buffer specification"""

    SINGULAR = 0
    REPEATED = 1
    OPTIONAL = 2
    REQUIRED = 3

    @staticmethod
    def resolve(name):
        lowerName = name.lower()

        for currentRule in FieldRule:
            if currentRule.name.lower() == lowerName:
                return currentRule

        return None



class MessageField:
    """Class representing a field inside a message in a protocol buffer specification"""

    def __init__(self, fieldName, fieldType, fieldRule, fieldNumber):
        self.fieldName = fieldName
        self.fieldType = fieldType

        # Make sure fieldRule is actually of type FieldRule
        if isinstance(fieldRule, FieldRule) or fieldRule is None:
            self.fieldRule = fieldRule
        else:
            rule = FieldRule.resolve(fieldRule)

            if rule is None:
                raise TypeError("Can't convert %s to FieldRule" % str(fieldRule))
            else:
                self.fieldRule = rule

        # Make sure fieldNumber is an int
        self.fieldNumber = int(fieldNumber)
        self.fieldAnnotations = []


    def __eq__(self, other):
        """Overload of the == operator"""

        if isinstance(other, self.__class__):
            return (self.fieldName == other.fieldName
                    and self.fieldType == other.fieldType
                    and self.fieldRule == other.fieldRule
                    and self.fieldNumber == other.fieldNumber
                    and self.fieldAnnotations == other.fieldAnnotations)
        else:
            return False

    def __str__(self):
        strRep = ""

        if not self.fieldRule is None:
            strRep += self.fieldRule.name + " "
        
        strRep += str(self.fieldType) + " " + self.fieldName + " = " + str(self.fieldNumber) \
                + " (" + str(len(self.fieldAnnotations)) + " annotations)"
        
        return strRep



    @staticmethod
    def parse(content, offset = 0):
        """Parses a ProtoBuf message field sepcification in the given content
        starting at the given offset"""

        endIndex = content.find(";", offset)

        if endIndex < 0:
            raise RuntimeError("Unexpected format for message field (no delimiting semicolon): %s" % content[offset:])

        line = content[offset : endIndex]

        # Make sure the equals sign is surrounded by whitespace
        line = line.replace("=", " = ")

        # strip off amy potential options (e.g. [packed = true]
        line = re.sub("\\[[^\\]]*\\]\\s*$", "", line)

        segments = line.split()
        
        # remove empty segments (empty strings convert to false)
        segments = [x for x in segments if x]

        if len(segments) == 4:
            # The message has format <type> <name> = <number>
            if not segments[2] == "=":
                raise RuntimeError("Unexpected format for message field (\"=\" not at expected position): %s" % line)

            return MessageField(fieldName=segments[1], fieldType=segments[0], fieldRule=None, fieldNumber=segments[3])

        elif len(segments) == 5:
            # The message has format <rule> <type> <name> = <number>
            if not segments[3] == "=":
                raise RuntimeError("Unexpected format for message field (\"=\" not at expected position): %s" % line)

            return MessageField(fieldName=segments[2], fieldType=segments[1], fieldRule=segments[0], fieldNumber=segments[4])

        else:
            raise RuntimeError("Unexpected format for message field (got %d segments instead of 4 or 5): %s" % (len(segments), line))

