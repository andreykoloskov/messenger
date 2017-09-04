#include "server.h"
#include "chat_session.h"

using boost::asio::ip::tcp;

void
chat_session::deliver(const chat_message& msg)
{
    if (msg.get_to() != login_)
        return;
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if (!write_in_progress)
        do_write();
}

void
chat_session::do_read()
{
    auto self(shared_from_this());
    async_read(socket_,
            boost::asio::buffer(read_msg_.get_buffer(),
                    chat_message::MSG_LENGTH),
            [this](boost::system::error_code ec, std::size_t size)
            {
                if (!ec && read_msg_.decode())
                {
                    if (read_msg_.get_type() == "authorization")
                    {
                        login_ = read_msg_.get_from();
                        room_.first_deliver(shared_from_this());
                    }

                    room_.deliver(read_msg_);
                    do_read();
                }
                else
                    room_.leave(shared_from_this());
            });
}

void
chat_session::do_write()
{
    auto self(shared_from_this());
    async_write(socket_,
            boost::asio::buffer(write_msgs_.front().get_buffer(),
                    chat_message::MSG_LENGTH),
            [this, self](boost::system::error_code ec, std::size_t)
            {
                if (!ec)
                {
                    write_msgs_.pop_front();
                    if (!write_msgs_.empty())
                        do_write();
                }
                else
                    room_.leave(shared_from_this());
            });
}
