#ifndef __DIAGNOSTICS_H__
#define __DIAGNOSTICS_H__
#include <iostream>
#include <memory>

class diagnostics
{

    std::ostream *osp_;

    void set_ostream(std::ostream *os);
    enum levels { trace, info, warn, err };
    static std::unique_ptr<diagnostics> instance_;
    void log(levels lvl, const std::string &msg);
    
public:
    static void enable(std::ostream &os);
    static void log_info(const std::string &str);
};

#endif // __DIAGNOSTICS_H__
