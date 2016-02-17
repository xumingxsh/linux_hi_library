#include "test.h"
#include "net/hiTcpListen.h"
#include "net/hiTcpEPoll.h"
#include "net/hiTcpServer.h"
#include "net/hiByteMsgHandle.h"
#include "net/hiTcpConnect.h"
#include "net/hiTcpClient.h"
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <vector>
#include <map>
#ifdef LINUX_
#include "pthread.h"
#endif
#include <sys/epoll.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

extern void write_log(const char* fmt, ...);

using namespace std;
using namespace std::placeholders;

const char* server_ip = "192.168.10.1";
const char* my_ip = "192.168.10.130";
const unsigned short my_listen_port = 6000;

static void test_hi_listen();
static void test_hi_listen_close();
static void test_hi_epoll();
static void test_hi_epoll_close();
static void test_hi_server();
static void test_hi_connect();
static void test_hi_net_client();

void test_main()
{
	//TEST_FUN("HiTcpListen with close function", test_hi_listen_close);
	//TEST_FUN("HiTcpListen", test_hi_listen);
	//TEST_FUN("HiTcpEPoll with close function", test_hi_epoll_close);
	//TEST_FUN("HiTcpEPoll", test_hi_epoll);
	//TEST_FUN("HiTcpServer", test_hi_server);
	//TEST_FUN("HiTcpConnect", test_hi_connect);
	TEST_FUN("HiTcpClient", test_hi_net_client);
}

void test_hi_listen_close()
{
	vector<int> socks;
	
	Hi::HiTcpListen listen;
	
	bool result = false;
	while (!result)
	{
		listen.set_close_fun([&](){
			for (auto it = socks.begin(); it != socks.end(); ++it)
			{
				close(*it);
				sleep(1);
				write_log("close client connect sock[%d]", *it);
			}
			socks.clear();
			write_log("close listen thread");
		});
		result = listen.open([&](int sock){
			
			write_log("accept an connect remote socket[%d]",
				sock);
			
			socks.push_back(sock);
		}, my_listen_port);
		
		if (!result)
		{
			write_log("open listen failed");
			sleep(3);
		}
	}
	
	write_log("open listen success");
	sleep(10);
	listen.close();
	
}

void test_hi_listen()
{	
	vector<int> socks;
	Hi::HiTcpListen listen;
	bool result = false;
	while (!result)
	{	
		result = listen.open([&](int sock){
			
			write_log("accept an connect remote socket[%d]",
				sock);
				
			socks.push_back(sock);
		}, my_listen_port, my_ip);
		if (!result)
		{
			write_log("open listen failed");
			sleep(3);
		}
	}
	
	write_log("open listen success");
	sleep(10);
	listen.close();
	for (auto it = socks.begin(); it != socks.end(); ++it)
	{
		close(*it);
		write_log("close sock[%d]", *it);
	}
	socks.clear();
	write_log("close listen thread");
}

