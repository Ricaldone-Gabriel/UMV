import socket
import sys

print("Creating server...")
s = socket.socket()
s.bind(('0.0.0.0', 3030))
s.listen(0)

try:
    while True:
        client, addr = s.accept()
        while True:
                content = client.recv(32)
                if len(content) == 0:
                        break
                else:
                        print(content)
        print("Closing connection")
        client.close()
except KeyboardInterrupt:
  print("Ctrl-C pressed!")
  sys.exit(0)
