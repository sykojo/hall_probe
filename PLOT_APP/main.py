#from app import GraphPlotterApp, QApplication,np,plt,sys
from app import GraphPlotterApp, QApplication,sys
from threading import Condition, Thread
from data_process import SerialData, SensorData


if __name__=='__main__':

    app=QApplication(sys.argv)
    mainWin = GraphPlotterApp()
    mainWin.show()
    #mainWin.t_readSerial.start()


    #graph = mainWin.figure.add_subplot(111)
    #x=np.linspace(0,10,100)
    #y = np.sin(x)
    
    '''
    while True:
        graph.clear()  # Clear the previous plot
        graph.plot(x, y)
        mainWin.canvas.draw()
        x = x * 2
        y = np.sin(x)  # Recalculate y values
        print(x)
        
        app.processEvents()  # Allow Qt event processing
        '''