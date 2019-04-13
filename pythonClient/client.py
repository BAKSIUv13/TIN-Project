import socket
import threading
import time
import tkinter

s = socket.socket()		 

port = 12345				

s.connect(('127.0.0.1', port))

def reciving():
    while True:
        str=s.recv(1024)
        if "OwO!"==str[0:4].decode():
            if "maus"==str[4:8].decode():
                x=int.from_bytes(str[8:12], byteorder='big')
                y=int.from_bytes(str[12:16], byteorder='big')
                length=int.from_bytes(str[16:20], byteorder='big')
                name=str[20:20+length].decode()
                print('mause of '+name+' x: %d y: %d'%(x, y))
            elif "msg1"==str[4:8].decode():
                length=int.from_bytes(str[8:12], byteorder='big')
                name=str[12:12+length].decode()
                length1=int.from_bytes(str[12+length:16+length], byteorder='big')
                msg=str[16+length:16+length+length1].decode()
                print('CHAT: '+name+': '+msg)                
            else:
                print("-----------Nieznany komunikat--------------")

s.send('ip login haslo'.encode())
    
x = threading.Thread(target=reciving, args=())
x.start()

while True:
    s.send(''.encode())
    time.sleep(1)
    
x.join()
s.close()	 
