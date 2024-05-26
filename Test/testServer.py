import socket
import sys
import mysql.connector

mydb = mysql.connector.connect(
  host="localhost",
  user="yourusername",
  password="yourpassword",
  database="umv"
)

print("Creating server...")
s = socket.socket()
s.bind(('0.0.0.0', 3030))
s.listen(0)

while True:
        client, addr = s.accept()
        while True:
                content = client.recv(32)
                data = content.split("/")
                if len(content) == 0:
                        break
                else:
                        print(content)
        print("Closing connection")
        client.close()