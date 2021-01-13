#include <cursive/page.h>

using namespace cursive::pdf;

page::page(document &d)
    :doc_(d), hpage_(HPDF_AddPage(doc_.handle()))
{
}

page::~page()
{
}

