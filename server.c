#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{

 FILE* p;
 int sock, acc, client_len;
 struct sockaddr_in server, client;
 struct hostent* cmd_addr;

 if(argc == 1)
 {
	 fprintf(stderr,"\n Please enter the IP address or host name of the server, i.e the PC you're working on right now \n");
	 return -1;
 }

 char uptime_msg[80] = " ";
 
 client_len = sizeof(client);
 
 server.sin_family = AF_INET;
 server.sin_port = htons(14085);
 
 cmd_addr = gethostbyname(argv[1]);
 
 if(!(cmd_addr == NULL))
	 {
		 printf("\n host name/ip address is: %s \n",inet_ntoa(*((struct in_addr*) cmd_addr -> h_addr_list[0])));
		 server.sin_addr = *((struct in_addr*) (cmd_addr -> h_addr_list[0]));
	 }
	 else
	 {
		 inet_aton(argv[1], &server.sin_addr);	 
		 printf("\n IP number was entered ");
	 }
 
 sock = socket(AF_INET, SOCK_STREAM, 0);
 
 if(sock < 0)
 {
	 perror("\n socket() failed \n");
	 return -1;
 }
 
 if(bind(sock, (struct sockaddr*) &server, sizeof(server)) < 0)
 {
	 perror("\n bind() failed \n");
	 return -1;
 }
 
 if(listen(sock, 10) < 0 )
 {
	 perror("\n listen() failed \n");
	 return -1;
 }
 
 while(1)
 {
   	 acc = accept(sock, (struct sockaddr*) &client, (socklen_t *) &client_len);
	 
	 printf("\n accepted the client's connection \n");
	 
	 p = popen("uptime","r");
	 
	 printf("\n streaming from the pipe to process uptime \n");
	 
	 fgets(uptime_msg,80,p);
	 
	 printf("\n uptime is:\n %s \n", uptime_msg);
	 
	 write(acc, uptime_msg, 80 );
	 
	 printf("\n sent uptime to client \n");
	 
	 pclose(p);
	 
	 close(acc);
 }
 
 close(sock);

 return 0;
}
