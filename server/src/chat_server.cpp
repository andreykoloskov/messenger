#include "server.h"
#include "chat_server.h"

using boost::asio::ip::tcp;

chat_server::chat_server(boost::asio::io_service& io_service,
                         const tcp::endpoint& endpoint)
        : acceptor_(io_service, endpoint), socket_(io_service)
{
    do_accept();
}

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



