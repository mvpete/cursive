#ifndef __html_renderer_h__
#define __html_renderer_h__

#include <cursive/document.h>
#include <iostream>
#include <string>


namespace cursive
{
namespace detail
{
    void indent(std::ostream &os, size_t depth)
    {
        for (auto i = 0; i < depth * 4; ++i)
            os << " ";
    }



    using attribute_list = std::vector<std::pair<std::string, std::string>>;
    class tag
    {
    public:
        tag(const std::string &name, size_t depth, std::ostream &os, const attribute_list &attributes = {}, bool self = false)
            :os_(os), name_(name), depth_(depth), self_(self), owned_(true)
        {
            os << "\n";
            indent(os, depth);
            os_ << "<" << name_;

            for (const auto &attr : attributes)
            {
                os_ << " " << attr.first << "=\"" << attr.second << "\"";
            }

            if (self_)
                os << "/";
            os << ">";
        }

        tag(tag &&rhs) noexcept
            :os_(rhs.os_),owned_(false)
        {
            std::swap(name_, rhs.name_);
            std::swap(self_, rhs.self_);
            std::swap(depth_, rhs.depth_);
            std::swap(owned_, rhs.owned_);
        }

        ~tag()
        {
            if (!self_ && owned_)
            {
                os_ << "</" << name_ << ">\n";
            }
        }

    private:
        std::string name_;
        size_t depth_;
        bool self_;
        bool owned_;
        std::ostream &os_;
    };


}

    class html_renderer
    {

    private:
        html_renderer()
            :depth_(0)
        {
        }

        template <typename CharT>
        void render_impl(const basic_document<CharT> &doc, std::ostream &os)
        {
            tags_.emplace_back("html", 0, os);
            for (auto e = doc.begin(); e != doc.end(); ++e)
            {
                const auto depth = e.depth() + 1;
                if (depth_ > depth)
                {
                    for (int i = depth; i < depth_; ++i)
                    {
                        if(!tags_.empty())
                            tags_.pop_back();
                    }
                }
                depth_ = depth;

                switch (e->type())
                {
                case element_types::document:
                    tags_.emplace_back("body", depth, os);
                    break;
                case element_types::heading:
                    tags_.emplace_back("h1", depth, os);
                    break;
                case element_types::ordered_list:
                    tags_.emplace_back("ol", depth, os);
                    break;
                case element_types::unordered_list:
                    tags_.emplace_back("ul", depth, os);
                    break;
                case element_types::list_item:
                    tags_.emplace_back("li", depth, os);
                    break;
                case element_types::paragraph:
                    tags_.emplace_back("p", depth, os);
                    break;
                case element_types::link:
                {
                    detail::indent(os, depth);
                    detail::attribute_list attr;
                    attr.emplace_back("href", e->data());
                    detail::tag link("a", depth, os, attr);
                    os << e->value();
                    break;
                }
                case element_types::code:
                {
                    tags_.emplace_back("code", depth, os);
                    break;
                }
                case element_types::plain_text:
                    os << e->value();
                    break;
                }
            }
            while (!tags_.empty())
                tags_.pop_back();
        }




    private:
        std::vector<detail::tag> tags_;
        size_t depth_;
    public:
        template <typename CharT>
        static void render(const basic_document<CharT> &doc, std::ostream &os)
        {
            html_renderer r;
            r.render_impl(doc, os);
        }
    };

}

#endif // __html_renderer_h__