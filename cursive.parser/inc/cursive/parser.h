#ifndef __cursive_parser_h__
#define __cursive_parser_h__

#include <cursive/exception.h>
#include <cursive/tokenizer.h>
#include <cursive/document.h>

#include <stack>

namespace cursive
{
    template <typename CharT>
    class basic_parser
    {
    public:
        using char_t = CharT;
        using element_t = basic_element<CharT>;
        using document_t = basic_document<CharT>;
        using tokenizer_t = basic_tokenizer<char_t>;
        using token_t = typename basic_tokenizer<char_t>::token_t;
    public:
        basic_parser(const_iterator<CharT> begin, const_iterator<CharT> end)
            :tokenizer_(begin, end), document_(make_document<char_t>())
        {
        }

        document_t parse()
        {
            element_t elem;
            std::vector<token_t> line;
            bool has_more = false;
            bool in_code = false;
            while (read_line(line))
            {
                if (parse_line(elem, line, has_more, in_code))
                {
                    document_.add(elem);
                    elem = element_t{};
                }
            }
            // incase we had one line.
            parse_line(elem, line, has_more, in_code);
            if(!elem.empty())
                document_.add(elem);
            return document_;
        }

    private:
        void parse_header()
        {
            auto nxt = tokenizer_.next();
            if (nxt != tokens::space && nxt != tokens::eol && nxt != tokens::eof)
                throw parser_exception(tokenizer_.loc());
        }

        bool read_line(std::vector<token_t> &line)
        {
            line.clear();
            auto tkn = tokenizer_.next();
            while (tkn != tokens::eol && tkn != tokens::eof)
            {
                line.push_back(std::move(tkn));
                tkn = std::move(tokenizer_.next());
            }
            if (tkn == tokens::eof)
                return false;   
            if (!line.empty())
                line.push_back(std::move(tkn));
            return true;
        }

