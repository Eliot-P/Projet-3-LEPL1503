import os

def txt_saver(time,memory,thread,execution,file) :
    string = "l'execution {} avec {} threads à prit {} [ms] et {} [kB] \n".format(execution,thread,time,memory)
    file.write(string)
    print(string)




def exec (Number_of_thread,In) : 
    execution = os.popen(('./../SCRIPTS/fact -N {} -q ../IN-OUT/' + In +' ../IN-OUT/Output_python.txt').format(str(Number_of_thread))) #écrit dans le terminal
    time_taken_raw = execution.readlines() #lit ce que l'execution écris dans le terminal
    try :
        time_taken = float(time_taken_raw[3].split(" ")[3])*1000#transforme en float et retire le "\n"
        memory_taken = float(time_taken_raw[2].split(" ")[3])
    except:
        time_taken = -1
    return time_taken,memory_taken

def main (number_of_exec,max_number_of_thread,In):
    n_error = 0
    fileplace = "../DATA/" + In[:-4] + "_time_out.txt"
    file = open(fileplace,"w+")
    for n in range (max_number_of_thread):
        array_of_time_for_n_thread = []
        array_of_memory_for_n_thread = []
        for i in range (number_of_exec):
            time_taken,memory_taken = exec(n+1,In)
            if time_taken != -1 :
                txt_saver(time=time_taken,memory=memory_taken,thread=n+1,execution=i+1,file=file)
            else : 
                error_string = "erreur à l'exectuion {} avec {} threads \n".format(n+1,i+1)
                file.write(error_string)
                print(error_string)
                n_error += 1
        file.write("---\n")
    file.write("Nombre d'erreur total = {}".format(n_error))
    file.close()

def get_input():
    number_of_exec = int(input("nombre d'exéctuion : "))
    max_number_of_thread = int(input("nombre de thread maximum : "))
    In = input("Nom du fichier d'input : ")
    main(number_of_exec,max_number_of_thread,In)
    return 0

get_input()