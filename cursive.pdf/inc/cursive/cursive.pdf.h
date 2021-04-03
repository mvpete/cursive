#ifndef __cursive_pdf_h__
#define __cursive_pdf_h__

#include <cursive/document.h>

namespace cursive
{
namespace pdf
{
    class renderer
    {
    public:
        template<typename CharT>
        void render(const basic_document<CharT> &doc, std::ostream &os)
        {
        }
    };
}
}


#endif //__cursive_pdf_h__