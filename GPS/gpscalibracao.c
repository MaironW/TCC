for(i=0;i<2000;i++){
            UpdateDataGPS();        // get new data from GPS
            dNvar+=vN;
            dEvar+=vE;
            dDvar+=vD;
            char buffer[100];
            sprintf(buffer,"%i\t%8f\t%8f\t%8f\t%8.2f\t%8.2f\t%8.2f\t\n",i,dNvar,dEvar,dDvar,vN,vE,vD);
            UARTprintf("%s",buffer);
        }
        dNvar/=2000;
        dEvar/=2000;
        dDvar/=2000;
        char buffer[100];
        sprintf(buffer,"%8f\t%8f\t%8f\t\n",dNvar,dEvar,dDvar);
        UARTprintf("%s",buffer);
        break;