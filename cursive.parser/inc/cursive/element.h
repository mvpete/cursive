#ifndef __cursive_document_h__
#define __cursive_document_h__

#include <vector>
#include <string_view>
#include <cursive/tokenizer.h>

namespace cursive
{
    enum class element_types { none = 0, document, heading, paragraph, ordered_list, unordered_list, plain_text, emphasis, strong, strike, container };

    template <typename CharT>
    class basic_element
    {
    public:
        using char_t = CharT;
        using element_t = basic_element<char_t>;

    public:

        basic_element()
            :depth_(0), type_(element_types::none)
        {
        }

        basic_element(element_types type)
            :type_(type), depth_(0)
        {
        }

        basic_element(element_types type, size_t depth)
            :type_(type), depth_(depth)
        {
        }

        basic_element(element_types type, const std::basic_string_view<char_t>& value)
            :type_(type), value_(value), depth_(value.size())
        {
        }

        basic_element(const basic_element& rhs)
            :type_(rhs.type_), depth_(rhs.depth_), children_(rhs.children_), value_(rhs.value_)
        {
        }

        basic_element(basic_element&& rhs) noexcept
        {
            std::swap(type_, rhs.type_);
            std::swap(depth_, rhs.depth_);
            std::swap(children_, rhs.children_);
            std::swap(value_, rhs.value_);
        }

        basic_element& operator=(const basic_element& rhs)
        {
            type_= rhs.type_;
            depth_= rhs.depth_;
            children_= rhs.children_;
            value_= rhs.value_;
            return *this;
        }

        basic_element& operator=(basic_element&& rhs) noexcept
        {
            std::swap(type_, rhs.type_);
            std::swap(depth_, rhs.depth_);
            std::swap(children_, rhs.children_);
            std::swap(value_, rhs.value_);
            return *this;
        }

        element_types type() const
        {
            return type_;
        }

        void type(element_types type)
        {
            type_ = type;
        }

        size_t depth() const
        {
            return depth_;
        }

        const std::vector<element_t>& children() const
        {
            return children_;
        }

        const std::basic_string<char_t>& value() const
        {
            return value_;
        }

        void append(const std::basic_string_view<CharT>& text)
        {
            value_ += text;
        }

        element_t& add(element_t &&child)
        {
            if (child.type() == element_types::container)
            {
                children_.insert(children_.end(), child.children_.begin(), child.children_.end());
            }
            else
            {
                if (children_.empty() || children_.back().type() != child.type())
                    children_.emplace_back(std::move(child));
                else
                    children_.back().append(child.value());
            }
            return children_.back();
        }

        element_t& add(const element_t& child)
        {
            if (child.type() == element_types::container)
            {
                children_.insert(children_.end(), child.children_.begin(), child.children_.end());                
            }
            else
            {
                if (children_.empty() || children_.back().type() != child.type())
                    children_.emplace_back(child);
                else
                    children_.back().append(child.value());
            }
            return children_.back();
        }

        const element_t& child_at(size_t index)
        {
            return children_.at(index);
        }

        bool empty() const
        {
            return type_ == element_types::none;
        }

    private:
        element_types type_;
        size_t depth_;
        std::vector<element_t> children_;
        std::basic_string<CharT> value_;
    };

    template<typename CharT>
    basic_element<CharT> make_header_element(size_t depth)
    {
        return basic_element<CharT>(element_types::heading, depth);
    }

    template<typename CharT>
    basic_element<CharT> make_paragraph_element(const std::basic_string_view<CharT>& text)
    {
        return basic_element<CharT>(element_types::paragraph, text);
    }

    template<typename CharT>
    basic_element<CharT> make_text_element(const std::basic_string_view<CharT>& text)
    {
        return basic_element<CharT>(element_types::plain_text, text);
    }

    template<typename CharT>
    basic_element<CharT> make_emphasis_element(size_t depth)
    {
        return basic_element<CharT>(element_types::emphasis, depth);
    }
    

    template<typename CharT>
    basic_element<CharT> make_element(tokens t, const std::basic_string_view<CharT>& text)
    {
        switch (t)
        {
        case tokens::emphasis:
            return basic_element<CharT>(element_types::emphasis, text);
        case tokens::strikethrough:
            return basic_element<CharT>(element_types::strike, text);
        }
    }

    template <typename CharT>
    basic_element<CharT> make_container_element()
    {
        return basic_element<CharT>(element_types::container);
    }

    template <typename CharT>
    basic_element<CharT> make_document()
    {
        return basic_element<CharT>(element_types::document);
    }
    
}

#endif //__cursive_document_h__
