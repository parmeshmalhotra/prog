#!/usr/local/python-2.6.6-64bit/bin/python
import socket
import sys

#Create a TCP/IP Socket
sock =socket.socket(socket.AF_INET,socket.SOCK_STREAM)


server_address = ('10.88.211.123',49153)
print >>sys.stderr, 'connecting to %s port %s' %server_address

#Connect socket to the port where the server is listening
sock.connect(server_address)

try:
  #Send data
 while True:
  message= raw_input('ENTER COMMANDS \n 1.GET <filename> or \n 2.BOUNCE <text> or \n 3.EXIT <exitcode> : ')
  print >>sys.stderr, 'sending "%s"' %message
  sock.sendall(message)
  #Look for response from the server 
  data=sock.recv(10000)
  print >>sys.stderr, 'received "%s"'%data
finally:
  print >>sys.stderr,'closing socket'
  sock.close()
