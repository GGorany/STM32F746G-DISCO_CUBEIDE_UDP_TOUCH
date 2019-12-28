
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import abc
from socket import socket, AF_INET, SOCK_DGRAM
from threading import Thread
from datetime import datetime


xList = []
yList = []

fig = plt.figure()
ax1 = fig.add_subplot(1, 1, 1)


def animate(i):
    ax1.clear()
    ax1.set(xlim=[-10, 490], ylim=[282, -10], title='Touch Position Display')
    ax1.scatter(xList, yList)


class UdpServer(abc.ABC):
    def __init__(self, host="", port=13000, buffer_size=1024, exit_command="exit"):
        self.__host = host
        self.__port = port
        self.__buffer_size = buffer_size
        self.__exit_command = exit_command
        self.__udp_socket = None

        self.__bind_socket()

    def rec_udp(self):
        while True:
            (data_bytes, address_tuple) = self.__udp_socket.recvfrom(self.__buffer_size)
            address, port = address_tuple
            x = int.from_bytes(data_bytes[0:3], "little")
            y = int.from_bytes(data_bytes[4:7], "little")
            print("Rx- time:{0} addr:{1}, port:{2}, X:{3}, Y:{4}".format(datetime.now(), address, port, x, y))
            xList.append(x)
            yList.append(y)

    def __bind_socket(self):
        self.__udp_socket = socket(AF_INET, SOCK_DGRAM)
        self.__udp_socket.bind((self.__host, self.__port))
        self.listen_Udp = Thread(target=self.rec_udp)
        self.listen_Udp.start()


if __name__ == '__main__':
    udp = UdpServer(port=8089)
    ani = animation.FuncAnimation(fig, animate, interval=100)
    plt.show()
