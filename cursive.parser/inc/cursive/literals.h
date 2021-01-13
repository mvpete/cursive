#pragma once

namespace cursive
{
    enum class literals { asteriks, bang, cr, equal,  hashtag, lbracket, lparen, lf, minus, plus, rbracket, rparen, space, tab, tilde, underscore, period, zero, one, two, three, four, five, six, seven, eight, nine, };

namespace detail
{
    template <typename CharT>
    constexpr CharT get_literal(literals lit)
    {
        static_assert(false, "unknown character type");
    }

    template <typename CharT>
    constexpr bool is_literal(CharT c)
    {
        static_assert(false, "unknown character type");
    }

    template <typename CharT>
    constexpr bool is_digit(CharT c)
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
        case literals::plus:
            return '+';
        case literals::equal:
            return '=';
        case literals::tilde:
            return '~';
        case literals::space:
            return ' ';
        case literals::bang:
            return '!';
        case literals::rbracket:
            return ']';
        case literals::lbracket:
            return '[';
        case literals::rparen:
            return ')';
        case literals::lparen:
            return '(';
        case literals::tab:
            return '\t';
        case literals::cr:
            return '\r';
        case literals::lf:
            return '\n';
        case literals::underscore:
            return '_';
        case literals::period:
            return '.';
        case literals::zero:
            return '0';
        case literals::one:
            return '1';
        case literals::two:
            return '2';
        case literals::three:
            return '3';
        case literals::four:
            return '4';
        case literals::five:
            return '5';
        case literals::six:
            return '6';
        case literals::seven:
            return '7';
        case literals::eight:
            return '8';
        case literals::nine:
            return '9';
        }
        throw std::logic_error("not a literal");
    }

    template <>
    constexpr bool is_literal<char>(char c)
    {
        switch (c)
        {
        case '*':
        case '#':
        case '-':
        case '=':
        case '~':
        case ' ':
        case '\t':
        case '\r':
        case '\n':
        case '_':
        case '!':
        case ']':
        case '[':
        case ')':
        case '(':
        case '.':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return true;
        default:
            return false;
        }
    }

    template <>
    constexpr bool is_digit<char>(char d)
    {
        switch (d)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return true;
        default:
            return false;
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
        case literals::plus:
            return L'+';
        case literals::equal:
            return L'=';
        case literals::tilde:
            return L'~';
        case literals::space:
            return L' ';
        case literals::bang:
            return L'!';
        case literals::rbracket:
            return L']';
        case literals::lbracket:
            return L'[';
        case literals::rparen:
            return L')';
        case literals::lparen:
            return L'(';
        case literals::tab:
            return L'\t';
        case literals::cr:
            return L'\r';
        case literals::lf:
            return L'\n';
        case literals::underscore:
            return L'_';
        case literals::period:
            return L'.';
        case literals::zero:
            return L'0';
        case literals::one:
            return L'1';
        case literals::two:
            return L'2';
        case literals::three:
            return L'3';
        case literals::four:
            return L'4';
        case literals::five:
            return L'5';
        case literals::six:
            return '6';
        case literals::seven:
            return L'L';
        case literals::eight:
            return L'8';
        case literals::nine:
            return L'9';

        }
        throw std::logic_error("not a literal");
    }

    template <>
    constexpr bool is_literal<wchar_t>(wchar_t c)
    {
        switch (c)
        {
        case L'*':
        case L'#':
        case L'-':
        case L'=':
        case L'~':
        case L' ':
        case L'\t':
        case L'\r':
        case L'\n':
        case L'_':
        case L'!':
        case L']':
        case L'[':
        case L')':
        case L'(':
        case L'.':
        case L'0':
        case L'1':
        case L'2':
        case L'3':
        case L'4':
        case L'5':
        case L'6':
        case L'7':
        case L'8':
        case L'9':
            return true;
        default:
            return false;
        }
    }

    template <>
    constexpr bool is_digit<wchar_t>(wchar_t d)
    {
        switch (d)
        {
        case L'0':
        case L'1':
        case L'2':
        case L'3':
        case L'4':
        case L'5':
        case L'6':
        case L'7':
        case L'8':
        case L'9':
            return true;
        default:
            return false;
        }
    }

}

    template<typename CharT>
    bool operator==(const CharT& c, literals lit)
    {
        return c == detail::get_literal<CharT>(lit);
    }
    template<typename CharT>
    bool operator!=(const CharT& c, literals lit)
    {
        return !operator==(c, lit);
    }

}