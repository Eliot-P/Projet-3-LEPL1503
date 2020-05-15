import os
from numpy import *
import matplotlib.pyplot as plt

plt.style.use('ggplot')

def grapher_time(array,n_error,In):
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
    ax.set_title("Execution de fact avec " + In + " les machines INGI",pad=30)
    plt.plot()
    plt.savefig('../IMAGES/Time_of_execution_INGI_'+In+'.png',bbox_inches='tight')
    plt.close()
    return mean_arr

def grapher_memory(array,n_error,In):
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
    ax2.set_title("Execution de fact avec " + In + " les machines INGI",pad=30)
    plt.plot()
    plt.savefig('../IMAGES/Memory_usage_INGI_'+In+'.png',bbox_inches='tight')
    plt.close()

def grapher_Amdahl(mean_arr,In) :
    ax = plt.subplot()
    Number_of_thread = arange(1,len(mean_arr)+1)
    Fraction_of_thread = 0.90
    Gain =  1/((1-Fraction_of_thread)+(Fraction_of_thread/Number_of_thread))
    ax.set_xlabel("Number of thread")
    ax.set_ylabel("Speed Up")
    ax.set_title("Loi de Amdahl " + In + " sur les machines INGI",pad=30)
    ax.plot(Number_of_thread,mean_arr[0]/mean_arr,'o--',label="Real World")
    ax.plot(Number_of_thread,Gain,"-",label="Amdahl's Law")
    plt.plot()
    plt.savefig('../IMAGES/Amdahl_INGI_'+In+'.png',bbox_inches='tight')
    plt.close()

def transcripter(filename):
    file = open(filename,"r")
    big_mem_array = []
    big_time_array = []
    temp_time_array = []
    temp_mem_array = []
    N_error = 0
    for line in file.readlines() :
        words = line.split(" ")
        if words[0] == "---\n" :
            big_mem_array.append(temp_mem_array)
            big_time_array.append(temp_time_array)
            temp_time_array = []
            temp_mem_array = []
        else :
            if words[0] == "Nombre": 
                N_error = float(words[-1])
                file.close()    
                return big_mem_array,big_time_array,N_error
            elif words[0] == "l'execution" : 
                temp_mem_array.append(float(words[10]))
                temp_time_array.append(float(words[7]))

def main(mesure) :
    mem_arr, time_arr, n_error = transcripter("../DATA/" + mesure)
    mean_arr = grapher_time(array=time_arr,n_error=n_error,In=mesure[:-13])
    grapher_memory(array=mem_arr,n_error=n_error,In=mesure[:-13])
    grapher_Amdahl(mean_arr=mean_arr,In=mesure[:-13])
    return 0

def get_input():
    mesure = input("Nom du fichier de mesure : ")
    main(mesure)

get_input()



