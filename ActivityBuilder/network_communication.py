import socket
import time
import struct



class NetworkCommunicator(object):
    def __init__(self, address):
        self.host, self.port = address
        self.sock = None
        # self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.connected = False
        # self.thread=threading.Thread(target=self.start)
        self.last_send_packet = None

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

    def send(self, packet=None):
        if self.connected:
            try:
                packet = packet if packet is not None else self.last_send_packet
                self.sock.sendall(packet.pack())
                self.last_send_packet = packet
            except:
                self.connected = False
                self.start()

    def receive(self ):
        if self.connected:
            try:
                size, = struct.unpack("I", self.sock.recv(4))
                data = self.sock.recv(size - 4)
                return size, data
            except:
                self.connected = False
                self.start()


    def send_receive(self, request):
        self.send(RequestPacket(Requests[request]))
        return Packet(request, data=self.receive())

    def repeat_last_packet(self, packet_structure):
        self.send()
        return self.receive(packet_structure)
