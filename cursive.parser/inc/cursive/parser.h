#ifndef __cursive_parser_h__
#define __cursive_parser_h__

#include <cursive/exception.h>
#include <cursive/tokenizer.h>
#include <cursive/element.h>

#include <stack>

namespace cursive
{
    template <typename CharT>
    class basic_parser
    {
    public:
        using char_t = CharT;
        using element_t = basic_element<CharT>;
        using tokenizer_t = basic_tokenizer<char_t>;
        using token_t = typename basic_tokenizer<char_t>::token_t;
    public:
        basic_parser(const_iterator<CharT> begin, const_iterator<CharT> end)
            :tokenizer_(begin, end), document_(make_document<char_t>())
        {
        }

        element_t parse()
        {
            while (read_line())
                parse_line();
            // incase we had one line.
            parse_line();

            return document_;
        }

    private:
        void parse_header()
        {
            auto nxt = tokenizer_.next();
            if (nxt != tokens::space && nxt != tokens::eol && nxt != tokens::eof)
                throw parser_exception(tokenizer_.loc());
        }

        bool read_line()
        {
            line_.clear();
            auto tkn = tokenizer_.next();
            while (tkn != tokens::eol && tkn != tokens::eof)
            {
                line_.push_back(std::move(tkn));
                tkn = std::move(tokenizer_.next());
            }
            if (tkn == tokens::eof)
                return false;
            return true;
        }

        void parse_line()
        {
            if (line_.empty())
                return;

            element_t root;
            bool skip_space = true;
            for(auto i=line_.begin(); i!=line_.end(); ++i)
            {
                const auto& t = *i;
                switch (t.code())
                {
                case tokens::header:
                    if (root.empty())
                    {
                        root = make_header_element<CharT>(t.count());
                        skip_space = true;
                    }
                    else
                    {
                        root.add(make_text_element(t.text()));
                    }
                    break;
                case tokens::text:
                    if (root.empty())
                        root = make_paragraph_element<CharT>(t.text());
                    else
                        root.add(make_text_element(t.text()));
                    skip_space = false;
                    break;
                case tokens::space:
                    if (!skip_space && !root.empty())
                        root.add(make_text_element(t.text()));
                    skip_space = false;
                    break;
                case tokens::emphasis:
                {

                    auto elem = parse_inline(i, line_.end());

                    if (!root.empty())
                    {
                        root.add(std::move(elem));
                        break;
                    }
                    else if (i + 1 == line_.end())
                    {
                        elem.type(element_types::paragraph);
                        root = std::move(elem);
                        break;
                    }

                }
                }
            }


            document_.add(std::move(root));
        }


    private:

        template <typename TokenIt>
        element_t parse_inline(TokenIt &begin, TokenIt end)
        {
            if (begin + 1 == end)
                return make_text_element(begin->text());

           /* if (begin->code() == link)
                return parse_inline_link();
            else if (begin->code() == image)
                reutrn parse_inline_image();*/

            auto bookend = begin + 1;
            auto type = begin->code();
            // advance until we cap
            while (bookend != end && bookend->code() != type)
                bookend++;
            if (bookend == end)
            {
                return make_text_element(bookend->text());
            }

            auto left = begin->count();
            auto right = bookend->count();

            element_t elem;

            if (left > right)
            {
                elem = make_container_element<CharT>();
                auto lt = begin->split(right);
                elem.add(make_text_element(lt.text()));
            }
            auto inner = make_element<CharT>(bookend->code(), bookend->text());
            ++begin;
            for (begin; begin < bookend; ++begin)
                inner.add(parse_inline(begin, bookend));

            if(elem.empty())
                elem = inner;
            else
                elem.add(inner);      

            if (right > left)
            {
                auto rt = bookend->split(left);
                auto cont = make_container_element<CharT>();
                cont.add(elem);
                cont.add(make_text_element(rt.text()));
                return cont;
            }

            return elem;
        }


    private:
        tokenizer_t tokenizer_;
        element_t document_;
        std::vector<token_t> line_;


    };

    template <typename CharT>
    basic_element<CharT> parse(const std::basic_string<CharT>& text)
    {
        std::basic_string_view<CharT> sv(text);
        basic_parser<CharT> parser(sv.cbegin(), sv.cend());
        return parser.parse();
    }

    using parser = basic_parser<char>;
    using wparser = basic_parser<wchar_t>;
}

#endif // __cursive_parser_h__