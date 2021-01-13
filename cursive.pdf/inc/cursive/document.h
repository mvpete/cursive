#ifndef __cursive_pdf_document_h__
#define __cursive_pdf_document_h__

#include <hpdf.h>

namespace cursive
{
namespace pdf
{
    class document
    {
    public:
        using handle_type = HPDF_Doc;

    public:
        document();
        ~document();

        handle_type handle();

    private:
        handle_type hdoc_;
    };
}
}

#endif
