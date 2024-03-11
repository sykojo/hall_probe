import sys
from PyQt5.QtWidgets import QApplication,QMainWindow,QWidget,QFileDialog,QGridLayout,QPushButton,QToolBar,QDockWidget,QTabBar,QTabWidget,QSizePolicy
from PyQt5.QtCore import QObject, QTimer, QThread,pyqtSignal,QMutex,Qt,QSize
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
from threading import Thread
from time import sleep
from data_process import SensorData,Sensor,ReadSerialThread
from serial import Serial
from enum import Enum

class Screens:
    def __init__(self) -> None:
        self.singleSenTime=1
        self.allSenTime=2
        self.allSenXYZ=3
        self.test=4
    

class GraphPlotterApp(QMainWindow):
    def __init__(self):
        super().__init__()
        self.running = False
        self.plotUpdated = pyqtSignal()
        self.sensors=[]
        self.screens = Screens()
        self.current_screen = self.screens.singleSenTime
        self.setup_sensors()
        self.init_ui()
        self.init_serial()
        self.start_timer()
        
    def init_ui(self):
        self.setWindowTitle("Hall Probe")
        self.setGeometry(200,200,1600,1200)
        #self.setupMenuBar()
        self.change_graph_screen(self.screens.singleSenTime)
        self.setupToolbar()
        self.running=True
        self.debug=True

    def init_serial(self):
        self.t_readSerial = ReadSerialThread(self.sensors)
        self.t_readSerial.start()
        self.t_readSerial.dataAreReady.connect(self.update_plot)
        self.t_readSerial.start()

    def start_timer(self):
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.t_readSerial.start)
        self.timer.start(1)

    def setupMenuBar(self):
        menu_bar=self.menuBar()             #Vytvoření menu
        file_menu=menu_bar.addMenu("Settings")  #Přidánízáložky do menu
        moznosti_menu = menu_bar.addMenu("Moznosti")


    def change_graph_screen(self,screen:Screens):
        self.figure = plt.figure()
        self.figure.patch.set_facecolor("#ececec")
        self.canvas = FigureCanvas(self.figure)
    
        if screen == self.screens.test:
            self.pltA = self.figure.add_subplot(111)

        if screen == self.screens.singleSenTime:
            self.pltA = self.figure.add_subplot(311)
            self.pltB = self.figure.add_subplot(312)
            self.pltC = self.figure.add_subplot(313)
            self.current_screen=self.screens.singleSenTime

        if screen == self.screens.allSenTime:
            self.pltA = self.figure.add_subplot(811)
            self.pltB = self.figure.add_subplot(812)
            self.pltC = self.figure.add_subplot(813)
            self.pltD = self.figure.add_subplot(814)
            self.pltE = self.figure.add_subplot(815)
            self.pltF = self.figure.add_subplot(816)
            self.pltG = self.figure.add_subplot(817)
            self.pltH = self.figure.add_subplot(818)

            self.current_screen = self.screens.allSenTime

        if screen == self.screens.allSenXYZ:
            self.pltA = self.figure.add_subplot(311)
            self.pltB = self.figure.add_subplot(312)
            self.pltC = self.figure.add_subplot(313)

        self.setCentralWidget(self.canvas)
        self.canvas.draw()
        
    def setupToolbar(self):
        self.toolbar = QToolBar("Toolbar")
        self.setup_tool_buttons()
        self.toolbar.addSeparator()
        self.addToolBar(Qt.ToolBarArea.LeftToolBarArea,self.toolbar)
    
    def setup_tool_buttons(self):
        self.singleSenTimeButt = QPushButton("Single Sensor \n Time")
        self.allSenTime = QPushButton("All sensors \n Time")
        self.allSenXYZButt = QPushButton("All sensors \n X")


        self.singleSenTimeButt.setSizePolicy(QSizePolicy.Policy.MinimumExpanding,QSizePolicy.Policy.MinimumExpanding)
        self.allSenTime.setSizePolicy(QSizePolicy.Policy.MinimumExpanding,QSizePolicy.Policy.MinimumExpanding)
        self.allSenXYZButt.setSizePolicy(QSizePolicy.Policy.MinimumExpanding,QSizePolicy.Policy.MinimumExpanding)

        
        self.singleSenTimeButt.pressed.connect(lambda:self.change_graph_screen(self.screens.singleSenTime))
        self.allSenTime.pressed.connect(lambda: self.change_graph_screen(self.screens.allSenTime))
        self.allSenXYZButt.pressed.connect(lambda: self.change_graph_screen(self.screens.allSenXYZ))
        
        self.toolbar.addWidget(self.singleSenTimeButt)
        self.toolbar.addWidget(self.allSenTime)
        self.toolbar.addWidget(self.allSenXYZButt)



    def setupTabBar(self):
        tabBar = QTabBar()
        tabBar.addTab("Sensor 1")
        tabBar.addTab("Sensor 2")
        self.toolbar.addWidget(tabBar)
        self.addToolBar(self.toolbar)
        self.setTabShape(QTabWidget.TabShape.Triangular)
    
    def setupDock(self):
        self.dock = QDockWidget()
        area = Qt.DockWidgetArea.LeftDockWidgetArea
        self.dock.setWidget(QPushButton("button"))
        self.dock.setWidget(QPushButton("Qbutton2"))
        self.addDockWidget(area,self.dock)

    def setup_sensors(self):
        for i in range(0,7):
            self.sensors.append(Sensor(i))
  
    def animate(self,i):
        self.line.set_ydata(np.sin(self.x + i / 50))  # update the data.
        return self.line,

    def update_plot(self,sensors:list,t):
        self.x = np.arange(0,2*np.pi,0.01)
        if self.current_screen == self.screens.test:
            self.line, = self.pltA.plot(self.x, np.sin(self.x))
            ani = animation.FuncAnimation(self.figure, self.animate,interval=20,blit=True,save_count=50)
            self.canvas.draw()

        if self.current_screen == self.screens.singleSenTime:
            print(f"Data to plot: {sensors[0].data.x} \n")
            y1 = (sensors[0].data.x)
            y2 = (sensors[0].data.y)
            y3 = (sensors[0].data.z)
            #self.ax.clear()  # Clear the axes
            self.pltA.plot(t,y1,"-*")
            self.pltB.plot(t,y2,"-*")
            self.pltC.plot(t,y3,"-*")
            #ani = animation.FuncAnimation(self.figure, self.animate,interval=20,blit=True,save_count=50)
            #self.pltA.set_ylim(-5e9,5e9)

        if self.current_screen == self.screens.allSenTime:
            print(f"Data to plot: {sensors[0].data.x} \n")
            y = sensors[0].data.x
            #self.ax.clear()  # Clear the axes
            self.pltA.plot(t,y,"-*")
            self.pltB.plot(t,y,"-*")
            self.pltC.plot(t,y,"-*")
            self.pltD.plot(t,y,"-*")
            self.pltE.plot(t,y,"-*")
            self.pltF.plot(t,y,"-*")
            self.pltG.plot(t,y,"-*")
            self.pltH.plot(t,y,"-*")
        if self.current_screen == self.screens.allSenXYZ:
            self.pltA.plot(t,y,"-*")
            self.pltB.plot(t,y,"-*")
            self.pltC.plot(t,y,"-*")
            
            #self.ax.set_ylim(1.2e9,1.3e9)
        self.canvas.draw()




 
  



    
     

                


             
         

        

