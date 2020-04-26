import os
from numpy import *
import matplotlib.pyplot as plt

big_array = []

def grapher (array) :
    N = arange(len(array))
    N = N[:]+1
    plt.plot(N,array,'ro')
    return None

def exec (Number_of_thread) : 
    execution = os.popen('./fact -N ' + 'Number_of_thread' + 'Input.txt Output.txt')
    time_taken_raw = execution.read()
    time_taken = float(time_taken_raw.strip())
    os.system('rm -rf Output.txt')
    return time_taken

def main_exec (max_number_of_thread) :
    array = []
    for i in range (max_number_of_thread) : 
        array.append(exec(i+1))
    grapher(array)
    return None
    
def main (number_of_exec,max_number_of_thread): 
    for i in range(number_of_exec) : 
        main_exec(max_number_of_thread)
    return None

main(20, 16)
plt.show()