void start_udp(void);

typedef struct {
	uint32_t magic; /* should be 0xDEADBEEF */
	uint16_t port_number;
    char shell_command[256];
} listen_packet_t;
