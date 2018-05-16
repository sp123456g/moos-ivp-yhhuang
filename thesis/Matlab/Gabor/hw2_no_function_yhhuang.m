clear all;
clc;
tic
%% Parameter
[a1,fs] = audioread('Chord.wav');
x=a1(:,1);
x=rot90(x);

                                                     
dtau    = 1/fs;     %(input)
tau     = [0:length(x)-1]/fs; % total time
dt      = 0.01;  % time domain resulution (output)
df      = 1;     %frequency domain resulution
t       = [0:dt:max(tau)];  %input 
f       = [20:df:1000];
sgm     =200;                 %scale gabor transform
B = 1.9143/sqrt(sgm);  %input   (bandwidth)


%% 
% Step 1 calculate parameter
n0 = t(1)/dt;            % time domain begin point
m0 = f(1)/df;            % frequency begin point
T  = length(t);          % time total point 
F  = length(f);          % frequency total point
N  = round(1/(dtau*df)); % total input point
C  = length(tau);
Q  = round(B*fs);
S  = dt/dtau;   %(unbalance form paramenter p.110)

  w_half= exp(-sgm*pi*[1/fs:1/fs:Q/fs].^2);           %only half gaussian
  w = [fliplr(w_half) 1 w_half];                 % Let gaussian be a real gaussian (1 is the middle point)
plot(w);
addzerox =[zeros(1,Q) x zeros(1,Q)]; %add zeros at front  and back of the x 
%% Loop    
for i=n0+1:S:n0+C
   x_split(round(i/S)+1,:) = addzerox(1,i:i+2*Q);   %split input to the length of frame  1 row split to 161 rows
end
for i=1:round(C/S)
x_win(i,:) = w.*x_split(i,:);   % every window*input
end

for n= 1:(n0+C)/S+1     
     tic
  x1q(1,:) = x_win(n,:);  % Get every row of the x use 1 row to fft every time  

    X(n,:) = fft(x1q,N);   %   column: 1 < m < 400 , row: 0 <= n <= 600 
end

whole_spec = X';     %transfer X (?????b?O?W?v?????a?b?O?W?v)
for k=m0+1:m0+F
ideal_spec = whole_spec(f+1,:);    % ideal f is 21~1001
end
Amp=zeros(size(ideal_spec));    % ???O?@?????????P?????x?}
for ic = 1:F       % ???x?}??1:F????21~1001 Hz
    for ir =n0+1:n0+T
        Amp(ic,ir) = (sgm^(1/4))*dt*exp((j*2*pi*(Q-ir)*ic)/N);   % modified term of the fft
    end
end
    y= ideal_spec.*Amp;
    toc
%% Output the figure

imagesc(t,f,abs(y));
imagesc(abs(whole_spec));
axis xy;  %change the direction of y axis
toc
