#ifndef CHAT_MESSAGE_H
#define CHAT_MESSAGE_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

//message protocol
//{'type':'type', 'from':'from', 'to':'to', 'body':'body'}

class chat_message
{
public:
    enum { MSG_LENGTH = 512 };

    chat_message()
    {
        memset(buffer_, 0, sizeof(buffer_));
    }

    chat_message(std::string type, std::string from, std::string to,
                 std::string body)
    {
        memset(buffer_, 0, sizeof(buffer_));
        set_data(type, from, to, body);
        encode();
    }

    void set_type(std::string type) { type_ = type; }
    void set_from(std::string from) { from_ = from; }
    void set_to(std::string to) { to_ = to; }
    void set_body(std::string body) { body_ = body; }

    void set_data(std::string type, std::string from, std::string to,
                  std::string body)
    {
        set_type(type);
        set_from(from);
        set_to(to);
        set_body(body);
    }

    std::string get_type() const { return type_; }
    std::string get_from() const { return from_; }
    std::string get_to() const { return to_; }
    std::string get_body() const { return body_; }
    std::string get_data() const { return data_; }
    char *get_buffer() { return buffer_; }
    bool decode();
    void encode();

private:
    std::string type_;
    std::string from_;
    std::string to_;
    std::string body_;
    std::string data_;
    char buffer_[MSG_LENGTH];
};

#endif // CHAT_MESSAGE_H
