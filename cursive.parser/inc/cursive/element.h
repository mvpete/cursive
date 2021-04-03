#ifndef __cursive_document_h__
#define __cursive_document_h__

#include <stack>
#include <vector>
#include <string_view>
#include <cursive/tokenizer.h>

namespace cursive
{
    enum class element_types
    {
        none = 0,
        document,
        code,
        image,
        link,
        heading,
        paragraph,
        ordered_list,
        unordered_list,
        list_item,
        plain_text,
        emphasis,
        strong,
        strike,
        horizontal_rule,
        container
    };

    template <typename CharT>
    class basic_element
    {
    public:
        using char_t = CharT;
        using element_t = basic_element<char_t>;

    public:
        /// <summary>
       /// const_iterator is depth first traversal
       /// </summary>
        class const_iterator
        {
        public:
            const_iterator()
                :current_(nullptr), child_(0)
            {
            }
            explicit const_iterator(const element_t &e)
                :current_(&e), child_(0)
            {
            }

            const_iterator &operator++()
            {
                move_next_sibling();
                return *this;
            }

            const size_t depth() const
            {
                return levels_.size();
            }

            bool operator==(const const_iterator &rhs) const
            {
                return current_ == rhs.current_ && child_ == rhs.child_;
            }

            bool operator!=(const const_iterator &rhs) const
            {
                return !operator==(rhs);
            }

            const element_t *operator->() const
            {
                return current_;
            }

            const element_t &operator*() const
            {
                return *current_;
            }

        private:
            void move_next_sibling()
            {
                if (current_ != nullptr && child_ < current_->children().size())
                {
                    levels_.push(std::make_pair(current_, child_));
                    current_ = &current_->child_at(child_);
                    child_ = 0;
                }
                else
                {
                    while (!levels_.empty())
                    {
                        auto last = levels_.top();
                        levels_.pop();

                        current_ = last.first;
                        child_ = last.second + 1;

                        if (child_ < current_->children().size())
                        {
                            levels_.push(std::make_pair(current_, child_));
                            current_ = &current_->child_at(child_);
                            child_ = 0;
                            break;
                        }
                    }
                    if (levels_.empty())
                    {
                        current_ = nullptr;
                        child_ = 0;
                    }
                }
            }

        private:

            std::stack<std::pair<const element_t *, size_t>> levels_;
            const element_t *current_;
            size_t child_;
        };

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

        basic_element(element_types type, const std::basic_string<char_t> &value)
            :type_(type), value_(value), depth_(value.size())
        {
        }

        basic_element(element_types type, const std::basic_string_view<char_t> &value)
            :type_(type), value_(value), depth_(value.size())
        {
        }

        basic_element(element_types type, const std::basic_string_view<char_t> &value, const std::basic_string_view<char_t> &data)
            :type_(type), value_(value), depth_(value.size()), data_(data)
        {
        }

        basic_element(const basic_element &rhs)
            :type_(rhs.type_), depth_(rhs.depth_), children_(rhs.children_), value_(rhs.value_), data_(rhs.data_)
        {
        }

        basic_element(basic_element &&rhs) noexcept
        {
            std::swap(type_, rhs.type_);
            std::swap(depth_, rhs.depth_);
            std::swap(children_, rhs.children_);
            std::swap(value_, rhs.value_);
            std::swap(data_, rhs.data_);
        }

        basic_element &operator=(const basic_element &rhs)
        {
            type_ = rhs.type_;
            depth_ = rhs.depth_;
            children_ = rhs.children_;
            value_ = rhs.value_;
            data_ = rhs.data_;
            return *this;
        }