void test_hi_epoll()
{
	Hi::HiTcpEPoll poll;
	poll.start([&](int sock, unsigned int evt){
		write_log("sock[%d] event:%d", sock, evt);
		if ((evt & EPOLLRDHUP) || (evt & EPOLLHUP))
		{
			poll.remove(sock);
			write_log("close sock[%d]", sock);
		}
		if ((evt & EPOLLIN) || (evt & EPOLLPRI))
		{
			char rece_buf[256];
			memset(rece_buf, 0, 256);
			int n = recv(sock, rece_buf, 256, 0);
			write_log("sock [%d] receive byte:%d", sock, n);
		}
	});
	
	vector<int> socks;
	Hi::HiTcpListen listen;	
	
	bool result = false;
	while (!result)
	{
		listen.set_close_fun([&](){
			for (auto it = socks.begin(); it != socks.end(); ++it)
			{
				close(*it);
				write_log("close sock[%d]", *it);
			}
			socks.clear();
			write_log("close listen thread");
		});
		
		result = listen.open([&](int sock){
			
			write_log("accept an connect remote socket[%d]",
				sock);
		
			if (poll.add(sock, [&](int sock, unsigned int evt){				
				if ((evt & EPOLLRDHUP) || (evt & EPOLLHUP))
				{
					poll.remove(sock);
					write_log(":::close sock[%d]", sock);
				}
				if ((evt & EPOLLIN) || (evt & EPOLLPRI))
				{
					char rece_buf[256];
					memset(rece_buf, 0, 256);
					int n = recv(sock, rece_buf, 256, 0);
					write_log(":::sock [%d] receive byte:%d", sock, n);
				}
			}))
			{
				write_log("add sock[%d] to epoll success", sock);
			}
			else
			{
				write_log("add sock[%d] to epoll fail", sock);
			}
		}, my_listen_port);
		
		if (!result)
		{
			write_log("open listen failed");
			sleep(3);
		}
	}
	
	write_log("open listen success");
	sleep(10);
	listen.close();
	poll.finish();
}

void test_hi_epoll_close()
{
	Hi::HiTcpEPoll poll;
	bool result = poll.start([&](int sock, unsigned int sign){
		write_log("sock[%d] event:%d", sock, sign);
		if ((sign & EPOLLRDHUP) || (sign & EPOLLHUP))
		{
			poll.remove(sock);
			write_log("close sock[%d]", sock);
		}
		if ((sign & EPOLLIN) || (sign & EPOLLPRI))
		{
			char rece_buf[256];
			memset(rece_buf, 0, 256);
			int n = recv(sock, rece_buf, 256, 0);
			write_log("receive data byte:%d", n);
		}
	}, [&](){
		write_log("epoll is closed");
	});
	
	if (result)
	{
		write_log("start epoll success");
		sleep(10);
		poll.finish();
	}
	else
	{
		write_log("start epoll failed");
	}
}

void test_hi_server()
{
	Hi::TcpEvent evt;
	map<int, Hi::HiByteMsgHandle*> msg_handles;
	evt.on_open_ = [&](int sock) {
		
		write_log("accept an connect remote socket[%d]",
			sock);
			
		Hi::HiByteMsgHandle* handle = new Hi::HiByteMsgHandle(sock, 
			[&](Hi::HiByteMsg_Ptr msg){				
				write_log("recevice len:%d", msg->len());
			});
		msg_handles[sock] = handle;
	};
	evt.on_receive_ = [&](int sock) {	
		/*
		char rece_buf[256];
		memset(rece_buf, 0, 256);
		int n = recv(sock, rece_buf, 256, 0);
		write_log("sock ["<<sock<<"] receive byte:"<<n);
		::write(sock, rece_buf, n);*/
		
		write_log("on_receive_");
		auto it = msg_handles.find(sock);
		if (it == msg_handles.end())
		{
			write_log("receive a msg,but not handle");
		}
		if (!it->second->read())
		{
			write_log("receive failed");
		}
	};
	evt.on_close_ = [&](int sock) {	
		write_log("sock [%d] closed", sock);
		auto it = msg_handles.find(sock);
		if (it != msg_handles.end())
		{
			delete it->second;
		}
		msg_handles.erase(it);
		write_log("msg_handles.erase");
	};
	Hi::HiTcpServer server;
	bool result = server.open(evt, my_listen_port);
	if (!result)
	{	
		write_log("tcp server open failed");
		return;
	}
	write_log("tcp server open success");
	sleep(10);
	cout<<"server.close begin"<<endl;
	server.close();
	cout<<"server.close end"<<endl;
}

static void on_conn_open(bool is_ok, int sock, void* user_data)
{
	write_log("connect to server remote socket[%d] result:%d",
		sock, is_ok);
		
	if (is_ok)
	{
		char* buf = "hthis is a test";
		::write(sock, buf, ::strlen(buf));
	}
	else
	{
		write_log("connect failed");
	}
}

