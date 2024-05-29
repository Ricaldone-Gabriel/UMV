import socket
import sys
import matplotlib
import mysql.connector as mysql

mydb = mysql.connect(
  host="localhost",
  user="root",
  password="",
  database="umv"
)

mycursor = mydb.cursor()

print("Creating server...")
s = socket.socket()
s.bind(('0.0.0.0', 3030))
s.listen(0)

while True:
        try:
                client, addr = s.accept()
                while True:
                        data = client.recv(32)
                        dataList = data.decode().split("/")
                        print(dataList)
                        if len(data) == 0:
                                break
                        else:
                                sql = "INSERT INTO enviroment(UMVId,Temperature,Humidity,Pressure) VALUES (%s,%s,%s,%s)"
                                if not (dataList[1] == 1 and dataList[2] == 2 and dataList[3] == 3):
                                        val = tuple(dataList)
                                        mycursor.execute(sql,val)
                                        mydb.commit()

                        print("Closing connection")
                client.close()
        except KeyboardInterrupt:
                print("Ctrl-C pressed!")
                sys.exit(0)
