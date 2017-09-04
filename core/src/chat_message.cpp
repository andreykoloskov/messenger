#include "chat_message.h"

bool
chat_message::decode()
{
    namespace pt = boost::property_tree;

    data_ = std::string(buffer_);
    pt::ptree root;
    std::istringstream is(data_);
    pt::read_json(is, root);
    type_ = root.get<std::string>("type");
    from_ = root.get<std::string>("from");
    to_ = root.get<std::string>("to");
    body_ = root.get<std::string>("body");

    return true;
}

void
chat_message::encode()
{
    namespace pt = boost::property_tree;

    pt::ptree root;
    root.put("type", type_);
    root.put("from", from_);
    root.put("to", to_);
    root.put("body", body_);

    std::ostringstream buf;
    pt::write_json (buf, root, false);
    data_ = buf.str();
    memset(buffer_, 0, sizeof(buffer_));
    sprintf(buffer_, "%s", data_.c_str());
}
