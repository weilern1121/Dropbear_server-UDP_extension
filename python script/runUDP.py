import socket

UDP_IP = "127.0.0.1"
UDP_PORT = 53
LARGEST_UINT32 = 2147483647
BAD_LARGEST_UINT32_1 = 21474836479
BAD_LARGEST_UINT32_2 = 214748364799
LARGEST_UINT16 = 65535
BAD_LARGEST_UINT16 = 655359
MAGIC_NUM = int("0xDEADBEEF", 16)
NEG_NUM = -44
NUM1 = 1234
NUM2 = 56
COMMAND1 = "ls"+'\0'
MESSAGE1 = str(LARGEST_UINT32)+" "+str(LARGEST_UINT16)+" "+COMMAND1 #OK
MESSAGE2 = str(BAD_LARGEST_UINT32_1)+" "+str(LARGEST_UINT16)+" "+COMMAND1 #ERROR
MESSAGE3 = str(BAD_LARGEST_UINT32_2)+" "+str(LARGEST_UINT16)+" "+COMMAND1 #ERROR
MESSAGE4 = str(LARGEST_UINT32)+" "+str(BAD_LARGEST_UINT16)+" "+COMMAND1 #ERROR
MESSAGE5 = str(NEG_NUM)+" "+str(LARGEST_UINT16)+" "+COMMAND1 #OK
MESSAGE6 = str(NUM1)+" "+str(NEG_NUM)+" "+COMMAND1 #ERROR
MESSAGE7 = str(NUM1)+" "+str(NUM2)+" "+COMMAND1 #OK
MESSAGE8 = str(MAGIC_NUM)+" "+str(NUM2)+" "+COMMAND1 #OK
MESSAGE9 = str(MAGIC_NUM)+" "+str(35)+" "+COMMAND1 #OK

print "UDP target IP:", UDP_IP
print "UDP target port:", UDP_PORT
#print "MESSAGE:", MESSAGE8
sock = socket.socket(socket.AF_INET, # Internet
                      socket.SOCK_DGRAM) # UDP
#test1 - packet parsing check
"""
print "TEST1"
sock.sendto(MESSAGE1, (UDP_IP, UDP_PORT))
sock.sendto(MESSAGE2, (UDP_IP, UDP_PORT))
sock.sendto(MESSAGE3, (UDP_IP, UDP_PORT))
sock.sendto(MESSAGE4, (UDP_IP, UDP_PORT))
sock.sendto(MESSAGE5, (UDP_IP, UDP_PORT))
sock.sendto(MESSAGE6, (UDP_IP, UDP_PORT))
"""

#test2 - magic field check and port adding
"""
print "TEST2"
sock.sendto(MESSAGE7, (UDP_IP, UDP_PORT)) #NOT ADD
sock.sendto(MESSAGE8, (UDP_IP, UDP_PORT)) #OK
sock.sendto(MESSAGE9, (UDP_IP, UDP_PORT)) #ERROR
sock.sendto(MESSAGE8, (UDP_IP, UDP_PORT)) #ERROR
"""

#test3 - add ports:
#range(12) bigger than max ports allowed in the server
"""
print "TEST3"
for x in range(12):
	MSG = str(MAGIC_NUM)+" "+str(x+1000)+" "+COMMAND1
	sock.sendto(MSG, (UDP_IP, UDP_PORT))

print "MESSAGES sent!"
"""

#test4 - check exec
print "TEST4"
COMMAND2 = "echo PIG"+'\0'
MASSAGE10 = str(MAGIC_NUM)+" "+str(NUM2)+" "+COMMAND2 #OK
sock.sendto(MASSAGE10, (UDP_IP, UDP_PORT)) #OK
print "MESSAGE sent!"






