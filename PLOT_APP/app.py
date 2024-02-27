import sys
from PyQt5.QtWidgets import QApplication,QMainWindow,QWidget,QFileDialog
from PyQt5.QtCore import QTimer, QThread
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
import matplotlib.pyplot as plt
import numpy as np
from threading import Thread
from time import sleep
from data_process import SerialData


class GraphPlotterApp(QMainWindow):
    def __init__(self):
        super().__init__()
        self.running = False
        self.init_ui()
        self.serialData=SerialData(self.running)
        self.redrawTimer = QTimer(self)
        self.redrawTimer.timeout.connect(self.update_plot)
        self.redrawTimer.start(1000)
        #self.t_readSerial = Thread(target=self.serialData.read_serial,args=(self.serialData.dataAreReady,))
        self.t_readSerial = QThread()
        #self.t_waitAndPlot = Thread(target=self.plot_data)
        

    def init_ui(self):
        self.setWindowTitle("Hall Probe")
        self.setGeometry(100,100,800,600)
        self.setupMenuBar()
        self.setupPlotCanvas()
        self.running=True

    def setupMenuBar(self):
            menu_bar=self.menuBar()             #Vytvoření menu
            file_menu=menu_bar.addMenu("Settings")  #Přidánízáložky do menu
            moznosti_menu = menu_bar.addMenu("Moznosti")
    
    def setupPlotCanvas(self):
        self.figure = plt.figure()
        self.canvas = FigureCanvas(self.figure)
        self.setCentralWidget(self.canvas) 

    def update_plot(self):
        print(f"Data: {self.serialData.senData1.x} \n")
    ''' 
    def plot_data(self):
        while self.running:  
            #self.serialData.read_serial(self.serialData.dataAreReady)          
            with self.serialData.dataAreReady:
                print("Main thread waiting for data...")
                self.serialData.dataAreReady.wait()
                print("Notified... plotting the data")
                print(f"Data: {self.serialData.senData1.x} \n")
                sleep(1)
                '''
    
     

                


             
         

        

