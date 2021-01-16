# TCC - Navigation system for unmaned aerial vehicle

This repository contains most of the files used by me, Mairon S. Wolniewicz, to develop the final project of my Aerospace Engineer undergraduation studies at Federal University of Santa Catarina.
The work was done with support of the [Automation and Control Systems Laboratory (LASC)](https://lasc.ufsc.br/) and coordinated by Prof. Dr. Alexandro Garro Brito.

The text in Portuguese with all the fundaments and metodologies can be found [here](https://repositorio.ufsc.br/handle/123456789/218497). While the video of the presentation can be found [here](https://www.youtube.com/watch?v=W5IHHzp5yvA&feature=youtu.be) and a demo video of the system running is [here](https://www.youtube.com/watch?v=oCGdunOda3U).

## Abstract

Aircraft has six degree of freedom dynamics: three translations and three rotations. 
The composition of signals from multiple sensors such as accelerometers, gyroscopes, magnetometers and GNSS must be used to determine attitude and displacement of an aerial vehicle. 
This work aims to solve the problem of aerospace navigation using methods of data fusion such as proposed by Madgwick and Kalman with low cost sensors. 
The system is embedded in a quadcopter for in flight testing and evaluation of algorithm efficiency. 
The attitude determined by the Madgwick filter performed well enough, while the Kalman filter requires improvements for an adequate displacement estimate.

## Repository contents

Below you can get a small description of what you will find in the folders of this repository:

- **AngleAnimation**: a Python script that generates animation for angle over time.
- **Comparação MPU Yost**: has a Python script to plot data of two sensors simultaneous for human eye analysis.
- **GPS**: has some GPS operation scripts, like bearing calculation, distance between coordinates, haversine formulae and live plot.
- **IMUCalibration**: can be used to get realtime or post processed data to calculate calibration values for the accelerometer and gyroscope. It can also generate  animation and .tex figures.
- **InterfacePython**: it contains various Python scripts, the serial and bluetooth communication ones are the most important.
* **KalmanAnimation**: has a Python script to animate already calculated Kalman Filter estimatives and their respective measurements.
- **KalmanFilterTests**: contains all the tests that involved Kalman filtering, from the ones I used to learn to the ones used for validation of my system.
- **MagCalibration**: scripts used to calibrate the magnetometer and generate magnetic field figures.
- **MCU**: all the code that runs inside a microcontroller (Tiva or ESP) is here.
- **Qt**: code for the navigation interface with compass, attitude indicator and map.
- **SDreader**: codes that use data collected by the vehicle during flight and written inside the SD cart.

Further description can be found inside the respective directories.

## Notice

As you will notice, the code suffers from heterogeneous coding practices, like a variety of naming patterns, as well as comments and C/C++ mixture.
This occurs mainly because I started this project almost four years before it's conclusion, in a time I was still learning the best coding practices. 

It is important to say that I stopped working on this project when it reached the state of 'working as expected', so it wasn't improved to the state of 'working the best way as possible' and it is up to whoever will use this software to do this job.

While not every script here have comments, you can contact me to explain how everything works.
