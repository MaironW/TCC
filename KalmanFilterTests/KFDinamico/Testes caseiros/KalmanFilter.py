import numpy as np
from numpy.linalg import inv

def LinearKalmanFilter1D(d,v,a,Rd,Rv,Qa,dt):
    # dynamic matrix
    A = np.array([
        [1,dt],   # position update
        [0,1]])   # velocity update

    # control matrix
    B = np.array([
        [0.5*dt**2],
        [dt]])

    # measurement matrix
    H = np.array([
        [1,0],
        [0,1]])

    #================== KALMAN FILTER INIT

    # estimative uncertainty matrix
    P = np.array([
        [Rd*Rd,0],
        [0,Rv*Rv]])

    # measurement covariance matrix
    R = np.array([
        [Rd*Rd,0],
        [0,Rv*Rv]])

    # process noise covariance matrix
    Q = np.array([
        [Qa*Qa,0],
        [0,Qa*Qa]])
    
    # discrete noise
    Q = Qa*Qa*np.array([[0.25*dt**4,0.5*dt**3],
                          [0.5*dt**3,dt**2]])
    
    # continuous noise
    Q = Qa*Qa*np.array([[0.05*dt**5,0.125*dt**4],
                          [0.125*dt**4,dt**3/3.0]])


    # measurements
    Z = np.array([d,v]).T

    u = a # control signal

    # matrix dimensions
    nx = Q.shape[0]
    ny = R.shape[0]
    nt = Z.shape[0]

    I = np.eye(nx)

    #inital estimative
    x_init = np.array([d[0],v[0]])
    P_init = P

    x_pred = np.zeros((nt,nx))
    P_pred = np.zeros((nt,nx,nx))

    x_est = np.zeros((nt,nx))
    P_est = np.zeros((nt,nx,nx))

    K = np.zeros((nt,nx,ny))

    x_pred[0] = x_init
    P_pred[0] = P_init

    for i in range(nt):
        if i>0:
            x_pred[i] = np.dot(A,x_est[i-1]) + B.T*u[i]
            P_pred[i] = np.dot(np.dot(A,P_est[i-1]),A.T) + Q

        y = Z[i] - np.dot(H,x_pred[i])

        # Kalman gain
        S = np.dot(np.dot(H,P_pred[i]),H.T) + R
        K[i] = np.dot(np.dot(P_pred[i],H.T), inv(S))

        # prediction
        x_est[i] = x_pred[i] + np.dot(K[i],y)
        #P_est[i] = np.dot((I - np.dot(K[i],H)),P_pred[i])
        P_est[i] = np.dot(np.dot(I-K[i],P_pred[i]),(I-K[i]).T) + np.dot(np.dot(K[i],R),K[i].T)
    
    return x_est

def ExtendedKalmanFilter1D(d,v,a,Rd,Rv,Ra,dt):
    # dynamic matrix
    A = np.array([
        [1,dt,dt*dt*0.5],   # position update
        [0,1,dt],           # velocity update
        [0,0,1]])           # accel update

    # measurement matrix
    H = np.array([
        [1,dt,dt*dt*0.5],   # position update
        [0,1,dt],           # velocity update
        [0,0,1]])           # accel update

    #================== KALMAN FILTER INIT

    # estimative uncertainty matrix
    P = np.array([
        [Rd*Rd,0,0],
        [0,Rv*Rv,0],
        [0,0,Ra*Ra]])

    # measurement covariance matrix
    R = np.array([
        [Rd*Rd,0,0],
        [0,Rv*Rv,0],
        [0,0,Ra*Ra]])

    # process noise covariance matrix
    Q = np.array([
        [1e-5,0,0],
        [0,1e-5,0],
        [0,0,1e-5]])

    # measurements
    Z = np.array([d,v,a]).T

    # matrix dimensions
    nx = Q.shape[0]
    ny = R.shape[0]
    nt = Z.shape[0]

    I = np.eye(nx)

    #inital estimative
    x_init = np.array([d[0],v[0],a[0]])
    P_init = P

    x_pred = np.zeros((nt,nx))
    P_pred = np.zeros((nt,nx,nx))

    x_est = np.zeros((nt,nx))
    P_est = np.zeros((nt,nx,nx))

    K = np.zeros((nt,nx,ny))

    x_pred[0] = x_init
    P_pred[0] = P_init

    for i in range(nt):
        if i>0:
            x_pred[i] = np.dot(A,x_est[i-1]) #+ B.T*u[i]
            P_pred[i] = np.dot(np.dot(A,P_est[i-1]),A.T) + Q

        y = Z[i] - np.dot(H,x_pred[i])

        # Kalman gain
        S = np.dot(np.dot(H,P_pred[i]),H.T)+R
        K[i] = np.dot(np.dot(P_pred[i],H.T), inv(S))

        # prediction
        x_est[i] = x_pred[i] + np.dot(K[i],y)
        P_est[i] = np.dot((I - np.dot(K[i],H)),P_pred[i])

    return x_est
