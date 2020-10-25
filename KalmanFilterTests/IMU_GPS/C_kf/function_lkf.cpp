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

Matrix2d A,R,Q,I,AT;
Vector2d B,Z;

Vector2d x;
Matrix2d P, K, S;
Vector2d y;

void KalmanFilterInit(float d, float v){
    A << 1,dt,
         0,1;

    AT = A.transpose();

    B << 0.5*dt*dt, dt;

    I.setIdentity();

    x << d,v;
    P.setIdentity();
}

void Linear1DKalmanFilter(float d, float v, float a, float Rd, float Rv, float Qa){
    float u = a;

    R << Rd*Rd,0,
         0,Rv*Rv;

    Q << Qa*Qa,0,
         0,Qa*Qa;

    Z << d,v;

    x = A*x + B*u;
    P = (A*P)*AT + Q;

    y = Z - x;

    S = P + R;
    K = P*S.inverse();

    // prediction
    x = x + K*y;
    P = (I-K)*P;

    cout<<x(0)<<','<<x(1)<<endl;
}


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

    KalmanFilterInit(d(0),v(0));
    for(i=0;i<772;i++){
        Linear1DKalmanFilter(d(i),v(i),a(i),Rd,Rv,Qa);
    }

}