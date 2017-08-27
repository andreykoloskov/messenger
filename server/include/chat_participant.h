#ifndef SERVER_INCLUDE_CHAT_PARTICIPANT_H_
#define SERVER_INCLUDE_CHAT_PARTICIPANT_H_

#include "server.h"
#include "chat_message.h"

class chat_participant
{
public:
    virtual ~chat_participant() {}
    virtual void deliver(const chat_message& msg) = 0;
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

#endif /* SERVER_INCLUDE_CHAT_PARTICIPANT_H_ */
