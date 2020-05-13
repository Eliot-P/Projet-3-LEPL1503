import os
from numpy import *
import matplotlib.pyplot as plt
import cpuinfo
from time import time
from ctypes import *

so_file = "/mnt/c/Users/eliot/OneDrive - UCL/Bac1 Q4/Projet 3/lepl1503-2020-groupe-q5/Prime_thread.so"
CPrime = CDLL(so_file)
plt.style.use('ggplot')

def grapher(array,n_error):
    n = 1
    ax = plt.subplot()
    arr = asarray(array)
    mean_arr = []
    for n_thread in arr : 
        stand = around(std(n_thread),2)
        maxi = around(amax(n_thread),2)
        mini = around(amin(n_thread),2)
        moy = around(mean(n_thread),2)
        ax.plot(n,maxi,'go',)
        ax.plot(n,mini,'go')
        ax.plot([n,n],[mini,maxi],'g')
        ax.plot(n,moy,'bo')
        ax.text(n + 0.09,moy,"$\sigma={}$\nmoy={}\nmax={}\nmin={}".format(stand,moy,maxi,mini),bbox=dict(facecolor='wheat', alpha=0.7))
        mean_arr.append(moy)
        n+=1
    mean_numpy_arr = asarray(mean_arr)
    ax.plot(range(1,n),mean_numpy_arr,'b--')
    ax.axhline(amin(mean_numpy_arr))
    ax.set_xlabel('Number of thread [N]')
    ax.set_ylabel('Time [ms]')
    ax.text(0.05, 0.1, '{} ERREURS'.format(n_error), fontsize=10, transform=plt.gcf().transFigure)
    ax.set_title("Execution de fact avec l'exemple d'input sur un processeur " + cpuinfo.get_cpu_info()['brand'].split("w/")[0] ,pad=30)
    plt.show()



def exec_2 (Number_of_thread) : 
    t1 = time()
    a = CPrime.exec(Number_of_thread)
    print(a)
    t2 = time()
    time_taken = t2 - t1
    print(time_taken)
    return time_taken

def exec (Number_of_thread) : 
    execution = os.popen('./fact -N {} Input.txt Output.txt'.format(str(Number_of_thread))) #écrit dans le terminal
    time_taken_raw = execution.read() #lit ce que l'execution écris dans le terminal
    try :
        time_taken = float(time_taken_raw.strip())*1000#transforme en float et retire le "\n"
    except:
        time_taken = -1
    return time_taken

def main (number_of_exec,max_number_of_thread):
    big_array = []
    n_error = 0
    for n in range (max_number_of_thread):
        array_of_time_for_n_thread = []
        for i in range (number_of_exec):
            time_taken = exec(max_number_of_thread)
            print("i={}".format(i))
            if time_taken != -1 :
                array_of_time_for_n_thread.append(time_taken)
            else : 
                n_error += 1
        big_array.append(array_of_time_for_n_thread)
        print("n={}".format(n))
    grapher(big_array,n_error)

main(5, 8)