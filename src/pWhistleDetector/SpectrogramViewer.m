clear all;clc;

% Real-time plotting 
% device setting
%----------------------------------------------------
    fs        = 96000;
    duration  = 1;
    time_buff = 10;
%----------------------------------------------------
input = importdata("Spectrogram_data/original_P");
input_matrix_size = size(input);

input_time_axis_size = input_matrix_size(1,2);
input_frq_axis_size = input_matrix_size(1,1);
spectrogram_buff = zeros(input_frq_axis_size,input_time_axis_size);
detection_plot = zeros(input_frq_axis_size,input_time_axis_size);

while 1

A = importdata("Spectrogram_data/original_P");
B = importdata("Spectrogram_data/final_P");

A_matrix_size = size(A);
B_matrix_size = size(B);

A_frq_axis_size = A_matrix_size(1,1);
B_frq_axis_size = B_matrix_size(1,1);

T=0:0.1:time_buff;   % time (second)
F=1:fs/2; % frequency (Hz)

if(length(spectrogram_buff)>= time_buff*input_time_axis_size)
   spectrogram_buff(:,1:input_time_axis_size) = [];
   detection_plot(:,1:input_time_axis_size) = [];
end

if(A_frq_axis_size == input_frq_axis_size && B_frq_axis_size == input_frq_axis_size)
    spectrogram_buff = [spectrogram_buff A];
    detection_plot   = [detection_plot B];
    
subplot(2,1,1);
imagesc(T,F/1000,spectrogram_buff);
colormap(jet);
hbar=colorbar();
% xlabel(hbar,'PSD (dB re 1\muPa^2 per Hz)');
% xlabel(hbar,'PSD');
axis xy;
title("Original spectrogram");
% title("Spectrogram after median filter");
% title("Spectrogram before algorithm");
xlabel('Time (sec)');     
ylabel('Frequency (kHz)');     
% set(hbar,'ytick',[0:5:50]);
caxis([0 5]);
colormap(jet);


subplot(2,1,2);
imagesc(T,F/1000,detection_plot);
% imagesc(A);

colormap(jet);
hbar=colorbar();
% xlabel(hbar,'PSD (dB re 1\muPa^2 per Hz)');
% xlabel(hbar,'PSD');
axis xy;
title("Spectrogram after detection");
% title("Spectrogram after median filter");
% title("Spectrogram before algorithm");
xlabel('Time (sec)');     
ylabel('Frequency (kHz)');     
% set(hbar,'ytick',[0:5:50]);
caxis([0 2]);
colormap(jet);
pause(0.5);
end
end
