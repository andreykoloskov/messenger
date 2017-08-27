#include "chat_message.h"

chat_message::chat_message()
        : body_length_(0) {}

const char *
chat_message::data() const
{
    return data_;
}

char *chat_message::data()
{
    return data_;
}

std::size_t
chat_message::length() const
{
    return header_length + body_length_;
}

const char *
chat_message::body() const
{
    return data_ + header_length;
}

char *
chat_message::body()
{
    return data_ + header_length;
}

std::size_t
chat_message::body_length() const
{
    return body_length_;
}

void
chat_message::body_length(std::size_t new_length)
{
    body_length_ = new_length;
    if (body_length_ > max_body_length)
        body_length_ = max_body_length;
}

void
chat_message::set_type(std::string type)
{
    type_ = type;
}

void
chat_message::set_from(std::string from)
{
    from_ = from;
}

void
chat_message::set_to(std::string to)
{
    to_ = to;
}

std::string
chat_message::get_type() const
{
    return type_;
}

std::string
chat_message::get_from() const
{
    return from_;
}

std::string
chat_message::get_to() const
{
    return to_;
}

bool
chat_message::decode_header()
{
    char header[header_length + 1] = "";
    std::strncat(header, data_, header_length);

    std::vector<std::string> header_split;
    boost::algorithm::split(header_split, header,
            boost::algorithm::is_any_of("\n"),
            boost::algorithm::token_compress_on);

    for (std::string hd: header_split)
    {
        std::vector<std::string> header_pair;
        boost::algorithm::split(header_pair, hd,
                boost::algorithm::is_any_of(" "),
                boost::algorithm::token_compress_on);

        if (header_pair[0] == "Type")
            set_type(header_pair[1]);
        if (header_pair[0] == "From")
            set_from(header_pair[1]);
        if (header_pair[0] == "To")
            set_to(header_pair[1]);
        if (header_pair[0] == "Size")
            body_length_ = std::atoi(header_pair[1].c_str());
    }

    if (body_length_ > max_body_length)
    {
        body_length_ = 0;
        return false;
    }

    return true;
}

void
chat_message::encode_header()
{
    char header[header_length + 1] = "";

    std::sprintf(header, "Type %s\nFrom %s\nTo %s\nSize %d",
            type_.c_str(), from_.c_str(), to_.c_str(),
            static_cast<int>(body_length_));
    std::memcpy(data_, header, header_length);
}
