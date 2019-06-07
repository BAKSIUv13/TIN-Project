# TIN
# Bartłomiej Kulik

"""Main program module."""

from network_and_data import network, receiver, sender
from packets import packet_sender, packet_receiver

def main():
    """Pydocstyle is annoying."""
    host = '192.168.0.73'
    port = 22334

    # interface to sockets

    client_socket = network.prepare_socket(host, port)

    recv_read_pipe, recv_write_pipe = network.prepare_error_pipe()
    send_read_pipe, send_write_pipe = network.prepare_error_pipe()

    my_receiver = receiver.Receiver(client_socket, recv_read_pipe, send_write_pipe)
    my_packet_receiver = packet_receiver.PacketReceiver(my_receiver)

    my_sender = sender.Sender(client_socket, send_read_pipe, recv_write_pipe)
    my_packet_sender = packet_sender.PacketSender(my_sender)

    # my code

    my_packet_sender.send_logo("admin", "admin")

    while True:
        my_packet_receiver.receive_packet()

    my_packet_sender.send_lout()

    # end my code

    my_packet_receiver.join_receiver_thread()
    my_packet_sender.join_sender_thread()

    client_socket.close()

if __name__ == '__main__':
    main()
