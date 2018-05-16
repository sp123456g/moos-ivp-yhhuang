clear all;
clc;
%% Parameter
dt = 0.05;
df = 0.05;
t1 = [0:dt:10];  %input
t2 = [10:dt:20];  %input
t3 = [20:dt:30];  %input
t  = [0:dt:30];  %input
f  = [-5:df:5];  %input
x = [cos(2*pi*t1),cos(6*pi*t2),cos(4*pi*t3)]; %input

B = 1;  %input window length  (1 sec here)
%% 
% Step 1 calculate parameter
n0 = t(1)/dt;
m0 = f(1)/df;
T  = length(t);        % input time points
F  = length(f);        % input frequency points
N  = round(1/(dt*df)); % FFT based method constraints
Q  = B/dt;             % digital band width Q  total length = 2*Q

w(1,1:2*Q)=1;   % window function
addzerox =[zeros(1,Q) x zeros(1,Q)]; 
%add zeros at front  and back of the x , due to the window length is 2*Q
%first and last time can just have Q length.

%% Loop
for n=n0:n0+T-1
    %step1 window function convolution with input signal
    for q=1:2*Q
    x1q(1,q)=w(1,2*Q+1-q)*addzerox(1,n+q); %convolution  x1(q)=w((Q-q)dt)x(n-Q+q)dt) n-Q< n-Q+q <n+Q === n<n+q<n+2Q
    end
    %step2 use x1q to do fft
    X(n+1,:) = fft(x1q,N);   %   column: 1 < m < 400 , row: 0 <= n <= 600
    %step3 x has modified term in every loop.
    for m=m0:m0+F-1
        m1=mod(m,N)+1;
    X_mod(n+1,m1) = abs(X(n+1,m1))*dt*exp((j*2*pi*(Q-n)*m)/N);   %X(n,m)    +frequency:m  -frequency:m+N
    end  
end
% the output of the loop is the matrix: column is 1~400 is frequency but
% only 0~200 can be use, because another half is negative frequency
% row is 1~600 : time ouptut points


%% mapping the negative and positive frequency 

positive_freq = X_mod(:,1:m0+F-1);
if(m0<0)
negative_freq = X_mod(:,m0+1+N:-1+N+1);
end

y=[negative_freq positive_freq];  % Because using FFT will let the signal be periodic, put m+N to negative if m is -
%y=[positive_freq];
%% Output the figure
%change the axis of the output ???b?Om ?a?b?On ?n???? ???b?On ?a?b?O m 
rot_y = rot90(y);
% output the result
imagesc(t,f,abs(rot_y));
axis xy;  %change the direction of y axis
