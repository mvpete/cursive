#pragma once

#include <CppUnitTestAssert.h>
#include <cursive/cursive.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

class Check
{
public:
    template <typename CharT>
    static void IsType(cursive::element_types expected, const cursive::basic_element<CharT>& element)
    {
        Assert::AreEqual(expected, element.type());
    }

    template <typename CharT>
    static void HasChildren(size_t size, const cursive::basic_element<CharT>& element)
    {
        Assert::AreEqual(size, element.children().size());
    }

    template <typename CharT>
    static void HasChildren(size_t size, const cursive::basic_document<CharT>& element)
    {
        Assert::AreEqual(size, element.children().size());
    }

    template <typename CharT>
    static void HasValue(const CharT *expected, const cursive::basic_element<CharT>& element)
    {
        Assert::AreEqual(std::basic_string<CharT>(expected), element.value());
    }

    template <typename CharT>
    static void HasData(const CharT* expected, const cursive::basic_element<CharT>& element)
    {
        Assert::AreEqual(std::basic_string<CharT>(expected), element.data());
    }

    template <typename CharT>
    static void HasValueString(const std::basic_string<CharT> &expected, const cursive::basic_element<CharT>& element)
    {
        Assert::AreEqual(expected, element.value());
    }
};