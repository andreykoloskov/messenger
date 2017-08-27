#ifndef CHAT_MESSAGE_H
#define CHAT_MESSAGE_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>

//message protocol
//header
//    Type xxx\n
//    From xxx\n
//    To xxx\n
//    Size xxx\n
//body

class chat_message
{
public:
    enum { header_length = 256 };
    enum { max_body_length = 512 };

    chat_message();
    const char* data() const;
    char *data();
    std::size_t length() const;
    const char* body() const;
    char* body();
    std::size_t body_length() const;
    void body_length(std::size_t new_length);
    void set_type(std::string type);
    void set_from(std::string from);
    void set_to(std::string to);
    std::string get_type() const;
    std::string get_from() const;
    std::string get_to() const;
    bool decode_header();
    void encode_header();

private:
    char data_[header_length + max_body_length];
    std::string type_;
    std::string from_;
    std::string to_;
    std::size_t body_length_;
};

#endif // CHAT_MESSAGE_H
