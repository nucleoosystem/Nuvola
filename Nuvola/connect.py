import socket
import os


s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("127.0.0.1", 22223))

s.send("20208username08password05email0210")