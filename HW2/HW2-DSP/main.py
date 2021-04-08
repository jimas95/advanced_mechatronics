import my_functions
from my_functions import get_data,fft,subplot
from my_functions import plot_data,plot_data2
import filter_coefficients
import numpy as np
buffer_size = 250
file_name = "sigD.csv" 
H = filter_coefficients.hD
A = 0.005
B = 1.0 - A

def low_pass(time,data):
    buffer = []
    new_data = []
    for i in range(buffer_size):
        buffer.append(data[0])
    
    for val in data:
        buffer.append(val)
        buffer.pop(0)
        new_val = sum(buffer)/buffer_size
        new_data.append(new_val)
    
    return new_data

def IIR_filter(time,data):

    filter = [data[0]]
    for i in range(1,len(data)):
        new_val = A*data[i] + B*filter[-1]
        filter.append(new_val)
    return filter


def FIR_filter(time,data):
    
    filter = []
    filter_size = len(H)
    origi_size = len(data)
    temp_data = data.copy()
    for i in range(filter_size):
        temp_data.append(data[-1])

    for i in range(origi_size):
        values = temp_data[i:i+filter_size]
        new_val = np.average(values, weights=H)
        filter.append(new_val)
    return filter

def main():
    title =  "F=400 FL=20 BL=12 Filter FIR Signal :" + file_name
    time,data = get_data(file_name)

    # dt = 1.0/10000.0 # 10kHz
    # time = np.arange(0.0, 1.0, dt) # 10s
    # # a constant plus 100Hz and 1000Hz
    # s = 4.0 * np.sin(2 * np.pi * 100 * time) + 0.25 * np.sin(2 * np.pi * 1000 * time) + 25
    # data = s.tolist()

    dt = len(time)/time[-1]
    filter_data = FIR_filter(time,data)
    freq,fft_signal = fft(time, data, dt)
    freqF,fft_signalF = fft(time, filter_data, dt)
    subplot(time,data,filter_data,freq,fft_signal,freqF,fft_signalF,title)
    return 0


main()