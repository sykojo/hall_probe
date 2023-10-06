import tkinter as tk
import matplotlib
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import numpy as np

def plot():
    fig = Figure(figsize=(5,5),dpi=100)
    
    data_x = np.random.normal(50,25,300)
    data_y = np.random.normal(50,25,300)
    
    plot1 = fig.add_subplot()
    plot1.scatter(data_x,data_y)
    
    canvas = FigureCanvasTkAgg(fig,master=window)
    canvas.draw()
    
    canvas.get_tk_widget().pack()
    


window = tk.Tk()
window.title("Hall Probe")
'''
left_frame = tk.Frame(window,width=640,height=1080,bg="purple").pack(side="left")
right_frame = tk.Frame(window,width=1280,height=1080).pack(side="right")

label_left = tk.Label(left_frame,text="Left").pack(in_=left_frame)
label_right = tk.Label(right_frame,text="Right").pack(in_=right_frame)
'''
test = tk.Label(window, text="red", bg="red", fg="white")
test.pack(padx=5, pady=15, side=tk.LEFT)
test = tk.Label(window, text="green", bg="green", fg="white")
test.pack(padx=5, pady=20, side=tk.LEFT)
test = tk.Label(window, text="purple", bg="purple", fg="white")
test.pack(padx=5, pady=20, side=tk.LEFT)

window.mainloop()