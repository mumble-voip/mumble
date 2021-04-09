# Copyright 2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

from MessageField import MessageField, FieldRule


class FieldAnnotation:
    def __init__(self):
        self.name = None

    def __str__(self):
        if self.name is None:
            return "Invalid abstract annotation"
        else:
            return "@" + self.name

    def generateValidCheck(self, msgField):
        """Generates code appropriate for the given field.
        What the code is actually doing depends on the kind of annotation.
        The code always expecects that the currently processed ProtoBuf message
        object is named msg."""

        raise RuntimeError("Called abstract implementation")



    @staticmethod
    def parse(content):
        parts = [x.strip() for x in content.split() if x.strip()]

        if len(parts) == 0:
            raise RuntimeError("Invalid annotation format (no parts found): \"%s\"" % content)

        annotationName = parts[0]

        if not annotationName.startswith("@"):
            raise RuntimeError("Invalid annotation name (doesn't start with @): %s" % annotationName)
        
        annotationName = annotationName.lower()

        if annotationName == "@restrict":
            return RestrictAnnotation._parseBody(parts[1:])
        else:
            raise RuntimeError("Unknown annotation type %s" % annotationName)


class RestrictAnnotation:
    knownProperties = [ "size", "value" ]
    knownComparators = [ "<", ">", "==", "<=", ">=" ]

    def __init__(self, prop, comp, val):
        self.name = "restrict"
        self.prop = prop
        self.comp = comp
        self.val = val


    def __eq__(self, other):
        if isinstance(other, self.__class__):
            return self.prop == other.prop and self.comp == other.comp and self.val == other.val
        else:
            return False

    
    def __str__(self):
        return "@restrict " + self.prop + " " + self.comp + " " + self.val


    def generateValidCheck(self, msgField):
        """This function generates a boolean condition that will evaluate
        whether the given field is considered valid"""

        code = "("

        if msgField.fieldRule is FieldRule.OPTIONAL:
            code += "!msg.has_" + msgField.fieldName + "() || "

        code += "msg." + msgField.fieldName

        # Complete code to get the respective property
        if self.prop == "size":
            if msgField.fieldRule is FieldRule.REPEATED:
                code += "_size()"
            elif msgField.fieldType == "string":
                code += "().size()"
            else:
                raise RuntimeError("Don't know how to get size for type %s" % msgField.fieldType)

        elif self.prop == "value":
            code += "()"
        else:
            raise RuntimeError("Property %s not yet implemented" % self.prop)

        # Complete the comparison
        code += " " + self.comp + " " + self.val

        code += ")"

        return code


    @staticmethod
    def _parseBody(bodyParts):
        # we expect the body to be of format <property> <comparater> <value>
        if not len(bodyParts) == 3:
            raise RuntimeError("The body for a restrict annotation must have 3 parts, but the given one has %d" % len(parts))

        prop = bodyParts[0].lower()
        comp = bodyParts[1]
        val  = bodyParts[2]
        
        if not prop in RestrictAnnotation.knownProperties:
            raise RuntimeError("Unknown property %s" % prop)

        if not comp in RestrictAnnotation.knownComparators:
            raise RuntimeError("Unknown comparator %s" % comp)

        return RestrictAnnotation(prop=prop, comp=comp, val=val)
