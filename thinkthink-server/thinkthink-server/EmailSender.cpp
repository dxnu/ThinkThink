#include "EmailSender.h"
#include "okcrypt.h"
#include <iostream>
#pragma warning(disable:4996)


void EmailSender::Connect(const char* addr, const char* port)
{
	tcp::resolver r(m_io);
	tcp::resolver::query q(addr, port);
	auto iter = r.resolve(q);
	m_sock.connect(*iter);
	std::cout  << "connect succeeded!\n";
}


void EmailSender::SendEmail(const char* sender, const char* verifyCode, const char* receiver, const char* subject, const char* content)
{
	char data[1024];
	memset(data, 0, sizeof(data));

	// 连接成功后，服务器返回的消息
	m_sock.read_some(buffer(data));
	std::cout << data;

	strcpy(data, "helo qq\r\n");
	m_sock.write_some(buffer(data, strlen(data)));
	std::cout << data;
	
	memset(data, 0, sizeof(data));
	m_sock.read_some(buffer(data));
	std::cout << data;
	strcpy(data, "auth login\r\n");
	m_sock.write_some(buffer(data, strlen(data)));
	std::cout << data;

	memset(data, 0, sizeof(data));
	m_sock.read_some(buffer(data));
	std::cout << data;

	// 发送base64加密的用户名与密码
	std::string cipherSender = okcrypt::EncryptBase64(sender);
	cipherSender.append("\r\n");
	m_sock.write_some(buffer(cipherSender, cipherSender.size()));
	std::cout << cipherSender;

	memset(data, 0, sizeof(data));
	m_sock.read_some(buffer(data));
	std::cout << data;

	std::string cipherVerifyCode = okcrypt::EncryptBase64(verifyCode);
	cipherVerifyCode.append("\r\n");
	m_sock.write_some(buffer(cipherVerifyCode, cipherVerifyCode.size()));
	std::cout << cipherVerifyCode;

	memset(data, 0, sizeof(data));
	m_sock.read_some(buffer(data));
	std::cout << data;

	strcpy(data, "mail from:<");
	strcat(data, sender);
	strcat(data, ">\r\n");
	m_sock.write_some(buffer(data, strlen(data)));
	std::cout << data;

	memset(data, 0, sizeof(data));
	m_sock.read_some(buffer(data));
	std::cout << data;

	strcpy(data, "rcpt to:<");
	strcat(data, receiver);
	strcat(data, ">\r\n");
	m_sock.write_some(buffer(data, strlen(data)));
	std::cout << data;

	memset(data, 0, sizeof(data));
	m_sock.read_some(buffer(data));
	std::cout << data;

	// 开始输入内容
	strcpy(data, "data\r\n");
	m_sock.write_some(buffer(data, strlen(data)));
	std::cout << data;

	memset(data, 0, sizeof(data));
	m_sock.read_some(buffer(data));
	std::cout << data;

	strcpy(data, "subject:");
	strcat(data, subject);
	strcat(data, "\r\n\r\n");
	strcat(data, content);
	strcat(data, "\r\n.\r\n");
	m_sock.write_some(buffer(data, strlen(data)));
	std::cout << data;

	memset(data, 0, sizeof(data));
	m_sock.read_some(buffer(data));
	std::cout << data;

	std::cout << "send successful!\n";
}
