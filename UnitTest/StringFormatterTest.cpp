#include "stdafx.h"
#include "CppUnitTest.h"

#include "StringFormatter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Handmada;


namespace UnitTest
{		
	TEST_CLASS(StringFormatterTest)
	{
	public:
		
		TEST_METHOD(FunctionalityTest)
		{
            // Should work on empty pattern without arguments
            auto actual = ""_sf.format();
            Assert::AreEqual("", actual.c_str());

            // Should work on pattern without {} and no arguments
            actual = "sample text"_sf.format();
            Assert::AreEqual("sample text", actual.c_str());

            // Should NOT work on pattern with { and no arguments
            // Note: { is considered a typo
            auto test0 = []() {
                return "number { is big"_sf.format();
            };
            Assert::ExpectException<std::exception>(test0);

            // Should NOT work on pattern with {} and no arguments
            auto test1 = []() {
                return "number {} is big"_sf.format();
            };
            Assert::ExpectException<std::exception>(test1);

            // Should NOT work on pattern without {} and non-empty argument list
            auto test2 = []() {
                return "number is big"_sf.format(137);
            };
            Assert::ExpectException<std::exception>(test2);

            // Should work
            actual = "number {} is big"_sf.format(137);
            Assert::AreEqual("number 137 is big", actual.c_str());

            // Should NOT work
            // Note: {}'s more than arguments
            auto test3 = []() {
                return "{} is bigger than {}"_sf.format(137);
            };
            Assert::ExpectException<std::exception>(test3);

            // Should NOT work
            // Note: {}'s less than arguments
            auto test4 = []() {
                return "{} is bigger than "_sf.format(137, 42);
            };
            Assert::ExpectException<std::exception>(test4);

            // Should work for homogeneous types
            actual = "{} is bigger than {}"_sf.format(137, 42);
            Assert::AreEqual("137 is bigger than 42", actual.c_str());

            // Should work for heterogeneous types
            actual = "({}, {}, {})"_sf.format(137, 'A', "sample text");
            Assert::AreEqual("(137, A, sample text)", actual.c_str());

            // Should NOT work when braces are not closed
            auto test5 = []() {
                return "{} is bigger than {"_sf.format(137, 42);
            };
            Assert::ExpectException<std::exception>(test5);
		}


        TEST_METHOD(OptionsTest)
        {
            // Should work for hex
            auto actual = "{} dec is 0x{:x} hex"_sf.format(32, 32);
            Assert::AreEqual("32 dec is 0x20 hex", actual.c_str());

            // Should NOT work for :x and non-integral type
            auto test0 = []() {
                return "{} dec is 0x{:x} hex"_sf.format(32, "sample text");
            };
            Assert::ExpectException<std::exception>(test0);

            // Should NOT work for unknown options
            auto testN = []() {
                return "what the duck {:z} is?"_sf.format("sample text");
            };
            Assert::ExpectException<std::exception>(testN);
        }

	};
}