static void on_conn_event(int sock, unsigned int sign)
{
	write_log("sock[%d] get a event:%d", sock, sign);
}

void test_hi_connect()
{
	Hi::HiTcpEPoll poll;
	bool result = poll.start([&](int sock, unsigned int sign){
		write_log("sock[%d] event:%d", sock, sign);
	}, [&](){
		write_log("epoll is closed");
	});
	
	if (result)
	{
		write_log("start epoll success");
	}
	else
	{
		write_log("start epoll failed");
	}
	
	Hi::HiTcpConnect connect(poll);
	auto fun = std::bind(&on_conn_open, _1, _2, _3);
	auto onevt = std::bind(&on_conn_event, _1, _2);
	const char* ip2 = "127.0.0.1";
	const char* ip3 = server_ip;
	const char* ip1 = "172.16.5.154";
	unsigned short port = 6000;
	
	if (!connect.connect(ip1, 6001, fun, onevt, 8))
	{
		write_log("connect %s failed", ip1);
	}
	else
	{
		write_log("connect %s is send", ip1);
		//::send();
	}
	
	sleep(10);
	connect.close();
	
	if (!connect.connect(ip2, port, fun, onevt, 8))
	{
		write_log("connect %s failed", ip2);
	}
	else
	{
		write_log("connect %s is send", ip2);
	}
	sleep(10);
	connect.close();
	
	if (!connect.connect(ip3, port, fun, onevt, 8))
	{
		write_log("connect %s failed", ip3);
	}
	else
	{
		write_log("connect %s is send", ip3);
	}
	sleep(10);
	connect.close();
	poll.finish();
}

void test_hi_net_client()
{	
	Hi::TcpClientEvt evt([&](bool is_ok, int sock, void* user_data){		
		write_log("client connect to server remote socket[%d]result:%d",
			sock, is_ok);
			
		if (is_ok)
		{
			char* buf = "hthis is a test";
			::write(sock, buf, ::strlen(buf));
		}
		else
		{
			write_log("connect failed");
		}
	}, [&](int sock){		
		write_log("get a receive event");
		char rece_buf[256];
		memset(rece_buf, 0, 256);
		int n = recv(sock, rece_buf, 256, 0);
		write_log("sock [%d] receive byte:%d", sock, n);
		::write(sock, rece_buf, ::strlen(rece_buf));
	},[&](int sock){
		write_log("sock [%d] is closed", sock);
	});
	
	const char* ip2 = "127.0.0.1";
	const char* ip3 = server_ip;
	const char* ip1 = "172.16.5.154";
	unsigned short port = 6000;
	
	Hi::HiTcpClient client2;
	if (!client2.connect(ip3, port, evt, 8))
	{
		write_log("connect %s failed", ip3);
	}
	else
	{
		write_log("connect %s is send", ip3);
	}
	sleep(10);
	client2.close();
	
	
	Hi::HiTcpEPoll poll;
	bool result = poll.start();
	
	if (result)
	{
		write_log("start epoll success");
	}
	else
	{
		write_log("start epoll failed");
	}
	
	Hi::HiTcpClient client(poll);
	
	if (!client.connect(ip1, 6001, evt, 8))
	{
		write_log("connect %s failed", ip1);
	}
	else
	{
		write_log("connect %s is send", ip1);
		//::send();
	}
	
	sleep(10);
	client.close();
	
	if (!client.connect(ip2, port, evt, 8))
	{
		write_log("connect %s failed", ip2);
	}
	else
	{
		write_log("connect %s is send", ip2);
	}
	sleep(10);
	client.close();
	
	if (!client.connect(ip3, port, evt, 8))
	{
		write_log("connect %s failed", ip3);
	}
	else
	{
		write_log("connect %s is send", ip3);
	}
	sleep(10);
	client.close();
	poll.finish();
}