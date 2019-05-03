# TIN
# Bartlomiej Kulik

"""Main program module."""

from network_and_data import receiver

# interface to sockets


RECEIVER = receiver.Receiver()
RECEIVER.start()

print("main program")

while True:
    WORD = RECEIVER.r_queue.get(block=True, timeout=None)
    print(WORD[0:4].decode())
    if WORD.decode() == "0":
        break


RECEIVER.join()

print("the end!")
