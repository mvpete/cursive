#include <cursive/document.h>
#include <cursive/error.h>

using namespace cursive::pdf;

document::document()
    :hdoc_(HPDF_New(cursive::pdf::throw_on_error, nullptr))
{
}

document::~document()
{
    HPDF_Free(hdoc_);
}

document::handle_type document::handle()
{
    return hdoc_;
}