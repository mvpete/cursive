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

		TEST_METHOD(test_parser_unordered_list)
		{
			std::string text("- Item 1\r\n- Item 2\r\n- Item 3\r\n- Item 4");

			auto doc = cursive::parse(text);
			Check::HasChildren(1, doc);

			auto list = doc.child_at(0);
			Check::IsType(element_types::unordered_list, list);

			Check::HasChildren(4, list);
			for (int i = 0; i < 4; ++i)
			{
				std::stringstream ss;
				ss << "Item " << i + 1;
				auto &item = list.child_at(i);
				Check::HasChildren(1, item);

				auto &val = item.child_at(0);
				Check::HasValueString(ss.str(), val);
			}

		}

		TEST_METHOD(test_parser_unordered_list_single_item)
		{
			std::string text("- Item 1\r\n");

			auto doc = cursive::parse(text);
			Check::HasChildren(1, doc);

			auto &list = doc.child_at(0);
			Check::IsType(element_types::unordered_list, list);

			auto &item = list.child_at(0);
			Check::HasChildren(1, item);

			auto &val = item.child_at(0);
			Check::HasValue("Item 1", val);

		}

		TEST_METHOD(test_parser_unordered_list_single_item_no_crlf)
		{
			std::string text("- Item 1");

			auto doc = cursive::parse(text);
			Check::HasChildren(1, doc);

			auto& list = doc.child_at(0);
			Check::IsType(element_types::unordered_list, list);

			auto& item = list.child_at(0);
			Check::HasChildren(1, item);

			auto& val = item.child_at(0);
			Check::HasValue("Item 1", val);

		}

		TEST_METHOD(test_parser_unordered_list_token_in_text)
		{
			std::string text("- It -m 1\r\n- It ---m 2\r\n");

			auto doc = cursive::parse(text);
			Check::HasChildren(1, doc);

			auto& list = doc.child_at(0);
			Check::IsType(element_types::unordered_list, list);
			Check::HasChildren(2, list);

			auto& item = list.child_at(0);
			Check::HasChildren(1, item);

			auto& val = item.child_at(0);
			Check::HasValue("It -m 1", val);

			auto& item2 = list.child_at(1);
			Check::HasChildren(1, item);

			auto& val2 = item2.child_at(0);
			Check::HasValue("It -m 1", val);

		}

		TEST_METHOD(test_parser_unordered_list_with_header)
		{
			std::string text("- # Item 1");

			auto doc = cursive::parse(text);
			Check::HasChildren(1, doc);

			auto& list = doc.child_at(0);
			Check::IsType(element_types::unordered_list, list);

			auto& item = list.child_at(0);
			Check::HasChildren(1, item);

			auto& val = item.child_at(0);

			Check::IsType(cursive::element_types::heading, val);
			Check::HasChildren(1, val);
			auto child = val.children()[0];
			Check::HasValue("Item 1", child);
		}

		TEST_METHOD(test_parser_unordered_list_in_header)
		{
			std::string text("# -Hello world");
			auto doc = cursive::parse(text);

			Check::HasChildren(1, doc);
			auto element = doc.child_at(0);
			Check::IsType(cursive::element_types::heading, element);
			Check::HasChildren(1, element);
			auto child = element.children()[0];
			Check::HasValue("-Hello world", child);
		}

		TEST_METHOD(test_parser_hr_in_header)
		{
			std::string text("# ---Hello world");
			auto doc = cursive::parse(text);

			Check::HasChildren(1, doc);
			auto element = doc.child_at(0);
			Check::IsType(cursive::element_types::heading, element);
			Check::HasChildren(1, element);
			auto child = element.children()[0];
			Check::HasValue("---Hello world", child);
		}

		TEST_METHOD(test_parser_image_link)
		{
			std::string text("![image](path/to/image)");

			auto doc = cursive::parse(text);
			Check::HasChildren(1, doc);

			auto image = doc[0];
			Check::IsType(element_types::image, image);

			Check::HasValue("image", image);
			Check::HasData("path/to/image", image);
		}

		TEST_METHOD(test_parser_link)
		{
			std::string text("[link](path/to)");

			auto doc = cursive::parse(text);
			Check::HasChildren(1, doc);

			auto link = doc[0];
			Check::IsType(element_types::link, link);

			Check::HasValue("link", link);
			Check::HasData("path/to", link);
		}

		TEST_METHOD(test_parser_link_incomplete)
		{
			std::string source("[link](path/to");

			auto doc = cursive::parse(source);
			Check::HasChildren(1, doc);

			auto link = doc[0];
			Check::IsType(element_types::paragraph, link);

			Check::HasChildren(1, link);
			auto text = link[0];

			Check::HasValue("[link](path/to", text);
		}

		TEST_METHOD(test_parser_link_incorrect)
		{
			std::string source("[link(path/to)");

			auto doc = cursive::parse(source);
			Check::HasChildren(1, doc);

			auto link = doc[0];
			Check::IsType(element_types::paragraph, link);

			Check::HasChildren(1, link);
			auto text = link[0];

			Check::HasValue("[link(path/to)", text);
		}

		TEST_METHOD(test_parser_link_incomplete_crlf)
		{
			std::string source("[link](path/to\r\n)");

			auto doc = cursive::parse(source);
			Check::HasChildren(1, doc);

			auto link = doc[0];
			Check::IsType(element_types::paragraph, link);

			Check::HasChildren(1, link);
			auto text = link[0];


			Check::HasValue("[link](path/to)", text);
		}

		TEST_METHOD(test_parser_simple_md)
		{
			std::string source("# Hello MD\r\n\
\r\n\
- This is an unordered list\r\n\
- Another list item\r\n\
\r\n\
1. An ordered list\r\n\
2. Second item\r\n\
\r\n\
Nothing complicated\r\n");

			auto doc = cursive::parse(source);


		}

	};
}
