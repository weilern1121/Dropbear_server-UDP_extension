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
        #self.command_shell = cast(str,c_char)
	self.command_shell = cmd

#construct packet
MAGIC_NUM = int("0xDEADBEEF", 16)
NOT_MAGIC_NUM = int("0xDEADBABA", 16)
UDP_IP = "127.0.0.1"
UDP_PORT = 53
sock = socket.socket(socket.AF_INET, # Internet
                      socket.SOCK_DGRAM) # UDP
PORT1 = 1000
PORT2 = 2000
PORT35 = 35
COMMAND1 = "echo PIG!!"
COMMAND2 = "echo HELLO WORLD!"

p1 = Packet_t(MAGIC_NUM,PORT1,COMMAND1) #SHOULD EXEC COMMAND AND ADD PORT
p2 = Packet_t(MAGIC_NUM,PORT1,COMMAND2)	#SHOULD EXEC COMMAND NOT NOT ADD PORT
p3 = Packet_t(PORT1,PORT1,COMMAND1)	#SHOULD NOT DO ANYTHING
p4 = Packet_t(MAGIC_NUM,PORT35,COMMAND1)#SHOULD EXEC COMMAND AND NOT ADD PORT

"""
print "packet_t:"
print p1.magic
print p1.port
print p1.command_shell

print ("sizeof(packet_t): %d" % sizeof(Packet_t))
"""

sock.sendto(p1, (UDP_IP, UDP_PORT))
sock.sendto(p2, (UDP_IP, UDP_PORT))
sock.sendto(p3, (UDP_IP, UDP_PORT))
sock.sendto(p4, (UDP_IP, UDP_PORT))

print "PACKET sent!"






