/*
* FILE: server.c
* NOTE: socket网络编程学习，server端程序
*
* TIME: 2021年11月12日23:32:53
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define   PORT  1234
#define   BUFFER_SIZE   1024
#define   MAX_QUE_CONN_NM   5

int main(int argc, char *argv[])
{
    struct sockaddr_in server_sockaddr, client_sockaddr;
	int sin_size, recvBytes;
	int sockfd, client_fd;
    char buf[BUFFER_SIZE];

    /* 创建socket */
	if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
        perror("socket");
		return -1;
	}

    printf("socket id = %d\n", sockfd);

    /* 设置sockaddr_in 结构体参数 */
	server_sockaddr.sin_family = AF_INET; //IPv4
	server_sockaddr.sin_port = htons(PORT); //端口号
	server_sockaddr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_sockaddr.sin_zero), 8);

    /* 允许重复使用本地地址与套接字进行绑定 */
	int i = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i));

	/* 绑定到端口号(TCP使用) */
	if (bind(sockfd, (struct sockaddr *)&server_sockaddr, sizeof(struct sockaddr)) == -1)
	{
        perror("bind");
		return -1;
	}
    printf("bind success!\n");

	/*调用 listen()函数，创建未处理请求的队列*/
	if (listen(sockfd, MAX_QUE_CONN_NM) == -1)
	{
        perror("listen");
		return -1;
	}
	printf("Listening...\n");

	/*调用accept(), 阻塞等待客户端的连接*/
	client_fd = accept(sockfd, (struct sockaddr *)&client_sockaddr, &sin_size);
	if (client_fd == -1)
	{
        perror("accept");
		return -1;
	}

	/*调用recv(), 阻塞接收客户端的请求*/
	memset(buf, 0, sizeof(buf));
	recvBytes = recv(client_fd, buf, BUFFER_SIZE, 0);
    if (recvBytes == -1)
    {
        perror("recv");
		return -1;
	}

    printf("Recv a message:%s \n", buf);

	/* 关闭socket */
	close(sockfd);
	
	return 0;
}


