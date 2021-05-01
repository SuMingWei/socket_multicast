/* Send Multicast Datagram code example. */
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
struct in_addr localInterface;
struct sockaddr_in groupSock;
int sd;

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

int main (int argc, char *argv[ ])
{
/* Create a datagram socket on which to send. */
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sd < 0)
	{
	  perror("Opening datagram socket error");
	  exit(1);
	}
	else
	  printf("Opening the datagram socket...OK.\n");
	 
	/* Initialize the group sockaddr structure with a */
	/* group address of 226.1.1.1 and port 4321. */
	memset((char *) &groupSock, 0, sizeof(groupSock));
	groupSock.sin_family = AF_INET;
	groupSock.sin_addr.s_addr = inet_addr("226.1.1.1");
	groupSock.sin_port = htons(4321);
	 
	/* Set local interface for outbound multicast datagrams. */
	/* The IP address specified must be associated with a local, */
	/* multicast capable interface. */
	localInterface.s_addr = inet_addr("10.0.2.15");
	
	/* IP_MULTICAST_IF:  Sets the interface over which outgoing multicast datagrams are sent. */
	if(setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface)) < 0)
	{
	  perror("Setting local interface error");
	  exit(1);
	}
	else
	  printf("Setting the local interface...OK.\n");
	/* Send a file to the multicast group specified by the*/
	/* groupSock sockaddr structure. */

	// filesize
	struct stat st;
    stat(argv[1],&st);
    long filesize = st.st_size;

	// open file 
	FILE *fp = fopen(argv[1],"rb");
	long numbytes;
    char buffer[1000];

	// send file
    while (!feof(fp)){
        numbytes = fread(buffer, sizeof(char), sizeof(buffer), fp);
        sendto(sd, buffer,numbytes,0,(struct sockaddr *)&groupSock, sizeof(groupSock));
    }
    fclose(fp);

	// send end message (avoid loss the packet)
    char endbuf[3] = {'e','n','d'};
    sendto(sd, endbuf, sizeof(endbuf), 0, (struct sockaddr *)&groupSock, sizeof(groupSock));
	sendto(sd, endbuf, sizeof(endbuf), 0, (struct sockaddr *)&groupSock, sizeof(groupSock));
	sendto(sd, endbuf, sizeof(endbuf), 0, (struct sockaddr *)&groupSock, sizeof(groupSock));
	
	// print message
	printf("Sending datagram message...OK.\n");
	printf("file size : %d %s.\n", getFileSize(getFileType(filesize),filesize), getFileType(filesize));
    
	// close socket
	close(sd);
	return 0;
}
