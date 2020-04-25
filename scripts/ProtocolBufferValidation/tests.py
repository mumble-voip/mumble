#!/usr/bin/env python3

# Copyright 2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.


import unittest

from MessageField import MessageField 
from ProtoMessage import ProtoMessage, ProtoEnum
from ProtoFile import ProtoFile
from FieldAnnotation import RestrictAnnotation

class TestMessageField(unittest.TestCase):
    def test_equal(self):
        self.assertEqual(MessageField("test", "string", None, 3), MessageField("test", "string", None, 3))
        self.assertEqual(MessageField("test", "string", "repeated", 3), MessageField("test", "string", "repeated", 3))

    def test_unequal(self):
        self.assertNotEqual (MessageField("Test", "string", None, 3), MessageField("test", "string", None, 3))
        self.assertNotEqual (MessageField("test", "String", None, 3), MessageField("test", "string", None, 3))
        self.assertNotEqual(MessageField("test", "string", "repeated", 3), MessageField("test", "string", None, 3))
        self.assertNotEqual(MessageField("test", "string", "repeated", 3), MessageField("test", "string", "optional", 3))
        self.assertNotEqual(MessageField("test", "string", None, 5), MessageField("test", "string", None, 3))

    def test_parseWithoutRule(self):
        fieldDef1 = "string myField = 5;"
        fieldDef2 = "   string     \t myField = 5  ;"
        fieldDef3 = "string myField=5;"
        fieldDef3 = "string myField = 5 [packed = true];"

        result = MessageField(fieldName="myField", fieldType="string", fieldRule=None, fieldNumber=5)

        self.assertEqual(MessageField.parse(fieldDef1), result)
        self.assertEqual(MessageField.parse(fieldDef2), result)
        self.assertEqual(MessageField.parse(fieldDef3), result)

    def test_parseWihtRule(self):
        fieldDef1 = "optional string myField = 5;"
        fieldDef2 = "  optional   \tstring      myField = 5  ;"
        fieldDef3 = "optional string myField=5;"
        fieldDef3 = "optional string myField = 5 [packed = true];"

        result = MessageField(fieldName="myField", fieldType="string", fieldRule="optional", fieldNumber=5)

        self.assertEqual(MessageField.parse(fieldDef1), result)
        self.assertEqual(MessageField.parse(fieldDef2), result)
        self.assertEqual(MessageField.parse(fieldDef3), result)


class TestProtoMessage(unittest.TestCase):
    def test_equal(self):
        self.assertEqual(ProtoMessage(name="TestMsg"), ProtoMessage(name="TestMsg"))

        msg1 = ProtoMessage(name="msg")
        msg2 = ProtoMessage(name="msg")
        field = MessageField(fieldName="myField", fieldType="string", fieldRule="optional", fieldNumber=5)
        msg1.messageFields.append(field)
        msg2.messageFields.append(field)
        self.assertEqual(msg1, msg2)

    def test_unequal(self):
        self.assertNotEqual(ProtoMessage(name="TestMsg"), ProtoMessage(name="OtherTestMsg"))

        msg1 = ProtoMessage(name="msg")
        msg2 = ProtoMessage(name="OtherMsg")
        msg3 = ProtoMessage(name="msg")
        field = MessageField(fieldName="myField", fieldType="string", fieldRule="optional", fieldNumber=5)
        otherField = MessageField(fieldName="otherField", fieldType="string", fieldRule="optional", fieldNumber=5)
        msg1.messageFields.append(field)
        msg2.messageFields.append(field)
        msg3.messageFields.append(otherField)

        self.assertNotEqual(msg1, msg2)
        self.assertNotEqual(msg1, msg3)
        self.assertNotEqual(msg2, msg3)


    def test_parse(self):
        msgDef1 = "message myMsg {}"
        result1 = ProtoMessage(name="myMsg")
        self.assertEqual(ProtoMessage.parse(msgDef1), result1)

        msgDef2 = "message myMsg { string test = 5; }"
        result2 = ProtoMessage(name="myMsg")
        result2.messageFields.append(MessageField(fieldName="test", fieldType="string", fieldRule=None, fieldNumber=5))
        self.assertEqual(ProtoMessage.parse(msgDef2), result2)

        msgDef3 = "message myMsg { string test = 5;\n optional int foo=7; }"
        result3 = ProtoMessage(name="myMsg")
        result3.messageFields.append(MessageField(fieldName="test", fieldType="string", fieldRule=None, fieldNumber=5))
        result3.messageFields.append(MessageField(fieldName="foo", fieldType="int", fieldRule="optional", fieldNumber=7))
        self.assertEqual(ProtoMessage.parse(msgDef3), result3)

    def test_nestedParse(self):
        def1 = "message a { message b { int foo = 1; } string bar = 3; enum test {} }"

        b = ProtoMessage(name="b")
        a = ProtoMessage("a")
        foo = MessageField(fieldName="foo", fieldType="int", fieldRule=None, fieldNumber=1)
        bar = MessageField(fieldName="bar", fieldType="string", fieldRule=None, fieldNumber=3)
        enum = ProtoEnum(name="test")

        b.messageFields.append(foo)
        a.messageFields.append(bar)
        a.nestedMessages.append(b)
        a.nestedEnums.append(enum)

        self.assertEqual(ProtoMessage.parse(def1), a)


    def test_annotations(self):
        def1 = "message test {\n @restrict size <= 20\nstring test = 0;}"

        msg = ProtoMessage(name="test")
        field = MessageField(fieldName="test", fieldType="string", fieldRule=None, fieldNumber=0)
        annotation = RestrictAnnotation(prop="size", comp="<=", val="20")
        field.fieldAnnotations.append(annotation)
        msg.messageFields.append(field)

        result = ProtoMessage.parse(def1)

        self.assertEqual(result, msg)



