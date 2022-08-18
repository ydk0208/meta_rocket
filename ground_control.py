import random
from itertools import count
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
 
plt.style.use('fivethirtyeight')
f,axes = plt.subplots(2, 3)
plt.subplots_adjust(wspace = 0.3, hspace = 0.3)

x_val = []
y_val1 = []
y_val2 = []
y_val3 = []
 
index = count()
 
def animate(i):
    x_val.append(next(index))
    y_val1.append(random.randint(-50,50))
    y_val2.append(random.randint(-50,50))
    y_val3.append(random.randint(-50,50))
    
    if len(x_val) >= 10:
        plt.cla()
        axes[0][0].plot(x_val[5:11],y_val1[5:11], color = 'purple')
        axes[0][1].plot(x_val[-10:],y_val2[-10:], color = 'green')
        axes[0][2].plot(x_val[-10:],y_val3[-10:], color = 'blue')
    else:
        plt.cla()
        axes[0][0].plot(x_val, y_val1, color = 'red')
        axes[0][1].plot(x_val, y_val2, color = 'green')
        axes[0][2].plot(x_val, y_val3, color = 'blue')

 
ani = FuncAnimation(plt.gcf(), animate, interval = 500)
 
 

 
plt.show()
