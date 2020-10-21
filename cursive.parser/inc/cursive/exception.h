#ifndef __cursive_exception_h__
#define __cursive_exception_h__

#include <stdexcept>

#include <cursive/loc.h>

namespace cursive
{
    class parser_exception : public std::exception
    {
    public:
        parser_exception(const loc& loc)
            :std::exception("failed to parse")
        {

        }
    };
}

#endif // __cursive_exception_h__