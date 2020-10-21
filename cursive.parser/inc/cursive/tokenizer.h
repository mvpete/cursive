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
        text,
        space,
        ordered_list,
        unordered_list,
        rbrack,
        lbrack,
        ibrack,
        inline_url,
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
        basic_token(tokens code)
            :code_(code)
        {
        }

        basic_token(tokens code, const_iterator<CharT> begin, const_iterator<CharT> end)
            :code_(code), text_(&(*begin), end-begin)
        {
        }

        basic_token(const basic_token& rhs)
            :code_(rhs.code_), text_(rhs.text_)
        {

        }

        basic_token& operator=(const basic_token& rhs)
        {
            code_ = rhs.code_;
            text_ = rhs.text_;
            return *this;
        }

        basic_token& operator=(basic_token&& rhs)
        {
            std::swap(code_, rhs.code_);
            std::swap(text_, rhs.text_);
            return *this;
        }

        basic_token(basic_token&& rhs)
            :code_(tokens::empty)
        {
            std::swap(code_, rhs.code_);
            std::swap(text_, rhs.text_);
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
            case detail::get_literal<char_t>(detail::literals::hashtag):
                return scan_header_token();

            case detail::get_literal<char_t>(detail::literals::minus):
            case detail::get_literal<char_t>(detail::literals::equal):
                return scan_post_header_token();

            case detail::get_literal<char_t>(detail::literals::asteriks):
            case detail::get_literal<char_t>(detail::literals::underscore):
                return scan_emphasis_token();

            case detail::get_literal<char_t>(detail::literals::tilde):
                return scan_strike_token();

            case detail::get_literal<char_t>(detail::literals::space):
            case detail::get_literal<char_t>(detail::literals::tab):
                return scan_whitespace_token();

            case detail::get_literal<char_t>(detail::literals::cr):
            case detail::get_literal<char_t>(detail::literals::nl):
                return scan_eol_token();
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
        token_t scan_header_token()
        {
            auto begin = where_;
            while (where_ != end_ && *where_ == detail::get_literal<char_t>(detail::literals::hashtag))
            {
                ++where_;
            }
            return token_t(tokens::header, begin, where_);
        }

        token_t scan_post_header_token()
        {
            throw 1;
        }

        token_t scan_text_token()
        {
            auto begin = where_;
            while (where_ != end_ && is_text(*where_))
            {
                ++where_;
            }
            return token_t(tokens::text, begin, where_);
        }

        token_t scan_whitespace_token()
        {
            auto begin = where_;
            while (where_ != end_ && *where_ == detail::get_literal<char_t>(detail::literals::space) || *where_ == detail::get_literal<char_t>(detail::literals::tab))
            {
                ++where_;
            }
            return token_t(tokens::space, begin, where_);
        }

        token_t scan_emphasis_token()
        {
            auto begin = where_;
            while (where_ != end_ && *where_ == detail::get_literal<char_t>(detail::literals::underscore) || *where_ == detail::get_literal<char_t>(detail::literals::asteriks))
            {
                ++where_;
            }
            return token_t(tokens::emphasis, begin, where_);
        }

        token_t scan_strike_token()
        {
            auto begin = where_;
            while (where_ != end_ && *where_ == detail::get_literal<char_t>(detail::literals::tilde))
            {
                ++where_;
            }
            return token_t(tokens::strikethrough, begin, where_);
        }

        token_t scan_eol_token()
        {
            auto begin = where_;
            if (*where_ == detail::get_literal<char_t>(detail::literals::cr) && where_ + 1 != end_ && *(where_ + 1) == detail::get_literal<char_t>(detail::literals::nl))
            {
                where_ += 2;
            }
            else
            {
                ++where_;
            }
            return token_t(tokens::eol);

        }

        bool is_text(char_t c)
        {
            return isalpha(static_cast<int>(c));
        }
    private:
        const_iterator where_;
        const_iterator begin_;
        const_iterator end_;

    };


}


#endif //__cursive_tokens_h__