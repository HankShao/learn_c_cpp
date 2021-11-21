/*
* FILE: client.c
* NOTE: socket网络编程学习，Client端程序
*
* TIME: 2021年11月12日23:32:53
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define   PORT  1234
#define   BUFFER_SIZE   1024
#define   MAX_QUE_CONN_NM   5

int main(int argc, char *argv[])
{
    struct sockaddr_in serv_addr;
	int sendBytes;
	int sockfd, client_fd;
    char buf[BUFFER_SIZE];
    struct hostent *host;

	/* 地址解析函数 */
	host = gethostbyname(argv[1]);
	if (host == NULL)
	{
        perror("gethostbyname");
		return -1;
	}

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%s", argv[2]);

	/* 创建socket */
	if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
        perror("socket");
		return -1;
	}

    printf("socket id = %d\n", sockfd);

	/* 设置sockaddr_in 结构体参数 */
	serv_addr.sin_family = AF_INET; //IPv4
	serv_addr.sin_port = htons(PORT); //端口号
	serv_addr.sin_addr = *((struct in_addr *)host->h_addr);
    bzero(&(serv_addr.sin_zero), 8);

	/* 调用 connect 函数主动发起对服务器端的连接 */
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) == -1)
	{
        perror("connect");
		return -1;
	}

	/* 发送消息给服务器端 */
	sendBytes = send(sockfd, buf, strlen(buf), 0);
    if (sendBytes == -1)
    {
        perror("send");
		return -1;
	}
	
    printf("Send a message:%s \n", buf);

	/* 关闭socket */
	close(sockfd);
	
	return 0;
}



