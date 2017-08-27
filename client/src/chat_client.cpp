#include "chat_client.h"

chat_client::chat_client(boost::asio::io_service& io_service,
                         tcp::resolver::iterator endpoint_iterator,
                         std::string client_name)
        : io_service_(io_service), socket_(io_service),
          client_name_(client_name)
{
    do_connect(endpoint_iterator);
}

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

std::string
chat_client::get_client_name()
{
    return client_name_;
}

void
chat_client::close()
{
    io_service_.post([this]() { socket_.close(); });
}

void
chat_client::do_connect(tcp::resolver::iterator endpoint_iterator)
{
    boost::asio::async_connect(socket_, endpoint_iterator,
            [this](boost::system::error_code ec, tcp::resolver::iterator)
            {
                if (!ec)
                    do_read_header();
            });
}

void
chat_client::do_read_header()
{
    boost::asio::async_read(socket_,
            boost::asio::buffer(read_msg_.data(),
                    chat_message::header_length),
            [this](boost::system::error_code ec, std::size_t)
            {
                if (!ec && read_msg_.decode_header())
                    do_read_body();
                else
                    socket_.close();
            });
}

void
chat_client::do_read_body()
{
    boost::asio::async_read(socket_,
            boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
            [this](boost::system::error_code ec, std::size_t)
            {
                if (!ec)
                {
                    std::cout.write(read_msg_.body(),
                            read_msg_.body_length());
                    std::cout << "\n";
                    do_read_header();
                }
                else
                    socket_.close();
            });
}

void
chat_client::do_write()
{
    boost::asio::async_write(socket_,
            boost::asio::buffer(write_msgs_.front().data(),
                    write_msgs_.front().length()),
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
