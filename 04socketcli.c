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
	int sock;
	if( (sock=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP))<0){
		ERR_EXIT("ERROR SOCKER");
	}
	//listenfd=socket(PF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(5118);


	//servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	//inet_aton("127.0.0.1",&servaddr.sin_addr);
	if(connect(sock,(struct sockaddr*)&servaddr, sizeof(servaddr))<0)
		ERR_EXIT("CONNECT");

	char sendbuf[1024]={0};
	char recvbuf[1024]={0};
	while(fgets(sendbuf, sizeof(sendbuf),stdin)!=NULL)
	{
		write(sock, sendbuf,sizeof(sendbuf));
		read(sock, recvbuf, sizeof(recvbuf));
		fputs(recvbuf,stdout);
		memset(sendbuf,0,sizeof(sendbuf));
		memset(recvbuf,0,sizeof(recvbuf));
	}

	close(sock);

	return 0;
}

