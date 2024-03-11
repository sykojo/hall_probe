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
        self.debug=True
        print(f"Sensors: {self.sensors}")
        self.serialPort = Serial('COM9',baudrate=115200)
        self.mutex = QMutex()
        

    def run(self):
        self.serialPort.write("0".encode())
        self.serialPort.timeout=0
        data = [0,0,0]
        data_decoded = [0,0,0]
        if self.debug:
            print("Reading data from serial \n")
            print("------------------------------------------------")
        self.mutex.lock()
        for i in range(3):
            data[i]=self.serialPort.read(4)
            data_decoded[i] = int.from_bytes(data[i],"little",signed=True)
            setattr(self.sensors[0].data,['x','y','z'][i],data_decoded[i])
            if self.debug:
                print(f"Recieved data: {int.from_bytes(data[i],"little",signed=True)} \n")  
        print(f"Stored data x: {self.sensors[0].data.x} \n")
        print(f"Stored data y: {self.sensors[0].data.y} \n")
        print(f"Stored data z: {self.sensors[0].data.z} \n")
        self.t = self.t+1
        self.dataAreReady.emit(self.sensors,self.t)
        self.mutex.unlock()


    
