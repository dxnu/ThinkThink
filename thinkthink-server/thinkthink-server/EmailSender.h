#pragma once
#include <boost/asio.hpp>
#include <random>
#include <string>

static std::string rand_bytes(int bytes_len)
{
	typedef std::default_random_engine rng;
	static rng dre((rng::result_type)time(0));
	std::uniform_int_distribution<unsigned int> uid(0, 9);

	std::string result;
	for (int i = 0; i < bytes_len; ++i)
	{
		char temp = uid(dre) + '0';
		result += temp;
	}

	return result;
}


using boost::asio::ip::tcp;
using boost::asio::ip::address;
using boost::asio::buffer;

class EmailSender
{
public:
	void Connect(const char* addr, const char* port);
	void SendEmail(const char* sender, const char* verifyCode, const char* receiver, const char* subject, const char* content);

private:
	boost::asio::io_service m_io;
	tcp::socket m_sock{m_io};
};

