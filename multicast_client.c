/* Receiver/client multicast Datagram example. */
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
 
struct sockaddr_in localSock;
struct ip_mreq group;
int sd;
int datalen;
char databuf[1024];

// calculate file size
int getFileSize(char *type,long file){
    if(!strcmp(type,"Bytes")){
        return file;
    }
    if(!strcmp(type,"KB")){
        return file / 1024;
    }
    if(!strcmp(type,"MB")){
        return file / (1024*1024);
    }
    if(!strcmp(type,"GB")){
        return file / (1024*1024*1024);
    }
    return file;
}
// choose file size type
char *getFileType(long size){
    if(size / 1024 == 0){
        return "Bytes";
    }
    if(size / (1024*1024) == 0){
        return "KB";
    }
    if(size / (1024*1024*1024) == 0){
        return "MB";
    }
    if(size > (1024*1024*1024)){
        return "GB";
    }
    return "Bytes";
}

int main(int argc, char *argv[])
{
/* Create a datagram socket on which to receive. */
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sd < 0)
	{
		perror("Opening datagram socket error");
		exit(1);
	}
	else
	printf("Opening datagram socket....OK.\n");
		 
	/* Enable SO_REUSEADDR to allow multiple instances of this */
	/* application to receive copies of the multicast datagrams. */
	
	int reuse = 1;
	if(setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0)
	{
		perror("Setting SO_REUSEADDR error");
		close(sd);
		exit(1);
	}
	else
		printf("Setting SO_REUSEADDR...OK.\n");
	
	 
	/* Bind to the proper port number with the IP address */
	/* specified as INADDR_ANY. */
	memset((char *) &localSock, 0, sizeof(localSock));
	localSock.sin_family = AF_INET;
	localSock.sin_port = htons(4321);
	localSock.sin_addr.s_addr = INADDR_ANY;
	if(bind(sd, (struct sockaddr*)&localSock, sizeof(localSock)))
	{
		perror("Binding datagram socket error");
		close(sd);
		exit(1);
	}
	else
		printf("Binding datagram socket...OK.\n");
	 
	/* Join the multicast group 226.1.1.1 on the local address*/
	/* interface. Note that this IP_ADD_MEMBERSHIP option must be */
	/* called for each local interface over which the multicast */
	/* datagrams are to be received. */
	group.imr_multiaddr.s_addr = inet_addr("226.1.1.1");
	/* your ip address */ 
	group.imr_interface.s_addr = inet_addr("10.0.2.15"); 
	/* IP_ADD_MEMBERSHIP:  Joins the multicast group specified */ 
	if(setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < 0)
	{
		perror("Adding multicast group error");
		close(sd);
		exit(1);
	}
	else
		printf("Adding multicast group...OK.\n");
	 
	/* Read from the socket. */
	// Receive file
    FILE *fp = fopen("receive.txt","wb");
    long numbytes, receivebytes = 0;
    char buffer[1000];

	// keep receive file , until receive "end" from sender
    while (1){
        memset(buffer, 0, sizeof(buffer)); // buffer 歸0
        numbytes = recvfrom(sd, buffer, sizeof(buffer), 0,NULL, NULL);
        // break when received "end" message
        if(!strcmp(buffer,"end")){
            break;
        }
		// write into file
        numbytes = fwrite(buffer, sizeof(char), numbytes, fp);
        receivebytes += numbytes;
        
    }
    fclose(fp);

	// print message
	printf("Reading datagram message...OK.\n");
	printf("receive file size : %d %s.\n", getFileSize(getFileType(receivebytes),receivebytes), getFileType(receivebytes));

	// close socket
	close(sd);
	return 0;
}
