function output_y = recSTFT(x,t,f,B)
tic
dt = (max(t)-min(t))/(length(t)-1);
df = (max(f)-min(f))/(length(f)-1);
n0 = t(1)/dt;
m0 = f(1)/df;
T  = length(t);
F  = length(f);
N  = round(1/(dt*df));
Q  = B/dt;
w(1,1:2*Q)=1;   %window function
 addzerox =[zeros(1,Q) x zeros(1,Q)]; %add zeros at front  and back of the x 
%% Loop
for n=n0:n0+T-1
    for q=1:2*Q
    x1q(1,q)=w(1,2*Q+1-q)*addzerox(1,n+q); %convolution  x1(q)=w((Q-q)dt)x(n-Q+q)dt) n-Q< n-Q+q <n+Q === n<n+q<n+2Q
    end
    X(n+1,:) = fft(x1q,N);   %   column: 1 < m < 400 , row: 0 <= n <= 600
    for m=m0:m0+F-1
        m1=mod(m,N)+1;
    X_mod(n+1,m1) = abs(X(n+1,m1))*dt*exp((j*2*pi*(Q-n)*m)/N);   %X(n,m)    +frequency:m  -frequency:m+N
    end  
end
%% mapping the negative and positive frequency 

positive_freq = X_mod(:,1:m0+F-1);
if(m0<0)
negative_freq = X_mod(:,m0+1+N:-1+N+1);
end

y=[negative_freq positive_freq];  % Because using FFT will let the signal be periodic, put m+N to negative if m is -

%% Output the figure
%change the axis of the output 橫軸是m 縱軸是n 要轉成 橫軸是n 縱軸是 m 
output_y = rot90(y);
toc