#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define ERR_EXIT(m) \
	do\
	{\
		perror(m);\
		exit(EXIT_FAILURE);\
	}while(0)

int main(void){
	int listenfd;
	if( (listenfd=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP))<0){
		ERR_EXIT("ERROR SOCKER");
	}
	//listenfd=socket(PF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(5118);
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	//servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	//inet_aton("127.0.0.1",&servaddr.sin_addr);
	
	//bind之前可以使用setsockopt设置端口的重复利用
	int on=1;
	if(setsockopt(listenfd,SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))<0)
		ERR_EXIT("SETSOCKOPT");

	if(bind(listenfd, (struct sockaddr*)&servaddr,sizeof(servaddr))<0)
		ERR_EXIT("bind"); 
	
	if(listen(listenfd, SOMAXCONN)<0)
		ERR_EXIT("listen");
	/*listen函数会将主动套接字变为被动套接字*/
	struct sockaddr_in peeraddr;
	//必须初始值，不然accept创建失败
	socklen_t peerlen=sizeof(peeraddr);
	int conn;//自动套接字
	if((conn=accept(listenfd, (struct sockaddr*)&peeraddr, &peerlen))<0)
		ERR_EXIT("ACCEPT");
	
	printf("ip=%s, port=%d\n",inet_ntoa(peeraddr.sin_addr),ntohs(peeraddr.sin_port) );
	char recvbuf[1024];
	while(1)
	{
		memset(recvbuf, 0,sizeof(recvbuf));
		int ret = read(conn, recvbuf, sizeof(recvbuf));
		fputs(recvbuf,stdout);
		write(conn, recvbuf, ret);
	}
	close(listenfd);
	close(conn);
	return 0;
}

