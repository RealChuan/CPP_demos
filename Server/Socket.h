#ifndef SOCKET_H
#define SOCKET_H

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

#define port 8888

int socketListenBind()
{
	int serverfd = 0;
	serverfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverfd < 0)
	{
		perror("socket Server error:");
		return -1;
	}

	int flag = 1,opt = 1;
	//int recv_size = 1024;
	//端口复用S
	setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof (opt));
	//接收缓冲区大小设置
	//setsockopt(serverfd, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<const char*> (&recv_size), sizeof (int));
	//无延时
	setsockopt(serverfd, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*> (&flag), sizeof (flag));

	//bind
	int ret=0;
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof (struct sockaddr_in));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ret = ::bind(serverfd, reinterpret_cast<struct sockaddr*>(&serverAddr),
				 static_cast<socklen_t> (sizeof (struct sockaddr_in)));
	if (ret < 0)
	{
		close(serverfd);
		perror("bind error:");
		return -1;
	}

	//listen
	ret = ::listen(serverfd, SOMAXCONN);
	if (ret < 0)
	{
		close(serverfd);
		perror("listen error:");
		return -1;
	}
	cout<<"打开服务器成功,正在监听..."<<endl;
	cout<<"服务器 :"<<inet_ntoa(serverAddr.sin_addr)<<":"<<ntohs(serverAddr.sin_port)<<endl;
	return serverfd;
}

int server_accept(int serverfd)
{
	struct sockaddr_in clientAddr;
	socklen_t addrlen = static_cast<socklen_t> (sizeof clientAddr);
	int clientfd = accept(serverfd,reinterpret_cast<struct sockaddr*>(&clientAddr),
						  &addrlen);
	return clientfd;
}

//通过fd得到ip和port
void show_info(int connfd)
{
	//	struct sockaddr_in serveraddr;
	//	bzero(&serveraddr,sizeof(serveraddr));
	//	socklen_t serveraddr_len = sizeof(serveraddr);
	//	getsockname(connfd,reinterpret_cast<struct sockaddr*>(&serveraddr),&serveraddr_len);
	//	//cout<<"serveraddr "<<inet_ntoa(serveraddr.sin_addr)<<":"<<ntohs(serveraddr.sin_port)<<endl;
	//	printf("serveraddr %s:%d\n",inet_ntoa(serveraddr.sin_addr),ntohs(serveraddr.sin_port));

	struct sockaddr_in peer_addr;
	bzero(&peer_addr,sizeof(peer_addr));
	socklen_t peer_addr_len = sizeof(peer_addr);
	getpeername(connfd,reinterpret_cast<struct sockaddr*>(&peer_addr),&peer_addr_len);
	printf("客户端 clientaddr %s:%d\n",inet_ntoa(peer_addr.sin_addr),ntohs(peer_addr.sin_port));
}

/*----------------------------------------------------------/
/对文件描述符的操作必须先使用									/
/int flags=fcntl(fd,F_GETFL,0)获取当前描述符的属性，			/
/然后在此基础上添加或者删除，否则会删除之前的属性（本来可能有多个属性）	/
/----------------------------------------------------------*/
//配置非阻塞模式
void setNonBlocking(int fd)
{
	int flags=fcntl(fd,F_GETFL,0);
	flags |=O_NONBLOCK;
	fcntl(fd,F_SETFL,flags);
}

//配置为阻塞模式
void setBlock(int fd)
{
	int flags=fcntl(fd,F_GETFL,0);
	flags &=~O_NONBLOCK;
	fcntl(fd,F_SETFL,flags);
}



#endif // SOCKET_H
