import unittest
from unittest import TestCase
import fast_stringcase as stringcase


class TestFastStringcase(TestCase):

    def test_camelcase(self):
        self.assertEqual(stringcase.camelcase("foo_bar_baz"), "fooBarBaz")
        self.assertEqual(stringcase.camelcase("FooBarBaz"), "fooBarBaz")

    def test_capitalcase(self):
        self.assertEqual(stringcase.capitalcase("foo_bar_baz"), "Foo_bar_baz")
        self.assertEqual(stringcase.capitalcase("FooBarBaz"), "FooBarBaz")

    def test_constcase(self):
        self.assertEqual(stringcase.constcase("foo_bar_baz"), "FOO_BAR_BAZ")
        self.assertEqual(stringcase.constcase("FooBarBaz"), "FOO_BAR_BAZ")

    def test_lowercase(self):
        self.assertEqual(stringcase.lowercase("foo_bar_baz"), "foo_bar_baz")
        self.assertEqual(stringcase.lowercase("FooBarBaz"), "foobarbaz")

    def test_pascalcase(self):
        self.assertEqual(stringcase.pascalcase("foo_bar_baz"), "FooBarBaz")
        self.assertEqual(stringcase.pascalcase("FooBarBaz"), "FooBarBaz")

    def test_pathcase(self):
        self.assertEqual(stringcase.pathcase("foo_bar_baz"), "foo/bar/baz")
        self.assertEqual(stringcase.pathcase("FooBarBaz"), "foo/bar/baz")

    def test_sentencecase(self):
        self.assertEqual(stringcase.sentencecase("foo_bar_baz"), "Foo bar baz")
        self.assertEqual(stringcase.sentencecase("FooBarBaz"), "Foo bar baz")

    def test_snakecase(self):
        self.assertEqual(stringcase.snakecase("foo_bar_baz"), "foo_bar_baz")
        self.assertEqual(stringcase.snakecase("FooBarBaz"), "foo_bar_baz")

    def test_spinalcase(self):
        self.assertEqual(stringcase.spinalcase("foo_bar_baz"), "foo-bar-baz")
        self.assertEqual(stringcase.spinalcase("FooBarBaz"), "foo-bar-baz")

    def test_titlecase(self):
        self.assertEqual(stringcase.titlecase("foo_bar_baz"), "Foo Bar Baz")
        self.assertEqual(stringcase.titlecase("FooBarBaz"), "Foo Bar Baz")

    def test_trimcase(self):
        self.assertEqual(stringcase.trimcase("foo_bar_baz"), "foo_bar_baz")
        self.assertEqual(stringcase.trimcase("FooBarBaz"), "FooBarBaz")

    def test_uppercase(self):
        self.assertEqual(stringcase.uppercase("foo_bar_baz"), "FOO_BAR_BAZ")
        self.assertEqual(stringcase.uppercase("FooBarBaz"), "FOOBARBAZ")

    def test_alphanumcase(self):
        self.assertEqual(stringcase.alphanumcase("_Foo., Bar"), "FooBar")
        self.assertEqual(stringcase.alphanumcase("Foo_123 Bar!"), "Foo123Bar")


if __name__ == "__main__":
    unittest.main()
