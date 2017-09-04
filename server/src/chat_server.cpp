#include "server.h"
#include "chat_server.h"

using boost::asio::ip::tcp;

void
chat_server::do_accept()
{
    acceptor_.async_accept(socket_,
            [this](boost::system::error_code ec)
            {
                if (!ec)
                    std::make_shared<chat_session>(
                            std::move(socket_), room_)->start();
                do_accept();
            });
}



