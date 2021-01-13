#ifndef __cursive_pdf_page_h__
#define __cursive_pdf_page_h__

#include <hpdf.h>

#include <cursive/document.h>

namespace cursive
{
namespace pdf
{
    class page
    {
    public:
        using handle_type = HPDF_Page;

    public:
        page(document &d);
        ~page();

    private:
        document &doc_;
        handle_type hpage_;
    };
}
}

#endif