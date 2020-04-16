#!/usr/bin/env python3

import sys
import os.path
import re
from datetime import datetime


class CppCode:
    """A wrapper class for storing the implementation specific code
    as well as the header code for a cpp-file.
    """

    def __init__(self):
        self.header = ""
        self.impl = ""


    def __iadd__(self, other):
        """Overload the += operator so that it'll add to both: implementation and header
        """
        self.header += other
        self.impl += other

        return self


class MessageType:
    """Wrapper class for a ProtoBuf message. It contains information
    about its name, the name of its fields and their types
    """

    def __init__(self, name, parents):
        self.parents = parents
        self.name = name
        self.fieldNames = []
        self.fieldTypes = {}


    def fullName(self):
        """Get the fully qualified name (without the package) of this message
        """

        fullName = ""

        for currentParent in self.parents:
            fullName += currentParent.name + "::"

        fullName += self.name

        return fullName


    def __str__(self):
        strRep = "message " + self.fullName() + " {\n"

        for fieldName in self.fieldNames:
            fieldType = self.fieldTypes[fieldName]

            strRep += "\t" + fieldType + " " + fieldName + "\n"

        strRep += "}"

        return strRep


    def __eq__(self, other):
        if other is None:
            return False

        # Assume that the fields are the same if the ful name is the same
        return self.fullName() == other.fullName()



def printHelp():
    """Print a help message that explains how to use this script
    """

    print("This script generates validators for the ProtobufMessages used by Mumble.")
    print("Usage:")
    print("generateProtoValidator.py <ProtoFile>")


def readMessage(lines, currentLineIndex, messages, parents):
    """Read a message from a ProtoBuf specification (already split up in lines
    """

    currentMessage = None

    it = iter(range(currentLineIndex, len(lines)))
    for lineIndex in it:
        currentLine = lines[lineIndex].strip()

        if len(currentLine) == 0:
            continue

        if currentLine.startswith("message"):
            if not currentMessage is None:
                # read nested message declaration
                nextLineIndex = readMessage(lines, lineIndex, messages, parents + [currentMessage])

                for i in range(nextLineIndex - lineIndex - 1):
                    try:
                        next(it)
                    except Exception:
                        pass
                continue

            messageName = currentLine.split()[1]
            currentMessage = MessageType(messageName, parents)
        elif currentLine.startswith("enum"):
            # skip enum declaration
            while not "}" in lines[lineIndex]:
                lineIndex = lineIndex + 1
                next(it)
        elif "}" in currentLine:
            # message is done
            messages.append(currentMessage)
            return lineIndex + 1
        else:
            # a new field for the message
            # each field type consists of 2 words: (optional|repeated) name
            parts = currentLine.split()
            fieldType = parts[0] + " " + parts[1]
            fieldName = parts[2]

            currentMessage.fieldNames.append(fieldName)
            currentMessage.fieldTypes[fieldName] = fieldType

    return len(lines)



def extractMessages(protoFile):
    """Parse the given ProtoBuf file and get the package name as well as all
    specified message types
    """

    with open(protoFile, "r") as content_file:
        content = content_file.read()

    # remove comments
    content = re.sub("//[^\\n]*", "", content)

    # remove line with "syntax = proto2"
    content = re.sub("syntax\\s*=[^\\n]*", "", content)

    # remove options
    content = re.sub("option\\s+[^\\n]*", "", content)

    # get package name
    content = content[content.find("package") + len("package"):]
    packageName = content[0:content.find(";")].strip()
    content = content[content.find(";") + 1:]

    lines = content.split("\n")

    messages = []

    lineIndex = readMessage(lines, 0, messages, [])

    while lineIndex < len(lines):
        lineIndex = readMessage(lines, lineIndex, messages, [])

    return (packageName, messages)


def isContained(typeName, messages):
    """Check whether a Message of the given name exists in the given list of messages
    """

    for currentMsg in messages:
        if currentMsg.name == typeName:
            return True

    return False

def getFullName(typeName, messages):
    """Get the full name of a message with the given name that exists in the given list of messages
    """

    for currentMsg in messages:
        if currentMsg.name == typeName:
            return currentMsg.fullName()


