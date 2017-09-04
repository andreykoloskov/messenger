#ifndef SERVER_INCLUDE_CHAT_SERVER_H_
#define SERVER_INCLUDE_CHAT_SERVER_H_

#include "server.h"
#include "chat_session.h"
#include "chat_room.h"

using boost::asio::ip::tcp;

class chat_server
{
public:
    chat_server(boost::asio::io_service& io_service,
                const tcp::endpoint& endpoint):
            acceptor_(io_service, endpoint), socket_(io_service)
    {
        do_accept();
    }

private:
    void do_accept();

  tcp::acceptor acceptor_;
  tcp::socket socket_;
  chat_room room_;
};

#endif /* SERVER_INCLUDE_CHAT_SERVER_H_ */