        bool parse_line(element_t &elem, std::vector<token_t> &line, bool &has_more, bool &in_code)
        {
            if (line.empty())
            {
                has_more = false;
                return true;
            }

            element_t *idx_ptr = nullptr;
            bool skip_space = true;
            for(auto i=line.begin(); i!=line.end(); ++i)
            {
                const auto& t = *i;

                if (in_code && idx_ptr != nullptr && !(t.code() == tokens::backtick && t.count() >= 3))
                {                   
                    if (t.code() == tokens::eol)
                    {
                        idx_ptr->add(make_text_element(detail::get_literal<CharT>(literals::lf)));
                    }
                    else
                    {
                        idx_ptr->add(make_text_element(t.text()));
                    }
                    continue;
                }

                switch (t.code())
                {
                case tokens::backtick:
                {
                    element_t e;
                    if (t.count() == 1)
                    {
                        e = parse_inline(i, line.end());
                    }
                    else if (t.count() >= 3 )
                    {
                        if (!in_code)
                        {
                            elem = make_code_element<CharT>();
                            idx_ptr = &elem;
                        }
                        in_code = !in_code;
                        has_more = true;
                        return false;
                    }
                    else
                    {
                        e = make_text_element(t.text());
                    }

                    if (elem.empty() && !e.empty())
                    {
                        elem = e;
                        idx_ptr = &elem;
                    }
                    else
                    {
                        idx_ptr = &elem;
                        idx_ptr->add(std::move(e));
                    }
                    break;
                }
                case tokens::header:
                {
                    auto hdr = make_header_element<CharT>(t.count());
                    if (elem.empty())
                    {
                        elem = hdr;
                        idx_ptr = &elem;
                        skip_space = true;
                    }
                    else if(idx_ptr!=nullptr)
                    {
                        if (idx_ptr->type() != element_types::heading)
                            idx_ptr = &idx_ptr->add(hdr);
                        else
                            idx_ptr->add(make_text_element(t.text()));
                    }
                    break;
                }
                case tokens::image:
                {
                    if (elem.empty())
                    {
                        elem = make_inline_element<char_t>(element_types::container);
                        idx_ptr = &elem;
                    }
                    if (idx_ptr != nullptr)
                        idx_ptr->emplace_child(make_image_element(t.text(), t.data()));
                    break;
                }
                case tokens::link:
                {
                    if (elem.empty())
                    {
                        elem = make_inline_element<char_t>(element_types::container);
                        idx_ptr = &elem;
                    }
                    if (idx_ptr != nullptr)
                        idx_ptr->emplace_child(make_link_element(t.text(), t.data()));
                    break;
                }
                case tokens::text:
                    if (elem.empty())
                    {
                        elem = make_paragraph_element<CharT>();
                        idx_ptr = &elem;
                    }
                    else if(idx_ptr == nullptr)
                    {
                        idx_ptr = &elem;
                    }
                    
                    idx_ptr->add(make_text_element(t.text()));
                    has_more = true;
                    skip_space = false;
                    break;
                case tokens::space:
                {
                    if (t.count() >= 4 && idx_ptr==nullptr)
                    {
                        elem = make_code_element<CharT>();
                        idx_ptr = &elem;
                        break;
                    }

                    if (idx_ptr!=nullptr)
                        idx_ptr->add(make_text_element(t.text()));
                    skip_space = false;
                    break;
                }
                case tokens::post_header:
                {
                    if (!elem.empty() && elem.type() == element_types::paragraph)
                    {
                        elem.type(element_types::heading);
                        break;
                    }

                    if (elem.empty())
                    {
                        elem = make_paragraph_element<CharT>();
                        idx_ptr = &elem;
                        idx_ptr->add(make_text_element(t.text()));
                    }
                    else if (idx_ptr == nullptr)
                    {
                        idx_ptr = &elem;
                    }
                    idx_ptr->add(make_text_element(t.text()));
                    
                    
                    break;
                }
                case tokens::emphasis:
                {
                    auto inline_elem = parse_inline(i, line.end());
                    if (elem.empty())
                    {
                        elem = inline_elem;
                        idx_ptr = &elem;
                    }
                    else
                    {
                        idx_ptr = &elem;
                        idx_ptr->add(std::move(inline_elem));                      
                    }
                    break;
                }
                case tokens::horizontal_rule:
                {
                    if (elem.empty())
                        elem = make_inline_element<char_t>(element_types::horizontal_rule);
                    if (idx_ptr != nullptr)
                        idx_ptr->add(make_text_element(t.text()));
                    break;
                }
                case tokens::unordered_list:
                {
                    if (elem.empty())
                        elem = make_inline_element<char_t>(element_types::unordered_list);
                    else if (idx_ptr == nullptr)
                        idx_ptr = &elem;

                    if (elem.type() == element_types::unordered_list)
                        idx_ptr = &elem.emplace_child(make_inline_element<char_t>(element_types::list_item));
                    else
                        idx_ptr->add(make_text_element(t.text()));
                    has_more = true;
                    break;
                }
                case tokens::ordered_list:
                {
                    if (elem.empty())
                        elem = make_inline_element<char_t>(element_types::ordered_list);
                    else if (idx_ptr == nullptr)
                        idx_ptr = &elem;

                    if (elem.type() == element_types::ordered_list)
                        idx_ptr = &elem.emplace_child(make_inline_element<char_t>(element_types::list_item));
                    else
                        idx_ptr->add(make_text_element(t.text()));
                    has_more = true;
                    break;
                }
                case tokens::eol:
                {
                    if (idx_ptr != nullptr && idx_ptr->type() == element_types::paragraph)
                    {
                        idx_ptr->add(make_space_element<CharT>());
                    }
                    //[[fallthrough]]
                }
                default:
                    return false;
                }
            }

            return !has_more;
        }


    private:

        template <typename TokenIt>
        element_t parse_inline(TokenIt &begin, TokenIt end)
        {
            if (begin + 1 == end)
                return make_text_element(begin->text());

            auto bookend = begin + 1;
            auto type = begin->code();
            // advance until we cap
            while (bookend != end && bookend->code() != type)
                ++bookend;
            if (bookend == end)
            {
                auto e = bookend - 1;
                return make_text_element(e->text());
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
            auto inner = make_inline_element<CharT>(bookend->code(), bookend->text());
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
        document_t document_;

    };
     

    template <typename CharT>
    basic_document<CharT> parse(const std::basic_string<CharT>& text)
    {
        std::basic_string_view<CharT> sv(text);
        basic_parser<CharT> parser(sv.cbegin(), sv.cend());
        return parser.parse();
    }

    using parser = basic_parser<char>;
    using wparser = basic_parser<wchar_t>;
}

#endif // __cursive_parser_h__