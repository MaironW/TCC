import numpy as np
from numpy.linalg import inv

# simplified Kalman filter
def LinearKalmanFilter1D(d,v,a,Rd,Rv,Qa,dt):
    #================== KALMAN FILTER INIT

    # estimative uncertainty matrix
    P = np.array([
        [Rd*Rd,0],
        [0,Rv*Rv]])

    # measurement covariance matrix
    R = np.array([
        [Rd*Rd,0],
        [0,Rv*Rv]])

    # measurements
    Z = np.array([d,v]).T

    u = a # control signal

    # matrix dimensions
    nx = R.shape[0]
    ny = R.shape[0]
    nt = Z.shape[0]

    I = np.eye(nx)

    #inital estimative
    x_est = np.zeros((nt,nx))
    P_est = np.zeros((nt,nx,nx))

    K = np.zeros((nt,nx,ny))

    x_est[0] = np.array([d[0],v[0]])
    P_est[0] = P

    for i in range(nt):
        # dynamic matrix
        A = np.array([
        [1,dt[i]],   # position update
        [0,1]])   # velocity update

        # control matrix
        B = np.array([
        [0.5*dt[i]**2],
        [dt[i]]])
        
        # process noise covariance matrix
        Q = Qa*Qa*np.array([[0.25*dt[i]**4,0.5*dt[i]**3],
                        [0.5*dt[i]**3,dt[i]**2]])
        
        if i>0:
            x_est[i] = np.dot(A,x_est[i-1]) + B.T*u[i]
            P_est[i] = np.dot(np.dot(A,P_est[i-1]),A.T) + Q

        y = Z[i] - x_est[i]

        # Kalman gain
        S = P_est[i] + R
        K[i] = np.dot(P_est[i], inv(S))

        # prediction
        x_est[i] = x_est[i] + np.dot(K[i],y)
        P_est[i] = np.dot((I - K[i]),P_est[i])

    return x_est

