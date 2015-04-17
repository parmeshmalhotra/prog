#!/usr/local/python-2.6.6-64bit/bin/python
import pprint
import socket
import sys

#Create a TCP/IP socket

sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)

# wqops-test2 machine ip address : 10.81.95.50
##Ports are specified by a number ranging from 1 to 65535.
## Many ports below 1024 are associated with services that Linux and Unix-like operating systems consider critical to essential network functions, so you must have root privileges to assign services to them.
##
## Ports between 1024 and 49151 are considered "registered". This means that they can be "reserved" (in a very loose sense of the word) for certain services by issuing a request to the IANA (Internet Assigned Numbers Authority). They are not strictly enforced, but they can give a clue as to the possible services running on a certain port.
##
## Ports between 49152 and 65535 cannot be registered and are suggested for private use.
##
##


server_address= ('10.88.211.123',49153)
print >>sys.stderr, 'starting up on %s port %s' %server_address
#Bind the socket to the port
sock.bind(server_address)

#Listen for incoming connections
sock.listen(1)

while True:
#Wait for a connection
  print >>sys.stderr, 'waiting for a connection'
  connection, client_address = sock.accept()

  try:
    print >>sys.stderr, 'connection from',client_address

    #Receive data in small chunks and retransmit it

    while True:
      data= connection.recv(10000)
      if not data:
       continue
      if (data[:4] == 'GET ' or data[:7] == 'BOUNCE ' or data[:5] == 'EXIT '):
        if (data[:7] == 'BOUNCE '):
#      sys.exit('DONE')
         print >>sys.stderr, 'received "%s"'% data[7:]
         print >>sys.stderr, 'sending data back to client'
         connection.sendall(data[7:])
      else: 
       print "Wrong Input"
       break
  finally:
    connection.close()

