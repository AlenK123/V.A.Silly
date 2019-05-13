#pragma once
#include <stdexcept>
#include <string>

class s_except : std::exception {
private:
    std::string err_msg;
public:
    s_except(std::string err_msg);
    virtual ~s_except();

    virtual const char *what() const throw();
};