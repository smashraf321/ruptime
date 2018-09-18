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

 int z, sock, n;
 struct sockaddr_in client;
 struct hostent* cmd_addr;
 
 if(argc == 1)
 {
	 fprintf(stderr,"\n Please type in the IP addresses or host names of the servers on the command line itself \n");
	 return -1;
 }

 char r_uptime[80] = " ";
 char server_name[80] = " ";
 n=0;
 
 client.sin_family = AF_INET;
 client.sin_port = htons(14085); 
 
 for(z = 1; z < argc; z++)
 {
	 
	 sock = socket(AF_INET, SOCK_STREAM, 0);
 
	 if (sock < 0)
		{
			 perror("\n socket() failed \n");
			 return -1;
		}
	 
	 cmd_addr = gethostbyname(argv[z]);
	 
	 if(!(cmd_addr == NULL))
	 {
		 client.sin_addr = *((struct in_addr*) (cmd_addr -> h_addr_list[0]));
	 }
	 else
	 {
		 inet_aton(argv[z], &client.sin_addr);	 
	 }
	 
	 if(connect(sock,(struct sockaddr*) &client, sizeof(client)) < 0)
	 {
		 perror("\n connect() failed \n");
		 return -1;
	 }
	 
	 if ( (n = read(sock, r_uptime, 80)) < 0)
	 {
		 perror("\n read() failed \n");
		 return -1;
	 }	 
	 
	 if(getnameinfo((struct sockaddr*) &client, sizeof(client), server_name, 80, NULL, 0, 0 )==0)
	 {
		 printf("\n Received from Server: %s \n",server_name);
	 }
	 printf("\n Uptime message of size %d on server %s is:\n %s \n",n,argv[z],r_uptime);	

	 close(sock);	 
 }
 
 return 0;
}
