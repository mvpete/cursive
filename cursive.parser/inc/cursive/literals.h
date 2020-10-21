#pragma once

namespace cursive
{
namespace detail
{

    enum class literals { hashtag, minus, underscore, equal, tilde, space, tab, cr, nl, asteriks };

    template <typename CharT>
    constexpr CharT get_literal(literals lit)
    {
        static_assert(false, "unknown character type");
    }

    template <>
    constexpr char get_literal<char>(literals lit)
    {
        switch (lit)
        {
        case literals::asteriks:
            return '*';
        case literals::hashtag:
            return '#';
        case literals::minus:
            return '-';
        case literals::equal:
            return '=';
        case literals::tilde:
            return '~';
        case literals::space:
            return ' ';
        case literals::tab:
            return '\t';
        case literals::cr:
            return '\r';
        case literals::nl:
            return '\n';
        case literals::underscore:
            return '_';
           
        }
    }

    template <>
    constexpr wchar_t get_literal<wchar_t>(literals lit)
    {
        switch (lit)
        {
        case literals::asteriks:
            return L'*';
        case literals::hashtag:
            return L'#';
        case literals::minus:
            return L'-';
        case literals::equal:
            return L'=';
        case literals::tilde:
            return L'~';
        case literals::space:
            return L' ';
        case literals::tab:
            return L'\t';
        case literals::cr:
            return L'\r';
        case literals::nl:
            return L'\n';
        case literals::underscore:
            return L'_';

        }
    }
}
}