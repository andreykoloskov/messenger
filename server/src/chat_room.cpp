#include "server.h"
#include "chat_room.h"

using boost::asio::ip::tcp;

void
chat_room::first_deliver(chat_participant_ptr participant)
{
    for (auto msg: recent_msgs_)
        participant->deliver(msg);
}

void
chat_room::deliver(const chat_message& msg)
{
    recent_msgs_.push_back(msg);
    while (recent_msgs_.size() > max_recent_msgs)
        recent_msgs_.pop_front();

    for (auto participant: participants_)
        participant->deliver(msg);
}
