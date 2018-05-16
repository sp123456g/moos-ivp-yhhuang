clear all;
clc;
%% Parameter
 dt = 0.05;
 df = 0.05;
t1 = [0:dt:10-dt];  %input
t2 = [10:dt:20-dt];  %input
t3 = [20:dt:30];  %input
t  = [0:dt:30];  %input
f  = [-5:df:5];  %input
x = [cos(2*pi*t1),cos(6*pi*t2),cos(4*pi*t3)]; %input
B = 1;  %input

y=recSTFT(x,t,f,B);


imagesc(t,f,abs(y));
axis xy;  %change the direction of y axis