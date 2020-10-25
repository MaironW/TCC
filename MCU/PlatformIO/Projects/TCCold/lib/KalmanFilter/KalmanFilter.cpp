// KalmanFilter.h
// written by: Mairon de Souza Wolniewicz
// last update: 07/01/2020

#include <Eigen30.h>
#include <Eigen/LU>
#include <KalmanFilter.h>
#include <runtime.h>

Eigen::Matrix2d A,R,Q,I,AT;
Eigen::Vector2d B;

Eigen::Matrix2d K, S;
Eigen::Vector2d Z, y;

Eigen::Matrix2d P_N, P_E, P_D;
Eigen::Vector2d x_N, x_E, x_D;

float Rd,Rv,Qa;

Eigen::Vector2d x;
Eigen::Matrix2d P;

// LINEAR KALMAN FILTER 

void KalmanFilterInit(float dt){
    A << 1,dt,
         0,1;

    AT = A.transpose();
    
    B << 0.5*dt*dt, dt;
    
    I.setIdentity();

    x_N << 0,0;
    x_E << 0,0;
    x_D << 0,0;
    
    P_N.setIdentity()*10;
    P_E.setIdentity()*10;
    P_D.setIdentity()*10;
}

Eigen::Vector2d Linear1DKalmanFilter(float d, float v, float a, float dt, char direction){

    switch(direction){
        case 'N':
            Rd = Rd_N;
            Rv = Rv_N;
            x = x_N;
            P = P_N;
            break;
        case 'E':
            Rd = Rd_E;
            Rv = Rv_E;
            Qa = Qa_E;
            x = x_E;
            P = P_E;
            break;
        case 'D':
            Rd = Rd_D;
            Rv = Rv_D;
            Qa = Qa_D;
            x = x_D;
            P = P_D;
            break;
    }

    A << 1,dt,
         0,1;

    AT = A.transpose();
    
    B << 0.5*dt*dt, dt;

    R << Rd*Rd, 0,
         0, Rv*Rv;

    Q << 0.25*dt*dt*dt*dt, 0.5*dt*dt*dt,
         0.5*dt*dt*dt, dt*dt;
    Q << (Qa*Qa)*Q;

    Z << d,v;

    x = A*x + B*a;
    P = (A*P)*AT + Q;

    y = Z - x;

    S = P + R;
    K = P*S.inverse();

    x = x + K*y;
    P = (I-K)*P;

    return x;
}
