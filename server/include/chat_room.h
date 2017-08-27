#ifndef SERVER_INCLUDE_CHAT_ROOM_H_
#define SERVER_INCLUDE_CHAT_ROOM_H_

#include "server.h"
#include "chat_participant.h"
#include "chat_message.h"

using boost::asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

class chat_room
{
public:
    void join(chat_participant_ptr participant);
    void first_deliver(chat_participant_ptr participant);
    void leave(chat_participant_ptr participant);
    void deliver(const chat_message& msg);

private:
    std::set<chat_participant_ptr> participants_;
    enum { max_recent_msgs = 100 };
    chat_message_queue recent_msgs_;
};

#endif /* SERVER_INCLUDE_CHAT_ROOM_H_ */
