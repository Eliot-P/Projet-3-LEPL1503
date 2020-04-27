import os
from numpy import *
import matplotlib.pyplot as plt
import cpuinfo

plt.style.use('ggplot')

def grapher(array):
    n = 1
    ax = plt.subplot()
    arr = asarray(array)
    mean_arr = []
    for n_thread in arr : 
        maxi = amax(n_thread)
        mini = amin(n_thread)
        moy = mean(n_thread)
        ax.plot(n,maxi,'go',)
        ax.text(n-0.05,maxi,maxi)
        ax.plot(n,mini,'go')
        ax.text(n-0.05,mini,mini)
        ax.plot([n,n],[mini,maxi],'g')
        ax.plot(n,moy,'bo')
        mean_arr.append(moy)
        n+=1
    mean_numpy_arr = asarray(mean_arr)
    ax.plot(range(1,n),mean_numpy_arr,'b--')
    ax.axhline(amin(mean_numpy_arr))
    ax.set_xlabel('Number of thread [N]')
    ax.set_ylabel('Time [ms]')
    ax.set_title("Execution de fact avec l'exemple d'input sur un processeur " + cpuinfo.get_cpu_info()['brand'].split("w/")[0] ,pad=30)
    plt.show()



def exec (Number_of_thread) : 
    execution = os.popen('./fact -N ' + str(Number_of_thread) + 'Input.txt Output.txt')
    time_taken_raw = execution.read()
    time_taken = float(time_taken_raw.strip())
    os.system('rm -rf Output.txt')
    return time_taken
    
def main (number_of_exec,max_number_of_thread):
    big_array = []
    for n in range (max_number_of_thread):
        array_of_time_for_n_thread = []
        for i in range (number_of_exec):
            time_taken = around(exec(max_number_of_thread)*100,2)
            array_of_time_for_n_thread.append(time_taken)
        big_array.append(array_of_time_for_n_thread)
    grapher(big_array)

main(10, 8)