import numpy as np
import matplotlib.pyplot as plt

'''
Comparer les résultats de performance de notre code au performance théorique (loi de Amdahl)
'''
f = 0.90                   #fraction du programme qui est parallélisable
N = np.linspace(0, 8, 100) #nombre de threads
Gain =  1/((1-f)+(f/N))    #loi de Amdahl

plt.title("Comparaison à la théorie")
plt.plot(N,Gain, "-",label="Loi de Amdahl")
plt.plot([1,2,3,4,5,6,7,8], [1, 1.064, 1.062, 1.100, 1.030, 1.124, 1.120, 1.156], "o--",label="notre programme")
plt.xlabel('Number of Processors')
plt.ylabel('SpeedUp')
plt.axis([1, 10, 0, 6])
plt.legend()
plt.show()
