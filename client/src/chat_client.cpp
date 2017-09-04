#include "chat_client.h"

void
chat_client::write(const chat_message& msg)
{
    io_service_.post(
            [this, msg]()
            {
                bool write_in_progress = !write_msgs_.empty();
                write_msgs_.push_back(msg);
                if (!write_in_progress)
                    do_write();
            });
}

void
chat_client::do_connect(tcp::resolver::iterator endpoint_iterator)
{
    boost::asio::async_connect(socket_, endpoint_iterator,
            [this](boost::system::error_code ec, tcp::resolver::iterator)
            {
                if (!ec)
                    do_read();
            });
}

void
chat_client::do_read()
{
    async_read(socket_,
            boost::asio::buffer(read_msg_.get_buffer(),
                    chat_message::MSG_LENGTH),
            [this](boost::system::error_code ec, std::size_t size)
            {
                if (!ec)
                {
                    read_msg_.decode();
                    std::cout.write(read_msg_.get_body().c_str(),
                            read_msg_.get_body().length());
                    std::cout << "\n";
                    do_read();
                }
                else
                    socket_.close();
            });
}

void
chat_client::do_write()
{
    async_write(socket_,
            boost::asio::buffer(write_msgs_.front().get_buffer(),
                    chat_message::MSG_LENGTH),
            [this](boost::system::error_code ec, std::size_t)
            {
                if (!ec)
                {
                    write_msgs_.pop_front();
                    if (!write_msgs_.empty())
                        do_write();
                }
                else
                    socket_.close();
            });
}
