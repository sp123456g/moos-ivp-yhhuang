function out_y = Gabor(x,tau,t,f,sgm)
%% Parameter
% Step 1 calculate parameter

input      = rot90(x);
dtau    = tau(1,2)-tau(1,1);
fs=1/dtau;
B = 1.9143/sqrt(sgm);
dt = t(1,2)-t(1,1);
df = f(1,2)-f(1,1);
n0 = t(1)/dt;
m0 = f(1)/df;
T  = length(t);
F  = length(f);
N  = round(1/(dtau*df));
C  = length(tau);
Q  = round(B*fs);
S  = dt/dtau;   %(unbalance form paramenter p.110)

  w_half= exp(-sgm*pi*[1/fs:1/fs:Q/fs].^2);           %only half gaussian
  w = [fliplr(w_half) 1 w_half];                 % Let gaussian be a real gaussian (1 is the middle point)

addzerox =[zeros(1,Q) input zeros(1,Q)]; %add zeros at front  and back of the x 

%% Loop    

for i=n0+1:S:n0+C
   x_split(round(i/S)+1,:) = addzerox(1,i:i+2*Q);   %split input to the length of frame  1 row split to 161 rows
end
for i=1:round(C/S)
x_win(i,:) = w.*x_split(i,:);   % every window*input
end

for n= 1:(n0+C)/S+1     
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
    out_y= ideal_spec.*Amp;
  toc
