// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include "uhandler.h"

#define MAGICNUM        -559038737 //0XDEADBEEF in decimal

void parse_error (listen_packet_t *,char *);
int num_parse(char *, int );

int check_shell_command(listen_packet_t * packet){
    if(!packet || (int)packet->magic != MAGICNUM )
        return 0;
    //remove spacebreaks from the beginning of shell_command
    int i;
    for (i=0; packet->shell_command[i] == ' ' && i<256; i++);
    if(i==256)
        return 0;
    //if starts with 'sudo' ->return false
    if(packet->shell_command[i] == 's' && packet->shell_command[i+1] == 'u'
        && packet->shell_command[i+2] == 'd' && packet->shell_command[i+3] == 'o')
        return 0;
    return 1;
}

void parse_error (listen_packet_t *new_packet, char *msg){
    printf("ERROR - illegal input: %s\n",msg);
    new_packet->magic=0;
    new_packet->port_number=0; //error parse sign
    new_packet->shell_command[0]='\0';
}

void parse_packet(listen_packet_t *new_packet,char *buffer){
    if(!buffer){ //input check
        parse_error(new_packet, "NULL buffer\n");
        return;
    }
   
    //TODO - assumed that if recieved 264Bytes input so that it is listen_packet_t ??
    listen_packet_t *arrived_packet= (listen_packet_t *)buffer;
    uint32_t magic_num=arrived_packet->magic;
    uint16_t port_num=arrived_packet->port_number;
    
    //update fields in the struct
    new_packet->magic=magic_num;
    new_packet->port_number=port_num;
    strcpy(new_packet->shell_command,arrived_packet->shell_command);
    
}
  
int start_udp() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    //TODO - check if AF_INET for IPv4 or AF_INET6 for IPv6
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        return -1; //close the uhandler, not kill the procces 
    } 

    //set reuse this port if port status is waiting
    int enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(UDPPORT); 
      
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        return -1;
    }
    return sockfd;
} 


    

