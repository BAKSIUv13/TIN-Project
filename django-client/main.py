# TIN
# Bartłomiej Kulik

"""Main program module."""

from network_and_data import network, receiver, sender
from packets import packet_sender

def main():
    """Pydocstyle is annoying."""
    host = 'localhost'
    port = 22345

    # interface to sockets

    client_socket = network.prepare_socket(host, port)

    recv_read_pipe, recv_write_pipe = network.prepare_error_pipe()
    send_read_pipe, send_write_pipe = network.prepare_error_pipe()

    my_receiver = receiver.Receiver(client_socket, recv_read_pipe, send_write_pipe)
    my_sender = sender.Sender(client_socket, send_read_pipe, recv_write_pipe)
    my_packet_sender = packet_sender.PacketSender(my_sender)

    my_receiver.start()

    # my code


    # end my code

    my_receiver.join()
    my_packet_sender.join_sender_thread()

    client_socket.close()

if __name__ == '__main__':
    main()
