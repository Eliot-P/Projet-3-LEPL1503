import os
from numpy import *
import matplotlib.pyplot as plt

plt.style.use('ggplot')

def grapher_time(array,n_error):
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
    ax.set_title("Execution de fact avec l'exemple d'input sur les serveurs INGI ",pad=30)
    plt.draw()
    plt.savefig('../OTHER/Time_of_execution_INGI.png',bbox_inches='tight')
    plt.close()
    return mean_arr

def grapher_memory(array,n_error):
    n = 1
    ax2 = plt.subplot()
    arr = asarray(array)
    mean_arr = []
    for n_thread in arr : 
        stand = around(std(n_thread),2)
        maxi = around(amax(n_thread),2)
        mini = around(amin(n_thread),2)
        moy = around(mean(n_thread),2)
        ax2.plot(n,maxi,'go',)
        ax2.plot(n,mini,'go')
        ax2.plot([n,n],[mini,maxi],'g')
        ax2.plot(n,moy,'bo')
        ax2.text(n + 0.09,moy,"$\sigma={}$\nmoy={}\nmax={}\nmin={}".format(stand,moy,maxi,mini),bbox=dict(facecolor='wheat', alpha=0.7))
        mean_arr.append(moy)
        n+=1
    mean_numpy_arr = asarray(mean_arr)
    ax2.plot(range(1,n),mean_numpy_arr,'b--')
    ax2.axhline(amin(mean_numpy_arr))
    ax2.set_xlabel('Number of thread [N]')
    ax2.set_ylabel('Memory usage [kB]')
    ax2.text(0.05, 0.1, '{} ERREURS'.format(n_error), fontsize=10, transform=plt.gcf().transFigure)
    ax2.set_title("Execution de fact avec l'exemple d'input sur les serveurs INGI ",pad=30)
    plt.draw()
    plt.savefig('../OTHER/Memory_usage_INGI.png',bbox_inches='tight')
    plt.close()

def grapher_Amdahl(mean_arr) :
    ax = plt.subplot()
    Number_of_thread = arange(1,len(mean_arr)+1)
    Fraction_of_thread = 0.90
    Gain =  1/((1-Fraction_of_thread)+(Fraction_of_thread/Number_of_thread))
    ax.set_xlabel("Number of thread")
    ax.set_ylabel("Speed Up")
    ax.set_title("Amdahl's Law",pad=30)
    ax.plot(Number_of_thread,mean_arr/mean_arr[0],'o--',label="Real World")
    ax.plot(Number_of_thread,Gain,"-",label="Amdahl's Law")
    plt.draw()
    plt.savefig('../OTHER/Amdahl_INGI.png',bbox_inches='tight')
    plt.close()

def exec (Number_of_thread) : 
    execution = os.popen('./../SCRIPTS/fact -N {} -q ../IN-OUT/Input.txt ../IN-OUT/Output.txt'.format(str(Number_of_thread))) #écrit dans le terminal
    time_taken_raw = execution.readlines() #lit ce que l'execution écris dans le terminal
    print(time_taken_raw[3].split(" ")[3])
    print(time_taken_raw[2].split(" ")[3])
    try :
        time_taken = float(time_taken_raw[3].split(" ")[3])*1000#transforme en float et retire le "\n"
        memory_taken = float(time_taken_raw[2].split(" ")[3])
    except:
        time_taken = -1
    return time_taken,memory_taken

def main (number_of_exec,max_number_of_thread):
    big_array_of_time = []
    big_array_of_memory = []
    n_error = 0
    for n in range (max_number_of_thread):
        array_of_time_for_n_thread = []
        array_of_memory_for_n_thread = []
        for i in range (number_of_exec):
            time_taken,memory_taken = exec(max_number_of_thread)
            print("i={}".format(i))
            if time_taken != -1 :
                array_of_time_for_n_thread.append(time_taken)
                array_of_memory_for_n_thread.append(memory_taken)
            else : 
                n_error += 1
        big_array_of_time.append(array_of_time_for_n_thread)
        big_array_of_memory.append(array_of_memory_for_n_thread)
        print("n={}".format(n))
    mean_arr = grapher_time(big_array_of_time,n_error)
    grapher_memory(big_array_of_memory,n_error)
    grapher_Amdahl(mean_arr)
main(4,8)