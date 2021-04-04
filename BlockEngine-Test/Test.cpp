#include "pch.h"
#include "CppUnitTest.h"

#include "Files.h"
#include "Files.cpp" // https://stackoverflow.com/questions/47515089/lnk2019-unresolved-external-symbol-in-vs-unit-testing

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Test
{
	TEST_CLASS(FILE)
	{
	public:
		
		TEST_METHOD(READ)
		{
			std::string fileContent = Files::readFile("test.txt");
			std::string expected = "Hello\nWorld\n";
			Assert::AreEqual(expected, fileContent);
		}
	};
}
