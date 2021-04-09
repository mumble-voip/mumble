# Copyright 2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

from MessageField import MessageField
from FieldAnnotation import FieldAnnotation


class Interval:
    """A class specifying an interval with a start and an end offset as well
    as potential nested sub-intervals"""

    def __init__(self, start = None, end = None):
        self.start = start
        self.end = end
        self.parent = None
        self.children = []


    def setParent(self, interval):
        self.parent = interval
        interval.children.append(self)


class ProtoEnum:
    """Class representing an enum declaration in a protocol buffer specification"""

    def __init__(self, name):
        self.name = name

    def __eq__(self, other):
        if isinstance(other, self.__class__):
            return self.name == other.name
        else:
            return False


class ProtoMessage:
    """Class representing a message declaration in a protocol buffer specification"""

    def __init__(self, name):
        self.name = name
        self.messageFields = []
        self.nestedMessages = []
        self.nestedEnums = []


    def __eq__(self, other):
        if isinstance(other, self.__class__):
            return self.name == other.name and self.messageFields == other.messageFields \
                    and self.nestedMessages == other.nestedMessages and self.nestedEnums == other.nestedEnums
        else:
            return False

    def __str__(self):
        return "message " + self.name + "{ " + str(len(self.messageFields)) + " fields, " + str(len(self.nestedMessages)) + " messages, " \
            + str(len(self.nestedEnums)) + " enums }"

    def getAnnotatedFields(self, annotationClass = None):
        """Gets all MessageFields that have an annotation set. If the annotationClass is specified,
        only those fields are returned that have an annotation of that class."""

        annotatedFields = []

        for currentMsg in self.nestedMessages:
            annotatedFields += currentMsg.getAnnotatedFields(annotationClass)

        for currentField in self.messageFields:
            for currentAnnotation in currentField.fieldAnnotations:
                if currentAnnotation.__class__ == annotationClass or annotationClass is None:
                    annotatedFields.append(currentField)
                    break

        return annotatedFields




    @staticmethod
    def parse(content, offset = 0):
        """Parses a ProtoBuf message in the given content starting at
        the given offset"""

        messageBodyInterval = None

        # Gets the interval of the message body and potentially the sub-intervals of
        # nested declarations
        for i in range(offset, len(content)):
            if content[i] == "}":
                messageBodyInterval.end = i
                if messageBodyInterval.parent is None:
                    endIndex = i
                    break
                else:
                    messageBodyInterval = messageBodyInterval.parent
            elif content[i] == "{":
                if messageBodyInterval is None:
                    messageBodyInterval = Interval(start=i)
                else:
                    prevInterval = messageBodyInterval
                    messageBodyInterval = Interval(start=i)

                    messageBodyInterval.setParent(prevInterval)

        if not messageBodyInterval.parent is None:
            raise RuntimeError("Unbalanced nesting in message declaration: %s" % content[offset:])

        (message, enum) = ProtoMessage._processInterval(messageBodyInterval, content)


        if not enum is None:
            raise RuntimeError("Expected to parse message but instead parsed enum")

        return message


    @staticmethod
    def _processInterval(interval, content):
        """Processes the given body-interval of a ProtoBuf message declaration.
        If the given interval has children, they will be processed first."""

        nestedMessages = []
        nestedEnums = []

        for currentChild in interval.children:
            (message, enum) = ProtoMessage._processInterval(currentChild, content)

            if not message is None:
                nestedMessages.append(message)
            elif not enum is None:
                nestedEnums.append(enum)


        # get whether the nested thing is an enum or a message and also get its name
        words = [x for x in content[:interval.start].split() if x]

        if len(words) < 2:
            raise RuntimeError("Unexpected format previous to interval body")

        name = words[len(words) - 1]
        kind = words[len(words) - 2]

        body = content[interval.start + 1 : interval.end]

        # Remove all sub-intervals
        indexCorrection = interval.start + 1
        for currentChild in interval.children:
            # also remove the leading "message <name>" (or the same for an enum)
            # Therefore we have to delete the 2 words that come before the beginning
            # of the current child interval
            beginIndex = -1
            lastSpace = False
            processedName = False
            inName = False
            for i in range(currentChild.start - indexCorrection - 1, -1, -1):
                if body[i].isspace():
                    if inName:
                        inName = False
                        if processedName:
                            beginIndex = i
                            break
                        else:
                            processedName = True

                    lastSpace = True
                    continue

                lastSpace = False
                inName = True

            if beginIndex < 0:
                raise RuntimeError("Negative index")

            # Now that we know where the complete nested statement starts, we can remove it
            body = body[: beginIndex] + body[currentChild.end - indexCorrection + 1 :]
            # We have to update our index correction in case there are more nested intervals to come
            indexCorrection += (currentChild.end - indexCorrection) - beginIndex + 1



        message = None
        enum = None
        if kind == "message":
           message = ProtoMessage._generateMessageFromBody(body, name) 

           message.nestedMessages = nestedMessages
           message.nestedEnums = nestedEnums
        elif kind == "enum":
            # We don't care about enums except that there exists an enum with a given name
            enum = ProtoEnum(name)

            if not len(nestedEnums) == 0 or not len(nestedMessages) == 0:
                raise RuntimeError("Error: An enum can't have nested enums/messages")
        else:
            raise RuntimeError("Unknown nested kind %s" % kind)

        return (message, enum)
        


    def _generateMessageFromBody(msgBody, msgName):
        """Parse the body of a message declaration with the given name. This function 
        expects that there are no nested message/enum declarations contained in the body
        (anymore)"""

        message = ProtoMessage(msgName)

        fieldParts = msgBody.split(";")

        for currentField in fieldParts:
            currentField = currentField.strip()

            if not currentField:
                continue

            annotations = []

            if "\n" in currentField:
                # Handle annotations
                lines = [x.strip() for x in currentField.split("\n") if x.strip()]

                for currentLine in lines[:len(lines) - 1]:
                    annotations.append(FieldAnnotation.parse(currentLine))
                    

                # the last line must be the one with the actual field-spec
                currentField = lines[len(lines) - 1]
                pass

            # currentField does no longer contain the semicolon as it has been removed
            # in the splitting process. Therefore we have to attach it again
            field = MessageField.parse(currentField + ";")

            field.fieldAnnotations = annotations

            message.messageFields.append(field)

        return message
