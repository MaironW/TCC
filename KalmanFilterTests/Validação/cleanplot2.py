# reduce the number of points by half

import numpy as np

linhanew = open('dadosnew.dat','w+')
lnew = []

linha = open('dados.dat','r').read()
l = linha.split('\n')   


n = len(l)

temp = '0'

for i in range (n):
    if(i%2==0):
        lnew.append(l[i])

        
linhanew = open('dadosnew.dat','w+')

for i in range (len(lnew)):
    linhanew.write("%s\n" %lnew[i])
    


    
