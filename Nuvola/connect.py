import socket
import os


s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("127.0.0.1", 22223))

size = os.path.getsize('C:\\Users\\ShakedNissanov\\Documents\\SchoolWork\\MyWork\\CloudProject\\Nuvola\\download.jpg')

sendStr = "203" + str(size).zfill(5) + str(size)
s.send(sendStr)

f = open ("download.jpg", "rb")
l = f.read(1024)
while (l):
    s.send(l)
    l = f.read(1024)
s.close()
	

