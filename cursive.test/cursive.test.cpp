#include "pch.h"
#include "CppUnitTest.h"
#include "Check.h"
#include "ToString.h"

#include <cursive/cursive.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace cursive;

namespace cursivetest
{
	TEST_CLASS(test_cursive_parser)
	{
	public:

		TEST_METHOD(test_parser_basic_header)
		{
			std::string text("# Hello world");
			auto doc = cursive::parse(text);

			Check::HasChildren(1, doc);
			auto element = doc.child_at(0);
			Check::IsType(cursive::element_types::heading, element);
			Check::HasChildren(1, element);
			auto child = element.children()[0];
			Check::HasValue("Hello world", child);
		}

		TEST_METHOD(test_parser_basic_header_wide)
		{
			std::wstring text(L"# Hello world");
			auto doc = cursive::parse(text);

			Check::HasChildren(1, doc);
			auto element = doc.child_at(0);
			Check::IsType(cursive::element_types::heading, element);
			Check::HasChildren(1, element);
			auto child = element.children()[0];
			Check::HasValue(L"Hello world", child);
		}
			
		TEST_METHOD(test_parser_basic_header_eol)
		{
			std::string text("# Hello world\r\n");
			auto doc = cursive::parse(text);

			Check::HasChildren(1, doc);
			auto element = doc.child_at(0);
			Check::IsType(element_types::heading, element);
			Check::HasChildren(1, element);
			auto child = element.child_at(0);
			Assert::AreEqual(std::string("Hello world"), child.value());
		}

		TEST_METHOD(test_parser_basic_header_inline_emp)
		{
			std::string text("# _Hello_ world\r\n");
			auto doc = cursive::parse(text);

			Check::HasChildren(1, doc);
			auto element = doc.child_at(0);
			Check::HasChildren(2, element);
			auto emp = element.child_at(0);
			Check::IsType(element_types::emphasis, emp);
			Check::HasChildren(1, emp);
			auto child = emp.child_at(0);
			Check::HasValue("Hello", child);
		}

		TEST_METHOD(test_parser_basic_header_inline_emp_imbalance_left)
		{
			std::string text("# __Hello_ world\r\n");
			auto doc = cursive::parse(text);

			Check::HasChildren(1, doc);
			auto element = doc.child_at(0);
			Check::HasChildren(3, element);
			
			auto txt = element.children()[0];
			Check::IsType(element_types::plain_text, txt);
			Check::HasValue("_", txt);

			auto emp = element.child_at(1);
			Check::IsType(element_types::emphasis, emp);
			Check::HasChildren(1, emp);
			
			auto child = emp.child_at(0);
			Check::HasValue("Hello", child);
		}

		TEST_METHOD(test_parser_basic_header_inline_emp_imbalance_right)
		{
			std::string text("# _Hello__ world\r\n");
			auto doc = cursive::parse(text);

			Check::HasChildren(1, doc);
			auto element = doc.child_at(0);
			Check::HasChildren(2, element);

			auto emp = element.child_at(0);
			Check::IsType(element_types::emphasis, emp);
			Check::HasChildren(1, emp);
			auto child = emp.child_at(0);
			Check::HasValue("Hello", child);

			auto txt = element.child_at(1);
			Check::IsType(element_types::plain_text, txt);
			Check::HasValue("_ world", txt);
		}

		TEST_METHOD(test_parser_basic_header_inline_emp_strike)
		{
			std::string text("# _~Hello~_ world\r\n");
			auto doc = cursive::parse(text);

			Check::HasChildren(1, doc);
			auto element = doc.child_at(0);
			Check::HasChildren(2, element);

			auto emp = element.child_at(0);
			Check::IsType(element_types::emphasis, emp);
			Check::HasChildren(1, emp);

			auto strike = emp.child_at(0);
			Check::IsType(element_types::strike, strike);
			Check::HasChildren(1, strike);
			
			auto strike_val = strike.child_at(0);
			Check::HasValue("Hello", strike_val);
		}

		TEST_METHOD(test_parser_basic_header_inline_header_character)
		{
			std::string text("# Hello ### world\r\n");
			auto doc = cursive::parse(text);

			Check::HasChildren(1, doc);
			auto element = doc.child_at(0);
			Check::IsType(element_types::heading, element);
			Check::HasChildren(1, element);

			auto child = element.child_at(0);
			Check::HasValue("Hello ### world", child);
		}

	};
}
