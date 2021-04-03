#ifndef __cursive_tokens_h__
#define __cursive_tokens_h__

#include <string_view>
#include <cursive/loc.h>
#include <cursive/literals.h>

#include <ctype.h>

namespace cursive
{

    enum class tokens
    {
        empty =0,
        header,
        emphasis,
        strikethrough,
        unordered_list,
        horizontal_rule,
        text,
        space,
        post_header,
        ordered_list,
        rbrack,
        lbrack,
        ibrack,
        link,
        image,
        ref_text,
        block_quote,
        backtick,
        eol,
        eof
    };

    template <typename CharT>
    using const_iterator = typename std::basic_string_view<CharT>::const_iterator;

    template <typename CharT>
    class basic_token
    {
        
    public:

        using string_view = std::basic_string_view<CharT>;

        basic_token(tokens code)
            :code_(code)
        {
        }

        basic_token(tokens code, const_iterator<CharT> begin, const_iterator<CharT> end)
            :code_(code), text_(&(*begin), end-begin)
        {
        }

        basic_token(tokens code, const string_view& text, const string_view& data)
            :code_(code), text_(text), data_(data)
        {
        }

        basic_token(const basic_token& rhs)
            :code_(rhs.code_), text_(rhs.text_), data_(rhs.data_)
        {

        }

        basic_token& operator=(const basic_token& rhs)
        {
            code_ = rhs.code_;
            text_ = rhs.text_;
            data_ = rhs.data_;
            return *this;
        }

        basic_token& operator=(basic_token&& rhs) noexcept
        {
            std::swap(code_, rhs.code_);
            std::swap(text_, rhs.text_);
            std::swap(data_, rhs.data_);
            return *this;
        }

        basic_token(basic_token&& rhs) noexcept
            :code_(tokens::empty)
        {
            std::swap(code_, rhs.code_);
            std::swap(text_, rhs.text_);
            std::swap(data_, rhs.data_);
        }
        tokens code() const 
        {
            return code_;
        }
        size_t count() const
        {
            return text_.length();
        }
        const std::basic_string_view<CharT>& text() const
        {
            return text_;
        }

        const std::basic_string_view<CharT>& data() const
        {
            return data_;
        }

        loc loc() const
        {
            return cursive::loc{ 1,1 };
        }

        bool operator==(tokens code) const
        {
            return code_ == code;
        }
        bool operator!=(tokens code) const
        {
            return !operator==(code);
        }

        basic_token split(size_t index)
        {
            auto b = text_.begin();
            auto e = text_.begin() + index;
            text_.remove_prefix(index);
            return basic_token(code_, b,e);
        }

    private:
        tokens code_;
        std::basic_string_view<CharT> text_;
        std::basic_string_view<CharT> data_;
    };


    template<typename CharT>
    class basic_tokenizer
    {
    public:
        using char_t = CharT;
        using token_t = basic_token<char_t>;
        using const_iterator = const_iterator<char_t>;

    public:
        basic_tokenizer(const_iterator begin, const_iterator end)
            :begin_(begin),end_(end),where_(begin)
        {
        }

        basic_token<char_t> next()
        {
            if (where_ == end_)
                return token_t(tokens::eof);

            // peek to know where to go
            char_t t = *where_;
            switch (t)
            {
            case detail::get_literal<char_t>(literals::hashtag):
                return scan_header_token();

            case detail::get_literal<char_t>(literals::backtick):
                return scan_backtick_token();

            case detail::get_literal<char_t>(literals::minus) :
            case detail::get_literal<char_t>(literals::plus) :
                return scan_dash_token();

            case detail::get_literal<char_t>(literals::equal):
                return scan_post_header_token();

            case detail::get_literal<char_t>(literals::asteriks):
            case detail::get_literal<char_t>(literals::underscore):
                return scan_emphasis_token();

            case detail::get_literal<char_t>(literals::tilde):
                return scan_strike_token();

            case detail::get_literal<char_t>(literals::space):
            case detail::get_literal<char_t>(literals::tab):
                return scan_whitespace_token();

            case detail::get_literal<char_t>(literals::cr):
            case detail::get_literal<char_t>(literals::lf):
                return scan_eol_token();

            case detail::get_literal<char_t>(literals::bang):
                return scan_image_link();

            case detail::get_literal<char_t>(literals::lbracket):
                return scan_link(tokens::link);

            case detail::get_literal<char_t>(literals::zero) :
            case detail::get_literal<char_t>(literals::one) :
            case detail::get_literal<char_t>(literals::two) :
            case detail::get_literal<char_t>(literals::three) :
            case detail::get_literal<char_t>(literals::four) :
            case detail::get_literal<char_t>(literals::five) :
            case detail::get_literal<char_t>(literals::six) :
            case detail::get_literal<char_t>(literals::seven) :
            case detail::get_literal<char_t>(literals::eight) :
            case detail::get_literal<char_t>(literals::nine) :
                return scan_numeric_token();

            default:
                return scan_text_token();
            }
        }

