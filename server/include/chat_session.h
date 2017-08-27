#ifndef SERVER_INCLUDE_CHAT_SESSION_H_
#define SERVER_INCLUDE_CHAT_SESSION_H_

#include "server.h"
#include "chat_room.h"
#include "chat_message.h"

using boost::asio::ip::tcp;

class chat_session:
        public chat_participant,
        public std::enable_shared_from_this<chat_session>
{
public:
    chat_session(tcp::socket socket, chat_room& room);
    void start();
    void deliver(const chat_message& msg);

private:
    void do_read_header();
    void do_read_body();
    void do_write();

  tcp::socket socket_;
  chat_room& room_;
  chat_message read_msg_;
  chat_message_queue write_msgs_;
  std::string login_;
};

#endif /* SERVER_INCLUDE_CHAT_SESSION_H_ */
