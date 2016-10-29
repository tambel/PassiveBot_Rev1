import socket

import threading



class NetworkCommunicator(object):
    def __init__(self,address):
        self.host, self.port = address
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        #self.thread=threading.Thread(target=self.start)

    def start(self):
        try:

            self.sock.connect((self.host, self.port))
            print("Connected")

            #received = sock.recv(1024)
        except:
            print("Cant connect")

    def send(self,packet):
        self.sock.sendall(packet.pack())

    def recieve(self):
        return self.sock.recv(1024)
