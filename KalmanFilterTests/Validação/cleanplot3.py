# reduce the number of points for square signals
import matplotlib.pyplot as plt
import numpy as np

linhanew = open('dadosnew.dat','w+')
timenew = []
valnew = []

# linha = open('dados.dat','r').read()
# l = linha.split('\n')   
time,value = np.loadtxt('dados.dat',unpack=True)

n = len(time)

timenew.append(time[0])
valnew.append(value[0])

for i in range (1,n-1):

    if(value[i]!=value[i-1] and value[i]==value[i+1]):
        timenew.append(time[i])
        valnew.append(value[i])
    elif(value[i]==value[i-1] and value[i]!=value[i+1]):
        timenew.append(time[i])
        valnew.append(value[i])

timenew.append(time[-1])
valnew.append(value[-1])
        
linhanew = open('dadosnew.dat','w+')

for i in range (len(timenew)):
    linhanew.write("%s %s\n" %(timenew[i],valnew[i]))