class TestProtoFile(unittest.TestCase):
    def test_equal(self):
        self.assertEqual(ProtoFile(name="test", protoVersion=2, package="Test"), ProtoFile(name="test", protoVersion=2, package="Test"))
        self.assertEqual(ProtoFile(name="test", protoVersion=None, package="Test"), ProtoFile(name="test", protoVersion=None, package="Test"))

        msg = ProtoMessage(name="test")
        file1 = ProtoFile(name="test", protoVersion=3, package=None)
        file2 = ProtoFile(name="test", protoVersion=3, package=None)

        file1.messages.append(msg)
        file2.messages.append(msg)

        self.assertEqual(file1, file2)


    def test_unequal(self):
        self.assertNotEqual(ProtoFile(name="test", protoVersion=2, package="Test"), ProtoFile(name="Test", protoVersion=2, package="Test"))
        self.assertNotEqual(ProtoFile(name="test", protoVersion=None, package="Test"), ProtoFile(name="test", protoVersion=2, package="Test"))

        msg1 = ProtoMessage(name="test")
        msg2 = ProtoMessage(name="otherTest")
        file1 = ProtoFile(name="test", protoVersion=3, package=None)
        file2 = ProtoFile(name="test", protoVersion=3, package=None)

        file1.messages.append(msg1)

        self.assertNotEqual(file1, file2)

        file2.messages.append(msg2)

        self.assertNotEqual(file1, file2)


    def test_parse(self):
        def1 = """// This is a comment
        syntax=\"proto3\";
        // another comment @here
        package TestPkg;

        option someOption = something random { test? };

        message Foo {
            // This is a comment right here
            // on more tnat one line
            required int foo = 5;
        }

        message test {
            message nested{
                // this is weirdly formatted
                string bar = 1;}

            repeated nested field = 1;
        }
        """

        result = ProtoFile(name="Test.proto", protoVersion=3, package="TestPkg")
        FooMsg = ProtoMessage(name="Foo")
        fooField = MessageField(fieldName="foo", fieldType="int", fieldRule="required", fieldNumber=5)
        testMsg = ProtoMessage(name="test")
        nestedMsg = ProtoMessage(name="nested")
        barField = MessageField(fieldName="bar", fieldType="string", fieldRule=None, fieldNumber=1)
        fieldField = MessageField(fieldName="field", fieldType="nested", fieldRule="repeated", fieldNumber=1)

        nestedMsg.messageFields.append(barField)
        testMsg.messageFields.append(fieldField)
        testMsg.nestedMessages.append(nestedMsg)

        FooMsg.messageFields.append(fooField)

        result.messages.append(FooMsg)
        result.messages.append(testMsg)

        self.assertEqual(ProtoFile.parse(def1, "Test.proto"), result)


class TestAnnotations(unittest.TestCase):
    def test_restrictAnnotation(self):
        an1 = RestrictAnnotation(prop="size", comp="<", val="10")

        field1 = MessageField(fieldName="test", fieldType="string", fieldRule=None, fieldNumber=1)
        field2 = MessageField(fieldName="test", fieldType="int", fieldRule="repeated", fieldNumber=1)

        self.assertEqual(an1.generateValidCheck(field1), "msg.test().size() < 10")
        self.assertEqual(an1.generateValidCheck(field2), "msg.test_size() < 10")

        an2 = RestrictAnnotation(prop="value", comp="<", val="10")

        field3 = MessageField(fieldName="test", fieldType="int", fieldRule=None, fieldNumber=1)
        field4 = MessageField(fieldName="test", fieldType="int", fieldRule="optional", fieldNumber=1)

        self.assertEqual(an2.generateValidCheck(field3), "msg.test() < 10")
        self.assertEqual(an2.generateValidCheck(field4), "(msg.has_test() && msg.test() < 10)")



if __name__ == "__main__":
    unittest.main()
