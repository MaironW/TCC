import matplotlib.pyplot as plt
import numpy as np

from numpy import pi, cos, sin, sqrt, diag
from numpy.linalg import inv
from numpy.random import randn


t = np.linspace(0, 2*pi, 100)
dt = t[1] - t[0]

# position
x = 2*cos(t)
y = sin(2*t)

# velocity
dxdt = -2*sin(t)
dydt = 2*cos(2*t)

# accel
d2xdt2 = -2*cos(t)
d2ydt2 = -4*sin(2*t)

# jerk
d3xdt3 = 2*sin(t)
d3ydt3 = -8*cos(2*t)

# measurement error
gps_sig = 0.1
omega_sig = 0.3

# noisy measurements
x_gps = x + gps_sig * randn(*x.shape)
y_gps = y + gps_sig * randn(*y.shape)

A = np.array([
    [1, dt, (dt**2)/2, 0, 0, 0],
    [0, 1, dt, 0, 0, 0],
    [0, 0, 1, 0, 0, 0],
    [0, 0, 0, 1, dt, (dt**2)/2],
    [0, 0, 0, 0, 1, dt],
    [0, 0, 0, 0, 0, 1],
    ])

Q1 = np.array([(dt**3)/6, (dt**2)/2, dt, 0, 0, 0])
Q1 = np.expand_dims(Q1, 1)
Q2 = np.array([0, 0, 0, (dt**3)/6, (dt**2)/2, dt])
Q2 = np.expand_dims(Q2, 1)

j_var = max(np.var(d3xdt3), np.var(d3ydt3))

Q = j_var * (np.dot(Q1,np.transpose(Q1)) + np.dot(Q2,np.transpose(Q2)))

H = np.array([
    [1, 0, 0, 0, 0, 0],
    [0, 0, 0, 1, 0, 0],
    ])

R = diag(np.array([gps_sig**2, gps_sig**2]))

x_init = np.array([x[0], dxdt[0], d2xdt2[0], y[0], dydt[0], d2ydt2[0]])
P_init = 0.01 * np.eye(len(x_init))  # small initial prediction error

# create an observation vector of noisy GPS signals
observations = np.array([x_gps, y_gps]).T

# matrix dimensions
nx = Q.shape[0]
ny = R.shape[0]
nt = observations.shape[0]

# allocate identity matrix for re-use
Inx = np.eye(nx)

# allocate result matrices
x_pred = np.zeros((nt, nx))      # prediction of state vector
P_pred = np.zeros((nt, nx, nx))  # prediction error covariance matrix
x_est = np.zeros((nt, nx))       # estimation of state vector
P_est = np.zeros((nt, nx, nx))   # estimation error covariance matrix
K = np.zeros((nt, nx, ny))       # Kalman Gain

# set initial prediction
x_pred[0] = x_init
P_pred[0] = P_init

# for each time-step...
for i in range(nt):

    # prediction stage
    if i > 0:
        x_pred[i] = np.dot(A,x_est[i-1])
        P_pred[i] = np.dot(np.dot(A,P_est[i-1]),A.T) + Q

    # estimation stage
    y_obs = observations[i]

    K[i] = np.dot(np.dot(P_pred[i],H.T),        inv(np.dot(np.dot(H,P_pred[i]),H.T)+R))

    x_est[i] = x_pred[i] + np.dot(K[i],(y_obs - np.dot(H,x_pred[i])))
    P_est[i] = np.dot((Inx - np.dot(K[i],H)),P_pred[i])


plt.plot(x,y)
plt.plot(x_est[:,0],x_est[:,3])
plt.plot(x_gps,y_gps,'.')
plt.grid()
plt.show()