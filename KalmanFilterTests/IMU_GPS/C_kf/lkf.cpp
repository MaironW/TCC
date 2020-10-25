// Linear Kalman Filter implementation in C
// Mairon de Souza Wolniewicz
// 05/01/2020

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include <Eigen/Dense>

using namespace Eigen;
using namespace std;

float dt = 0.021;
float Rd = 2.5;
float Rv = 0.1;
float Qa = 0.006;


const char* getfield(char* line, int num){
    const char* tok;
    for (tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",\n")){
        if (!--num) return tok;
    }
    return NULL;
}

int main(){

    VectorXd a(772);
    VectorXd d(772);
    VectorXd v(772);

    // abrir arquivo com os dados
    FILE *fp;
    fp = fopen("sensor.dat","r");

    // separar dados em vetores a v d
    char line[1024];
    int i=0;
    while (fgets(line, 1024, fp)){
        char* tmp = strdup(line);
        a(i) = atof(getfield(tmp, 1));
        tmp = strdup(line);
        d(i) = atof(getfield(tmp, 2));
        tmp = strdup(line);
        v(i) = atof(getfield(tmp, 3));
        i++;
        free(tmp);
    }
    fclose(fp); // fechar o arquivo

    // ==============================================

    Matrix2d A,H,R,Q,P,I,AT;
    Vector2d B;
    MatrixXd Z(772,2);
    VectorXd u=a;

    Vector2d x_pred, x_est;
    Matrix2d P_pred, P_est, K, S;
    Vector2d y;

    A << 1,dt,
         0,1;

    AT = A.transpose();

    B << 0.5*dt*dt, dt;

    I.setIdentity();
    H.setIdentity();
    P.setIdentity();

    R << Rd*Rd,0,
         0,Rv*Rv;

    Q << Qa*Qa,0,
         0,Qa*Qa;

    Z << d,v;

    int nt = Z.rows();

    // initial estimative
    x_pred << d(0),v(0);
    P_pred << P;

    for(int i=0;i<nt;i++){
        if(i>0){
            x_pred = A*x_est + B*u(i);
            P_pred = (A*P_est)*AT + Q;
        }

        y = Z.block<1,2>(i,0).transpose() - x_pred;

        S = P_pred + R;
        K = (P_pred*H)*S.inverse();

        // prediction
        x_est = x_pred + K*y;
        P_est = (I-K)*P_pred;

        cout<<x_est(0)<<','<<x_est(1)<<endl;
    }
}