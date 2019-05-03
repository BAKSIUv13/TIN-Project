"""Main program module."""

import threading
import socket
import queue

WORDS_SIZE = 1024
WORD_SIZE = 4

words = queue.Queue(WORDS_SIZE)

class ClientSocket(threading.Thread):
    def __init__(self, HOST, PORT):
        threading.Thread.__init__(self)
        self.s = socket.socket()
        self.s.connect((HOST, PORT))

    def run(self):

        self.s.send('0'.encode())

        while(True):
            word = self.s.recv(WORD_SIZE)
            if (len(word) == 0):
                continue

            if (word.decode() == "0"):
                self.s.send('0'.encode())
                self.s.close()
                break


            words.put(word, block=True, timeout=None)


# interface to sockets

socket = ClientSocket('127.0.0.1', 12345)
socket.start()

print ("main program")

while True:
    w = words.get(block=True, timeout=None)
    print(w[0:4].decode())
    if (w.decode() == "0"):
        break


socket.join()

print("the end!")
