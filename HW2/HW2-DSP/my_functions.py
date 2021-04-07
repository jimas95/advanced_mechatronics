import csv
import matplotlib.pyplot as plt # for plotting
import numpy as np 


def fft(t, data, dt):
    Fs = 10000 # sample rate
    Ts = 1.0/Fs; # sampling interval
    ts = np.arange(0,t[-1],Ts) # time vector
    y = data # the data to make the fft from
    n = len(y) # length of the signal
    k = np.arange(n)
    T = n/Fs
    frq = k/T # two sides frequency range
    frq = frq[range(int(n/2))] # one side frequency range
    Y = np.fft.fft(y)/n # fft computing and normalization
    Y = Y[range(int(n/2))]
    return frq,abs(Y)


def subplot(time,data,dataF,freq,fft_signal,freqF,fft_signalF,title):
    fig, (ax1, ax2) = plt.subplots(2, 1)
    ax1.set_title(title)
    ax1.plot(time,data,'black')
    ax1.plot(time,dataF,'r')
    ax1.set_xlabel('Time')
    ax1.set_ylabel('Amplitude')
    ax2.loglog(freq,fft_signal,'black') # plotting the fft
    ax2.loglog(freqF,fft_signalF,'r') # plotting the fft filtered
    ax2.set_xlabel('Freq (Hz)')
    ax2.set_ylabel('|Y(freq)|')
    plt.show()





def info(time,data):
    lengthT = len(time)
    lengthD = len(data)
    if(lengthT!=lengthD):
        print("there is an error in data size")

    
    print("Total number of values --> ",lengthD)
    print("Start Time --> ",time[0])
    print("Total Time --> ",time[-1])
    print("Time    dt --> ",time[1]-time[0])
    print("Time   fre --> ",len(time)/time[-1])


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

def plot_data2(time,data1,data2):
    plt.plot(time,data1,'b-*')
    plt.plot(time,data2,'r-*')
    plt.xlabel('Time [s]')
    plt.ylabel('Signal')
    plt.title('Signal vs Time')
    plt.show()