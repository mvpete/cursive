#ifndef __cursive_pdf_error_h__
#define __cursive_pdf_error_h__

#include <stdexcept>

namespace cursive
{
namespace pdf
{

    class error : public std::runtime_error
    {
    public:
        error(unsigned long error_no, unsigned long detail_no, void *user_data);

    };

    void throw_on_error(unsigned long error_no, unsigned long detail_no, void *user_data)
    {
        throw error(error_no, detail_no, user_data);
    }

}
}

#endif