def generateCodeFor(fieldName, fieldType, fieldModifier, messageName, generateFixCode=False):
    """Generate code for the field of given name and type. The generated code can
    either be for checking the validity of the field or (if generateFixCode is True)
    for making the field valid, if it isn't already.
    """

    code = ""

    if (messageName == "TextMessage" and fieldName == "message"
            or messageName == "UserState" and fieldName == "comment"
            or messageName == "ChannelState" and fieldName == "description"):
        return "// The field \"%s\" in the \"%s\" message is validated by Server::isTextAllowed so no additional validation is necessary\n" % (fieldName, messageName)

    if messageName == "ServerSync" and fieldName == "welcome_text":
        return "// Don't restrict the welcome message as it might have images embedded which can easily exceed the normal message length\n"

    if fieldModifier == "repeated":
        if fieldType == "string":
            code += "for (int i = 0; i < msg.%s_size(); i++) {\n" % fieldName
            code += "\tif (msg.%s(i).size() > MAX_STRING_LENGTH) {\n" % fieldName

            if generateFixCode:
                code += "\t// Only use a substring as the original String is too long\n"
                code += "\t\t*msg.mutable_%s(i) = msg.%s(i).substr(0, MAX_STRING_LENGTH);\n" % (fieldName, fieldName)
                code += "\n"
                code += "\t\tchanged = true;\n"
            else:
                code += "\t\treturn false;\n"

            code += "\t}\n"
            code += "}\n"
    else:
        if fieldType == "string":
            code += "if (msg.%s().size() > MAX_STRING_LENGTH) {\n" % fieldName

            if generateFixCode:
                code += "\t// Only use a substring as the original String is too long\n"
                code += "\tmsg.set_%s(msg.%s().substr(0, MAX_STRING_LENGTH));\n" % (fieldName, fieldName)
                code += "\n"
                code += "\tchanged = true;\n"
            else:
                code += "\treturn false;\n"

            code += "}\n"

    return code


def generateValidator(packageName, messages):
    """Generate the CppCode for the validator for the given messages in the given package
    """

    code = CppCode()
    code += "// Copyright %s The Mumble Developers. All rights reserved.\n" % ("%s%d" % ("" if datetime.now().year == 2020 else "2020-", datetime.now().year))
    code += "// Use of this source code is governed by a BSD-style license\n"
    code += "// that can be found in the LICENSE file at the root of the\n"
    code += "// Mumble source tree or at <https://www.mumble.info/LICENSE>\n"
    code += "\n"
    code += "// This code has been automatically generated via scripts/generateProtoValidator.py on %s\n" % (datetime.now().strftime("%Y-%m-%d"))
    code += "// DO NOT EDIT THIS FILE MANUALLY\n"
    code += "\n"
    code += "// This file contains validation functions for the ProtoMessages of package " + packageName + "\n"
    code += "\n"

    code.header += "#ifndef MUMBLE_PROTOVALIDATOR_H_\n"
    code.header += "#define MUMBLE_PROTOVALIDATOR_H_\n"
    code.header += "\n"
    code.header += "#include \"Mumble.pb.h\"\n"
    code.header += "\n"
    code.header += "#include <string>\n"

    code.impl += "#include \"ProtoValidator.h\"\n"
    code.impl += "\n"
    code.impl += "#include <QtGlobal>\n"

    code += "\n"

    code += "namespace ProtoValidator {\n"


    # define some constants that are being used for validation
    code.impl += "\t// Restrict the size of any string that we'll accept\n"
    code.impl += "\tconst size_t MAX_STRING_LENGTH = 5000;\n"
    code.impl += "\n"


    for currentMessage in messages:
        code.header += "\t/// @param msg A const reference to the proto message that is validated\n"
        code.header += "\t/// @returns Whether the given message was considered valid\n"
        code.header += "\tbool isValid(const %s::%s &msg);\n" % (packageName, currentMessage.fullName())
        code.header += "\n"
        code.header += "\t/// Turns this message into a valid one. If it is valid to begin with, no changes\n"
        code.header += "\t/// \twill be made to it.\n"
        code.header += "\t///\n"
        code.header += "\t/// @param msg A reference to the proto message that is validated\n"
        code.header += "\t/// @returns Whether the given message was ichanged\n"
        code.header += "\tbool makeValid(%s::%s &msg);\n" % (packageName, currentMessage.fullName())
        code.header += "\n"

        isValidCode = ""
        makeValidCode = ""

        noChecks = []
        for currentField in currentMessage.fieldNames:
            fieldTypeParts = currentMessage.fieldTypes[currentField].split()

            fieldModifier = fieldTypeParts[0]
            fieldType = fieldTypeParts[1]

            if isContained(fieldType, messages):
                if fieldModifier == "repeated":
                    isValidCode += "for (int i = 0; i < msg.%s_size(); i++) {\n" % currentField
                    isValidCode += "\tif (!isValid(msg.%s(i))) {\n" % currentField
                    isValidCode += "\t\treturn false;\n"
                    isValidCode += "\t}\n"
                    isValidCode += "}\n"

                    makeValidCode += "for (int i = 0; i < msg.%s_size(); i++) {\n" % currentField
                    makeValidCode += "\tchanged = changed || makeValid(*msg.mutable_%s(i));\n" % currentField
                    makeValidCode += "}\n"
                else:
                    isValidCode +="if (!isValid(msg.%s())) {\n" % currentField
                    isValidCode += "\treturn false;\n"
                    isValidCode += "}\n"

                    makeValidCode += "if (!isValid(msg.%s())) {\n" % currentField
                    makeValidCode += "\tchanged = changed || makeValid(*msg.mutable_%s());\n" % currentField
                    makeValidCode += "}\n"

                isValidCode += "\n"
                makeValidCode += "\n"
            else:
                genCode = generateCodeFor(currentField, fieldType, fieldModifier, currentMessage.fullName(), generateFixCode=False)

                if genCode == "" or genCode.startswith("//") and genCode.count("\n") <= 1:
                    noChecks.append(currentField)

                if not genCode == "":
                    isValidCode += genCode + "\n"
                    makeValidCode += generateCodeFor(currentField, fieldType, fieldModifier, currentMessage.fullName(), generateFixCode=True) + "\n"


        for currentNoCheck in noChecks:
            isValidCode += "// No check for field %s\n" % currentNoCheck
            makeValidCode += "// No check for field %s\n" % currentNoCheck

        if len(noChecks) == len(currentMessage.fieldNames):
            isValidCode += "Q_UNUSED(msg);\n"
            makeValidCode += "Q_UNUSED(msg);\n"


        code.impl += "\tbool isValid(const %s::%s &msg) {\n" % (packageName, currentMessage.fullName())
        code.impl += "\t\t" + isValidCode.replace("\n", "\n\t\t").strip() + "\n\n"
        code.impl += "\t\treturn true;\n"
        code.impl += "\t}\n"
        code.impl += "\n"
        code.impl += "\tbool makeValid(%s::%s &msg) {\n" % (packageName, currentMessage.fullName())
        code.impl += "\t\tbool changed = false;\n"
        code.impl += "\n"
        code.impl += "\t\t" + makeValidCode.replace("\n", "\n\t\t").strip() + "\n\n"
        code.impl += "\t\tif (changed) {\n"
        code.impl += "\t\t\tqWarning(\"ProtoValidator: Changed message \\\"%s\\\" to make it valid - this might have corrupted data...\");\n" % currentMessage.fullName()
        code.impl += "\t\t}\n\n"
        code.impl += "\t\treturn changed;\n"
        code.impl += "\t}\n"
        code.impl += "\n"


    code += "}; // namespace ProtoValidator\n"

    code.header += "\n"
    code.header += "#endif // MUMBLE_PROTOVALIDATOR_H_"

    return code






