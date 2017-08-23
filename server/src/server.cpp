#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
using namespace boost::asio;
using namespace boost::posix_time;

class talk_to_client : public boost::enable_shared_from_this<talk_to_client>,
		boost::noncopyable
{
public:
    typedef boost::system::error_code error_code;
    typedef boost::shared_ptr<talk_to_client> ptr;

    void start()
    {
        started_ = true;
        do_read();
    }

    static ptr new_(io_service *service)
    {
        ptr new_(new talk_to_client(service));
        return new_;
    }

    void stop()
    {
        if ( !started_) return;
        started_ = false;
        sock_.close();
    }

    ip::tcp::socket & sock() { return sock_;}

private:
    typedef talk_to_client self_type;

    ip::tcp::socket sock_;
    enum { max_msg = 1024 };
    char read_buffer_[max_msg];
    char write_buffer_[max_msg];
    bool started_;

    talk_to_client(io_service *service) : sock_(*service), started_(false) {}

    void on_read(const error_code & err, size_t bytes)
    {
        if ( !err)
        {
            std::string msg(read_buffer_, bytes);
            // echo message back, and then stop
            do_write(msg + "\n");
        }
        stop();
    }

    void on_write(const error_code & err, size_t bytes)
    {
        do_read();
    }

    void do_read()
    {
        async_read(sock_, buffer(read_buffer_),
                boost::bind(&self_type::read_complete, shared_from_this(),
                        _1, _2),
                boost::bind(&self_type::on_read, shared_from_this(),_1, _2));
    }

    void do_write(const std::string & msg)
    {
        std::copy(msg.begin(), msg.end(), write_buffer_);
        sock_.async_write_some(buffer(write_buffer_, msg.size()),
                boost::bind(&self_type::on_write, shared_from_this(),_1 ,_2));
    }
    size_t read_complete(const boost::system::error_code & err, size_t bytes)
    {
        if ( err) return 0;
        bool found = std::find(read_buffer_, read_buffer_ + bytes, '\n') <
                read_buffer_ + bytes;
        // we read one-by-one until we get to enter, no buffering
        return found ? 0 : 1;
    }
};

void handle_accept(talk_to_client::ptr client, ip::tcp::acceptor *acceptor,
				   io_service *service,
				   const boost::system::error_code & err)
{
    client->start();
    talk_to_client::ptr new_client = talk_to_client::new_(service);
    acceptor->async_accept(new_client->sock(), boost::bind(handle_accept,
    			new_client, acceptor, service, _1));
}

int
main(int argc, char* argv[])
{
	if (argc < 2)
		std::cout << "bad arguments" << std::endl;

	io_service service;
	ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(),
			atoi(argv[1])));

	talk_to_client::ptr client = talk_to_client::new_(&service);
    acceptor.async_accept(client->sock(), boost::bind(handle_accept,
    			client, &acceptor, &service, _1));
    service.run();
}
