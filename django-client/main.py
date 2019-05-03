# TIN
# Bartlomiej Kulik

"""Main program module."""

from network_and_data import receiver

# interface to sockets


RECEIVER = receiver.Receiver()
RECEIVER.start()

while True:
    WORD = RECEIVER.r_queue.get(block=True, timeout=None)
    TO_PRINT = WORD[0:1].decode()
    if TO_PRINT in ('', ' '):
        continue
    print('DATA: ', ord(TO_PRINT[0]))

    if TO_PRINT == "0":
        break

RECEIVER.join()

print("the end!")