if __name__ == "__main__":
    if len(sys.argv) != 2:
        printHelp()
        sys.exit(0)

    if sys.argv[1] == "-h" or sys.argv[1] == "--help":
        printHelp()
        sys.exit(0)

    protoFile = sys.argv[1];

    outDir = os.path.dirname(protoFile)

    if outDir == "":
        outDir = "."

    headerOut = outDir + os.path.sep + "ProtoValidator.h"
    implOut = outDir + os.path.sep + "ProtoValidator.cpp"

    # Check that the proto file actually exists
    if not os.path.isfile(protoFile):
        print("[ERROR]: \"%s\" is not a file" % protoFile)
        sys.exit(1)

    # If either of the Output files already exists, make sure that it is likely
    # a file that has been generated by this script before (to not accidentally
    # override other source files)
    if os.path.exists(headerOut):
        with open(headerOut, "r") as content_file:
            if not "automatically generated" in content_file.read():
                print("[ERROR]: \"%s\" does already exist and does not seem to have been generated by this script before!" % headerOut)
                sys.exit(1)

    if os.path.exists(implOut):
        with open(implOut, "r") as content_file:
            if not "automatically generated" in content_file.read():
                print("[ERROR]: \"%s\" does already exist and does not seem to have been generated by this script before!" % implOut)
                sys.exit(1)

    # Parse the proto file
    (packageName, messages) = extractMessages(protoFile)

    # Generate the code from it
    validatorCode = generateValidator(packageName, messages)

    # Write the generated source to the respective files
    with open(headerOut, "w") as content_file:
        content_file.write(validatorCode.header)

    with open(implOut, "w") as content_file:
        content_file.write(validatorCode.impl)
