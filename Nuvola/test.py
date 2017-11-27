import unittest
import socket
import time

class TestProjectMethods(unittest.TestCase):
	
	s = 0
	
	def test_signup_signin_signout(self):		
		self.s.send("20208username08password05email09firstname08lastname")
		data = self.s.recv(1024)
		self.assertEqual("1010", data)	
		
		self.s.send("20008username08password")
		data = self.s.recv(1024)
		self.assertEqual("1000", data) 
		
		self.s.send("201")
	
	
if __name__ == '__main__':
	TestProjectMethods.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	TestProjectMethods.s.connect(("127.0.0.1", 22223))
	unittest.main()
