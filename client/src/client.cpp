#include "client.h"
#include "chat_client.h"

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 4)
        {
            std::cerr << "Usage: chat_client <name> <host> <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        auto endpoint_iterator = resolver.resolve({ argv[2], argv[3] });
        chat_client c(io_service, endpoint_iterator, argv[1]);

        std::thread t([&io_service](){ io_service.run(); });

        chat_message login_msg("authorization", c.get_client_name(),
                c.get_client_name(), "authorized");
        c.write(login_msg);

        char line[chat_message::MSG_LENGTH / 2];
        while (std::cin.getline(line, chat_message::MSG_LENGTH / 2 - 1))
        {
            char *line_body = strchr(line, ':');
            if (!line_body)
                continue;

            std::string to(line, line_body - line);
            line_body++;

            chat_message msg("regular", c.get_client_name(), to, line_body);
            c.write(msg);
        }

        c.close();
        t.join();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
