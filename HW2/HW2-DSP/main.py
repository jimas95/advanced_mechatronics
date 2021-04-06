import my_functions
from my_functions import get_data
from my_functions import plot_data


def low_pass(time,data):
    buffer_size = 2000
    buffer = []
    new_data = []
    for i in range(buffer_size):
        buffer.append(0)
    
    for val in data:
        buffer.append(val)
        buffer.pop(0)
        # print(len(buffer))
        new_val = sum(buffer)/buffer_size
        new_data.append(new_val)
    
    return new_data

def main():

    file_name = "sigA.csv" 
    time,data = get_data(file_name)
    filter_data = low_pass(time,data)
    plot_data(time,filter_data)
    plot_data(time,data)
    return 0


main()