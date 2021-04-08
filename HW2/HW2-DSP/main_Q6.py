import my_functions
from my_functions import get_data,fft,subplot
from my_functions import plot_data,plot_data2
import filter_coefficients
buffer_size = 250
file_name = "sigD.csv" 
A = 0.1
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




def main():
    title = "A:" + str(A) + " B:" + str(B)  + " Filter IIR Signal :" + file_name
    time,data = get_data(file_name)
    dt = len(time)/time[-1]
    filter_data = IIR_filter(time,data)
    freq,fft_signal = fft(time, data, dt)
    freqF,fft_signalF = fft(time, filter_data, dt)
    subplot(time,data,filter_data,freq,fft_signal,freqF,fft_signalF,title)
    return 0


main()