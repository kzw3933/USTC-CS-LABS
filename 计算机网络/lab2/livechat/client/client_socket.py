import socket
from config import *

class ClientSocket(socket.socket):
    def __init__(self):
        super().__init__(socket.AF_INET,socket.SOCK_STREAM)

    def connect(self):
        super().connect((SERVER_IP,SERVER_PORT))

    def recv_data(self):

        return self.recv(512).decode('utf-8')

    def send_data(self,messages):

        return self.send(messages.encode('utf-8'))
