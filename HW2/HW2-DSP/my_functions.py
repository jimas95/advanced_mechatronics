import csv
import matplotlib.pyplot as plt # for plotting
import numpy as np # for sine function

def info(time,data):
    lengthT = len(time)
    lengthD = len(data)
    if(lengthT!=lengthD):
        print("there is an error in data size")

    
    print("Total number of values --> ",lengthD)
    print("Start Time --> ",time[0])
    print("Total Time --> ",time[-1])
    print("Time    dt --> ",time[1]-time[0])

def get_data(file_name):
    t = [] # column 0
    data = [] # column 1
    print("Reading Data -->",file_name)
    with open(file_name) as f:
        # open the csv file
        reader = csv.reader(f)
        for row in reader:
            t.append(float(row[0])) # leftmost column
            data.append(float(row[1])) # second column
    print("Getting Data --> Completed")
    info(t,data)
    return t,data



def plot_data(time,data):
    plt.plot(time,data,'b-*')
    plt.xlabel('Time [s]')
    plt.ylabel('Signal')
    plt.title('Signal vs Time')
    plt.show()

