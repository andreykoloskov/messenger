#ifndef CLIENT_INCLUDE_CHAT_CLIENT_H_
#define CLIENT_INCLUDE_CHAT_CLIENT_H_

#include "client.h"
#include "chat_message.h"

using boost::asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

class chat_client
{
public:
    chat_client(boost::asio::io_service& io_service,
                tcp::resolver::iterator endpoint_iterator,
                std::string client_name):
            io_service_(io_service), socket_(io_service),
            client_name_(client_name)
    {
        do_connect(endpoint_iterator);
    }

    void write(const chat_message& msg);
    std::string get_client_name() { return client_name_; }
    void close() { io_service_.post([this]() { socket_.close(); }); }

private:
    void do_connect(tcp::resolver::iterator endpoint_iterator);
    void do_read();
    void do_write();

    boost::asio::io_service& io_service_;
    tcp::socket socket_;
    std::string client_name_;
    chat_message read_msg_;
    chat_message_queue write_msgs_;
};

#endif /* CLIENT_INCLUDE_CHAT_CLIENT_H_ */
