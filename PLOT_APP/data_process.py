from threading import Condition, Thread
from serial import Serial
from time import sleep
from PyQt5.QtCore import QMutex, QThread



class SerialData(QThread):
    def __init__(self):
        super().__init__()
        self.senData1 = SensorData()
        self.dataAreReady = Condition()
        self.serialPort = Serial('COM9',baudrate=115200)
        self.mutex = QMutex()

    
    def read_serial(self,dataAreReady):
            while self.running:
                sleep(1)
                self.mutex.lock()
                self.senData1.x = self.serialPort.read(1) #replace with readUntil()
                self.mutex.unlock()
                # TODO add y,z,T
                print("Thread sending a notification... Data are ready to display")
                with dataAreReady:
                    dataAreReady.notify()

class SensorData:
    def __init__(self) -> None:
        self.x = 0.0
        self.y = 0.0
        self.z = 0.0
        



