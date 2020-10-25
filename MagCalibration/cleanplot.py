# reduce the number of points in Magnetometer plot

import numpy as np



linhanew = open('dadosnew.dat','w+')
lnew = []

linha = open('dados.dat','r').read()
l = linha.split('\n')   


n = len(l)

for i in range (n):
    try:
        lnew.append(l[i])
        #linhanew.write("%s\n" %l[i])
        for j in range (i+1,n):
            
            print i,j,n

            if(lnew[i]==l[j]):
                l.remove(l[j])
                print 'remove'
                n = len(l)
    except IndexError:
        i+=1
        
linhanew = open('dadosnew.dat','w+')

for i in range (len(lnew)):
    try:
        if(i%2==0):
            print i,len(lnew)
            lnew.remove(lnew[i])
        else:
            linhanew.write("%s\n" %lnew[i])
    except IndexError:
        break    
    


    
