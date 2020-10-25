def eficiencia(est,gps):
    npoints = len(gps)
    neff = 0
    maior = 0

    for i in range(1,npoints):
        if(gps[i-1]<gps[i]):
            maior = 0
        elif(gps[i-1]>gps[i]):
            maior = 1

        if(maior==0):
            if(est[i-1]>=gps[i-1]):
                neff+=1
        if(maior==1):
            if(est[i-1]<gps[i-1]):
                neff+=1

    # verifica o ultimo ponto
    if(gps[-2]<gps[-1]):
            maior = 0
    elif(gps[-2]>gps[-1]):
        maior = 1

    if(maior==0):
        if(est[-1]>=gps[-1]):
            neff+=1
    if(maior==1):
        if(est[-1]<gps[-1]):
            neff+=1
        
    return neff/float(npoints)