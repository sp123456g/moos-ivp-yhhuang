clear all;
clc;
%% Parameter
[a1,fs] = audioread('chirp.wav');
x=a1(:,1);
tau     = [0:length(x)-1]/fs;
dt      = 0.01;
df      = 1;
t       = [0:dt:max(tau)];  %input
f       = [20:df:1000];
sgm     =200;

tic
y=Gabor(x,tau,t,f,sgm);
toc

imagesc(t,f,abs(y));
axis xy;  %change the direction of y axis