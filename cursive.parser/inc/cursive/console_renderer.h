#ifndef __console_renderer_h__
#define __console_renderer_h__

#include <cursive/document.h>
#include <iostream>

namespace cursive
{
    class console_renderer
    {
    public:
        template <typename CharT>
        static void render(const basic_document<CharT>& doc, std::ostream& os)
        {
            for(auto e=doc.begin(); e!=doc.end(); ++e)
            {
                const auto depth = e.depth();
                for (int i = 0; i < depth*4; ++i)
                    os << " ";
                switch (e->type())
                {
                case element_types::document:
                    os << "document " << std::endl;
                    break;
                case element_types::heading:
                    os << "heading " << e->depth() << std::endl;
                    break;
                case element_types::ordered_list:
                    os << "ordered_list " << std::endl;
                    break;
                case element_types::unordered_list:
                    os << "unordered_list " << std::endl;
                    break;
                case element_types::paragraph:
                    os << "paragraph " << std::endl;
                    break;
                case element_types::plain_text:
                    os << "(" << e->value() << ")" << std::endl;
                    break;
                }
            }
        }
    };


};

#endif // __console_renderer_h__