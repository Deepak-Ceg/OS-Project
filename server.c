#include<string.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <unistd.h>
#include <strings.h>

#define BUFFERT 512
#define BACKLOG 1


int duration (struct timeval *start,struct timeval *stop, struct timeval *delta);
int create_server_socket (int port);

struct sockaddr_in sock_serv,sock_clt;


int main(int argc,char** argv){
	int sfd,fd;
	unsigned int length=sizeof(struct sockaddr_in);
	long int n, m,count=0;
	unsigned int nsid;
	ushort clt_port;
	char buffer[BUFFERT],filename[256];
	char dst[INET_ADDRSTRLEN];

	
	time_t intps;
	struct tm* tmi;

	if(argc!=2) {
		perror("utilisation ./a.out <num_port> <file2send>\n");
		exit(3);
	}

	sfd = create_server_socket(atoi(argv[1]));

	bzero(buffer,BUFFERT);
	listen(sfd,BACKLOG);

	
	nsid=accept(sfd,(struct sockaddr*)&sock_clt,&length);
	if(nsid==-1){
		perror("accept fail");
		return EXIT_FAILURE;
	}
	else {
		if(inet_ntop(AF_INET,&sock_clt.sin_addr,dst,INET_ADDRSTRLEN)==NULL){
			perror("erreur socket");
			exit (4);
		}
		clt_port=ntohs(sock_clt.sin_port);
		printf("Start of the connection for : %s:%d\n",dst,clt_port);

		bzero(filename,256);
		int r= recv(nsid,filename,100,0);
		printf("Creating output file : %s\n",filename);

		if ((fd=open(filename,O_CREAT|O_WRONLY,0600))==-1)
		{
			perror("open fail");
			exit (3);
		}
		bzero(buffer,BUFFERT);
		n=recv(nsid,buffer,BUFFERT,0);
		while(n) {
			if(n==-1){
				perror("recv fail");
				exit(5);
			}
			if((m=write(fd,buffer,n))==-1){
				perror("write fail");
				exit (6);
			}
			count=count+m;
			bzero(buffer,BUFFERT);
			n=recv(nsid,buffer,BUFFERT,0);
		}
		close(sfd);
		close(fd);
	}
	close(nsid);

	printf("End of transmission with %s.%d\n",dst,clt_port);
	printf("Number of bytes received : %ld \n",count);
	int i,j,len=strlen(filename);
	char *final;
	for(i=len-1;i>=0;i--)
	{
		if(filename[i]=='.'){
			final = (char *)malloc(sizeof(char) * (len-i));	
			for(j=i+1;j<len;j++){
				final[j-i-1]=filename[j];
			}
			final[len-i-1]='\0';
			break;
		}
	}

	if(strcmp(final,"c")==0){
		char execu[50]="gcc ";
		strcat(execu,filename);
		strcat(execu," -o output");
		system(execu);

		printf("\nOutput File has been generated Succesfully!");
	}
	else if(strcmp(final,"cpp")==0)
	{

		char execu[50]="g++ ";
		strcat(execu,filename);
		strcat(execu," -o output");
		system(execu);

		printf("\nOutput File has been generated Succesfully!");
	}	

	return EXIT_SUCCESS;
}

/* Function allowing the creation of a socket and its attachment to the system
 * Returns a file descriptor in the process descriptor table
 * bind allows its definition in the system
 */
int create_server_socket (int port){
	int l;
	int sfd;
	int yes=1;

	sfd = socket(PF_INET,SOCK_STREAM,0);
	if (sfd == -1){
		perror("socket fail");
		return EXIT_SUCCESS;
	}
	
/* SOL_SOCKET: To manipulate options at the API level sockets
* SO_REUSEADDR: When you have to restart a server after a crash it can serve
* to not have an error when creating the socket (the IP stack of the system does not have
* always had time to clean).
* When multiple servers listening to the same port ... (?)
*/

	if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR,&yes,sizeof(int)) == -1 ) {
		perror("setsockopt erreur");
		exit(5);
	}


	// prepare the address of the destination socket
	l=sizeof(struct sockaddr_in);
	bzero(&sock_serv,l);

	sock_serv.sin_family=AF_INET;
	sock_serv.sin_port=htons(port);
	sock_serv.sin_addr.s_addr=htonl(INADDR_ANY);

	// Assign an identity to the socket

	if(bind(sfd,(struct sockaddr*)&sock_serv,l)==-1){
		perror("bind fail");
		return EXIT_FAILURE;
	}   
	return sfd;
}
