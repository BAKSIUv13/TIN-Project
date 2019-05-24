# TIN
# Bartłomiej Kulik

"""Main program module."""

from network_and_data import network, receiver, sender

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

    my_receiver.start()
    my_sender.start()

    # my code

    my_sender.put_int32_value(589505316)

    print(my_receiver.get_int32_value())


    # end my code

    my_receiver.join()
    my_sender.join()

    client_socket.close()

if __name__ == '__main__':
    main()
