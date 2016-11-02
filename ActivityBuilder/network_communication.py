import socket
import time
import struct
import threading


class NetworkCommunicator(object):
    def __init__(self, address):
        self.host, self.port = address
        self.sock = None
        # self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.connected = False
        # self.thread=threading.Thread(target=self.start)

    def start(self):
        while not self.connected:
            try:
                self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                self.sock.connect((self.host, self.port))
                self.connected = True
                print("Connected")

                # received = sock.recv(1024)
            except:
                print("Cant connect")
            time.sleep(1)

    def send(self, packet):
        if self.connected:
            try:
                self.sock.sendall(packet.pack())
            except:
                self.connected = False
                self.start()


    def receive(self):
        if self.connected:
            try:
                size, = struct.unpack("I", self.sock.recv(4))
                data = self.sock.recv(size-4)
                return size, data
            except:
                self.connected = False
                self.start()

