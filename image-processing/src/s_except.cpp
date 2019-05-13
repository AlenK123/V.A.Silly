#include "s_except.hpp"

s_except::s_except(std::string err_msg) {
    this->err_msg = err_msg;
}

s_except::~s_except() {
    //
}

const char * s_except::what() const throw() {
    return this->err_msg.c_str();
}

