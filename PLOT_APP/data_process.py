from threading import Condition, Thread
from serial import Serial
from time import sleep
from PyQt5.QtCore import QMutex, QThread
from PyQt5.QtCore import QObject, QTimer, QThread,pyqtSignal,QMutex,Qt


class Sensor:
    def __init__(self,adr) -> None:
        self.adr = adr
        self.data = SensorData()

class SensorData:
    def __init__(self) -> None:
        self.x = 0
        self.y = 0
        self.z = 0
        self.senDataDict = {
            "x" : self.x,
            "y" : self.y,
            "z": self.z
        }
        


class ReadSerialThread(QThread):
    dataAreReady = pyqtSignal(list,int)
    def __init__(self,sensors:list):
        super().__init__()
        self.sensors = sensors
        self.t=0
        print(f"Sensors: {self.sensors}")
        self.serialPort = Serial('COM9',baudrate=115200)
        self.mutex = QMutex()
        

    def run(self):
        self.serialPort.write("\n".encode())
        self.serialPort.timeout=0
        print("Reading data from serial \n")
        self.mutex.lock()
        data = self.serialPort.read(4)
        print(len(data))
        
        print(f"Recieved data: {data} \n")
        #self.decode_data(data)
        self.sensors[0].data.x = int.from_bytes(data,"big",signed=True)
        print(f"Decoded data x: {(self.sensors[0].data.x)/1000000.0} \n")
        self.t = self.t+1
        self.dataAreReady.emit(self.sensors,self.t)
         # TODO x,y,z
        self.mutex.unlock()

    def decode_data(self,data):
        # Assuming the data is received as little endian
        print(f"data : {data} \n")
        x_B = 0
        x_B = data[0] << 24
        x_B |= data[1] << 16
        x_B |= data[2] << 8
        x_B |= data[3]
        self.sensors[0].data.x = x_B
        print(x_B)
        print(type(x_B))

    
