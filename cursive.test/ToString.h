#pragma once

#include <cursive/cursive.h>
#include <CppUnitTestAssert.h>

namespace Microsoft
{
namespace VisualStudio
{
namespace CppUnitTestFramework
{
    template<> inline std::wstring ToString<cursive::element_types>(const cursive::element_types& t) 
    { 
        using namespace cursive;
        std::wstringstream ws;
        switch (t)
        {
        case element_types::none:
            ws << "none";
            break;
        case element_types::heading:
            ws << "heading";
            break;
        case element_types::paragraph:
            ws << "paragraph";
            break;
        case element_types::ordered_list:
            ws << "ordered_list";
            break;
        case element_types::unordered_list:
            ws << "unordered_list";
            break;
        case element_types::plain_text:
            ws << "plain_text";
            break;
        case element_types::emphasis:
            ws << "emphasis";
            break;
        case element_types::strong:
            ws << "strong";
            break;
        case element_types::strike:
            ws << "strike";
            break;
        case element_types::container:
            ws << "container";
            break;
        default:
            ws << "unknown";
            break;
        }
        ws << "(" << int(t) <<")";
        return ws.str();
    }

}
}
}