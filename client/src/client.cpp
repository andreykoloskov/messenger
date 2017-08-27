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

    chat_message login_msg;
    login_msg.set_from(c.get_client_name());
    login_msg.set_to(c.get_client_name());
    login_msg.set_type("authorization");
    login_msg.body_length(std::strlen("authorized"));
    std::memcpy(login_msg.body(), "authorized", login_msg.body_length());
    login_msg.encode_header();
    c.write(login_msg);

    char line[chat_message::max_body_length + 1];
    while (std::cin.getline(line, chat_message::max_body_length + 1))
    {
      chat_message msg;
      msg.set_from(c.get_client_name());

      char *line_body = strchr(line, ':');
      if (!line_body)
          continue;

      std::string to(line, line_body - line);
      msg.set_to(to);
      line_body++;

      msg.set_type("regular");
      msg.body_length(std::strlen(line_body));
      std::memcpy(msg.body(), line_body, msg.body_length());
      msg.encode_header();
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