        basic_element &operator=(basic_element &&rhs) noexcept
        {
            std::swap(type_, rhs.type_);
            std::swap(depth_, rhs.depth_);
            std::swap(children_, rhs.children_);
            std::swap(value_, rhs.value_);
            std::swap(data_, rhs.data_);
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

        const std::vector<element_t> &children() const
        {
            return children_;
        }

        const std::basic_string<char_t> &value() const
        {
            return value_;
        }

        const std::basic_string<char_t> &data() const
        {
            return data_;
        }

        void append(const std::basic_string_view<CharT> &text)
        {
            value_ += text;
        }

        element_t &add(element_t &&child)
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

        element_t &add(const element_t &child)
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

        element_t &emplace_child(element_t &&e)
        {
            children_.emplace_back(std::move(e));
            return children_.back();
        }

        const element_t &child_at(size_t index) const
        {
            return children_.at(index);
        }

        bool empty() const
        {
            return type_ == element_types::none;
        }

        const_iterator begin() const
        {
            return const_iterator(*this);
        }
        const_iterator end() const
        {
            return const_iterator();
        }
    public:
        const element_t &operator[](size_t index) const
        {
            return children_[index];
        }

    private:
        element_types type_;
        size_t depth_;
        std::vector<element_t> children_;
        std::basic_string<char_t> value_;
        std::basic_string<char_t> data_;
    };

    template<typename CharT>
    basic_element<CharT> make_code_element()
    {
        return basic_element<CharT>(element_types::code);
    }

    template<typename CharT>
    basic_element<CharT> make_header_element(size_t depth)
    {
        return basic_element<CharT>(element_types::heading, depth);
    }

    template<typename CharT>
    basic_element<CharT> make_paragraph_element()
    {
        return basic_element<CharT>(element_types::paragraph);
    }

    template<typename CharT>
    basic_element<CharT> make_text_element(const std::basic_string_view<CharT> &text)
    {
        return basic_element<CharT>(element_types::plain_text, text);
    }

    template<typename CharT>
    basic_element<CharT> make_text_element(const std::basic_string<CharT> &text)
    {
        return basic_element<CharT>(element_types::plain_text, text);
    }

    template<typename CharT>
    basic_element<CharT> make_text_element(const CharT text)
    {
        return basic_element<CharT>(element_types::plain_text, std::basic_string<CharT>(1,text));
    }

    template<typename CharT>
    basic_element<CharT> make_space_element()
    {
        return basic_element<CharT>(element_types::plain_text, detail::space_string_view<CharT>());
    }

    template<typename CharT>
    basic_element<CharT> make_emphasis_element(size_t depth)
    {
        return basic_element<CharT>(element_types::emphasis, depth);
    }

    template<typename CharT>
    basic_element<CharT> make_link_element(const std::basic_string_view<CharT>& text, const std::basic_string_view<CharT>& url)
    {
        return basic_element<CharT>(element_types::link, text, url);
    }

    template<typename CharT>
    basic_element<CharT> make_image_element(const std::basic_string_view<CharT>& text, const std::basic_string_view<CharT>& url)
    {
        return basic_element<CharT>(element_types::image, text, url);
    }

    template<typename CharT>
    basic_element<CharT> make_inline_element(tokens t, const std::basic_string_view<CharT>& text)
    {
        switch (t)
        {
        case tokens::emphasis:
            return basic_element<CharT>(element_types::emphasis, text);
        case tokens::strikethrough:
            return basic_element<CharT>(element_types::strike, text);
        case tokens::backtick:
            return basic_element<CharT>(element_types::code, text);
        default:
            return basic_element<CharT>(element_types::plain_text, text);
        }
        throw 1;
    }

    template<typename CharT>
    basic_element<CharT> make_inline_element(element_types type)
    {
        return basic_element<CharT>(type);
    }

    template <typename CharT>
    basic_element<CharT> make_unordered_list_element()
    {
        return basic_element<CharT>(element_types::unordered_list);
    }

    template <typename CharT>
    basic_element<CharT> make_container_element()
    {
        return basic_element<CharT>(element_types::container);
    }

    
}

#endif //__cursive_document_h__