        loc loc() const
        {
            return cursive::loc{ 1, 1 };
        }

        void reset()
        {
            where_ = begin_;
        }

    private:

        token_t scan_backtick_token()
        {
            auto begin = where_;
            while (where_ != end_ && *where_ == literals::backtick) ++where_;
            return token_t(tokens::backtick, begin, where_);
        }

        token_t scan_image_link()
        {
            if (where_ + 1 == end_ || *(where_ + 1) != literals::lbracket)
            {
                ++where_;
                return scan_text_token();
            }
            ++where_;
            return scan_link(tokens::image);
        }

        token_t scan_link(tokens type)
        {
            auto begin = where_;
            if (!scan_inline_until(literals::rbracket) || *(where_ + 1) != literals::lparen)
                return token_t(tokens::text, begin, where_);
            auto text = make_string_view<char_t>(begin + 1, where_);
            auto lbegin = where_;
            if (!scan_inline_until(literals::rparen))
                return token_t(tokens::text, begin, where_);
            auto link = make_string_view<char_t>(lbegin + 2, where_);
            ++where_;
            return token_t(type, text, link);
        }

        token_t scan_header_token()
        {
            auto begin = where_;
            while (where_ != end_ && *where_ == literals::hashtag) ++where_;
        
            if (*where_ != literals::space)
                return token_t(tokens::text, begin, where_);

            ++where_;
            return token_t(tokens::header, begin, where_);
        }

        token_t scan_post_header_token()
        {
            auto begin = where_;
            while (where_ != end_ && *where_ == literals::equal) ++where_;
            return token_t(tokens::post_header, begin, where_);
        }

        token_t scan_text_token()
        {
            auto begin = where_;
            while (where_ != end_ && is_text(*where_)) ++where_;
            return token_t(tokens::text, begin, where_);
        }

        token_t scan_whitespace_token()
        {
            auto begin = where_;
            while (where_ != end_ && *where_ == literals::space || *where_ == literals::tab)
            {
                ++where_;
            }
            return token_t(tokens::space, begin, where_);
        }

        token_t scan_emphasis_token()
        {
            auto begin = where_;
            while (where_ != end_ && *where_ == literals::underscore || *where_ == literals::asteriks)
            {
                ++where_;
            }
            auto len = where_ - begin;
            if (len == 1 && where_ + 1 != end_ && *where_ == literals::space)
            {
                auto end = where_;
                ++where_;
                return token_t(tokens::unordered_list, begin, end);
            }
            return token_t(tokens::emphasis, begin, where_);
        }

        token_t scan_strike_token()
        {
            auto begin = where_;
            while (where_ != end_ && *where_ == literals::tilde)
            {
                ++where_;
            }
            return token_t(tokens::strikethrough, begin, where_);
        }

        token_t scan_eol_token()
        {
            auto begin = where_;
            if (*where_ == literals::cr &&
                 where_ + 1 != end_     && 
                *(where_ + 1) == literals::lf)
            {
                where_ += 2;
            }
            else
            {
                ++where_;
            }
            return token_t(tokens::eol);

        }

        bool scan_inline_until(literals lit)
        {
            while (where_ != end_ && *where_ != lit && *where_ != literals::cr && *where_ != literals::lf) ++where_;
            if (where_ == end_)
                return false;
            return *where_ == lit;
        }

        token_t scan_dash_token()
        {
            auto begin = where_;
            while (where_ != end_ && (*where_ == literals::minus || *where_ == literals::plus)) ++where_;
            auto len = where_ - begin;
            if (len == 1 && where_ + 1 != end_ && *where_ == literals::space)
            {
                auto end = where_;
                ++where_;
                return token_t(tokens::unordered_list, begin, end);
            }
            else if (len > 2)
                return token_t(tokens::horizontal_rule, begin, where_);
            else
                return token_t(tokens::text, begin, where_);
        }

        token_t scan_numeric_token()
        {
            auto begin = where_;
            while (where_ != end_ && detail::is_digit(*where_)) ++where_;
            if (where_ != end_ && *where_ == literals::period)
            {
                ++where_;
                return token_t(tokens::ordered_list, begin, where_);
            }
            else
                return token_t(tokens::text, begin, where_);
        }

        bool is_text(char_t c)
        {
            if (c == literals::rparen || c == literals::lparen || c == literals::rbracket || c == literals::period)
                return true;
            return !detail::is_literal(c);
        }
    private:

        template<typename CharT>
        std::basic_string_view<CharT> make_string_view(const_iterator begin, const_iterator end)
        {
           return std::basic_string_view<CharT>(begin._Unwrapped(), end - begin);
        }

    private:
        const_iterator where_;
        const_iterator begin_;
        const_iterator end_;

    };


}


#endif //__cursive_tokens_h__