function varargout = GUI_spectrogramViewer(varargin)
% GUI_SPECTROGRAMVIEWER MATLAB code for GUI_spectrogramViewer.fig
%      GUI_SPECTROGRAMVIEWER, by itself, creates a new GUI_SPECTROGRAMVIEWER or raises the existing
%      singleton*.
%
%      H = GUI_SPECTROGRAMVIEWER returns the handle to a new GUI_SPECTROGRAMVIEWER or the handle to
%      the existing singleton*.
%
%      GUI_SPECTROGRAMVIEWER('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in GUI_SPECTROGRAMVIEWER.M with the given input arguments.
%
%      GUI_SPECTROGRAMVIEWER('Property','Value',...) creates a new GUI_SPECTROGRAMVIEWER or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before GUI_spectrogramViewer_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to GUI_spectrogramViewer_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help GUI_spectrogramViewer

% Last Modified by GUIDE v2.5 16-Nov-2018 15:27:40

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @GUI_spectrogramViewer_OpeningFcn, ...
                   'gui_OutputFcn',  @GUI_spectrogramViewer_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before GUI_spectrogramViewer is made visible.
function GUI_spectrogramViewer_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to GUI_spectrogramViewer (see VARARGIN)

% Choose default command line output for GUI_spectrogramViewer
handles.output = hObject;
handles.colorbarmax = '3';
handles.colorbarmin = '0';
handles.fs = '96000';
handles.style = 'jet';
handles.filepath = '~/moos-ivp-yhhuang/trunk/src/pWhistleDetector/Spectrogram_figure/';
% Update handles structure
set(gcf, 'color', [0.1 0.1 0.13]);
guidata(hObject, handles);


% UIWAIT makes GUI_spectrogramViewer wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = GUI_spectrogramViewer_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;



function edit1_Callback(hObject, eventdata, handles)
% hObject    handle to edit1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit1 as text
%        str2double(get(hObject,'String')) returns contents of edit1 as a double


% --- Executes during object creation, after setting all properties.
function edit1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit2_Callback(hObject, eventdata, handles)
% hObject    handle to edit2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit2 as text
%        str2double(get(hObject,'String')) returns contents of edit2 as a double


% --- Executes during object creation, after setting all properties.
function edit2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes during object creation, after setting all properties.
function axes2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to axes2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called



% Hint: place code in OpeningFcn to populate axes2


% --- Executes on button press in pushbutton1.
function pushbutton1_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% Real-time plotting 
% device setting
%----------------------------------------------------
    fs        = str2double(handles.fs);
    time_buff = 10;
    g = 0;
%----------------------------------------------------

while 1
if exist('./Spectrogram_data/original_P', 'file') == 2
input = importdata("./Spectrogram_data/original_P");
input_matrix_size = size(input);

input_time_axis_size = input_matrix_size(1,2);
input_frq_axis_size = input_matrix_size(1,1);
spectrogram_buff = zeros(input_frq_axis_size,input_time_axis_size*(time_buff-1));
detection_plot = zeros(input_frq_axis_size,input_time_axis_size*(time_buff-1));


while 1
    
set(handles.text4,'String','Plotting...');
A = importdata("./Spectrogram_data/original_P");
B = importdata("./Spectrogram_data/final_P");

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
colormap(handles.style);
hbar=colorbar();
% xlabel(hbar,'PSD (dB re 1\muPa^2 per Hz)');
% xlabel(hbar,'PSD');
axis xy;
title("Original spectrogram",'color','w');
% title("Spectrogram after median filter");
% title("Spectrogram before algorithm");
xlabel('Time (sec)','color','w');     
ylabel('Frequency (kHz)','color','w');
set(gca,'Xcolor','w');
set(gca,'Ycolor','w');
hbar.Color = 'w';
% set(hbar,'ytick',[0:5:50]);

caxis([str2double(handles.colorbarmin) str2double(handles.colorbarmax)]);
% caxis([0 5]);

% yticks([0:1:fs/2000]);

subplot(2,1,2);
imagesc(T,F/1000,detection_plot);

handles.P_origin_global = spectrogram_buff;
handles.P_final_global = detection_plot;
handles.T_global = T;
handles.F_global = F;

colormap(handles.style);
hbar=colorbar();
% xlabel(hbar,'PSD (dB re 1\muPa^2 per Hz)');
% xlabel(hbar,'PSD');dolphintest1.wav
axis xy;
title("Spectrogram after detection",'color','w');
% title("Spectrogram after median filter");
% title("Spectrogram before algorithm");
xlabel('Time (sec)','color','w');     
ylabel('Frequency (kHz)','color','w');     
% set(hbar,'ytick',[0:5:50]);
caxis([0 2]);
set(gca,'Xcolor','w');
set(gca,'Ycolor','w');
hbar.Color = 'w';
% yticks([0:1:fs/2000]);
pause(0.3);

guidata(hObject, handles);
    
end
end

else
    
    msg = strcat('waiting for data:',num2str(g),' s');
    set(handles.text4,'String',msg);
g = g+ 1;
    pause(1);
end
end



function edit3_Callback(hObject, eventdata, handles)
% hObject    handle to edit3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

aa_max = get(handles.edit3,'String');
handles.colorbarmax = aa_max;
guidata(hObject, handles);
disp(handles.colorbarmax);

% Hints: get(hObject,'String') returns contents of edit3 as text
%        str2double(get(hObject,'String')) returns contents of edit3 as a double


% --- Executes during object creation, after setting all properties.
function edit3_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit4_Callback(hObject, eventdata, handles)
% hObject    handle to edit4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit4 as text
%        str2double(get(hObject,'String')) returns contents of edit4 as a double


% --- Executes during object creation, after setting all properties.
function edit4_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

% --- Executes during object creation, after setting all properties.
function popupmenu3_CreateFcn(hObject, eventdata, handles)
% hObject    handle to popupmenu3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit6_Callback(hObject, eventdata, handles)
% hObject    handle to edit6 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit6 as text
%        str2double(get(hObject,'String')) returns contents of edit6 as a double
fs_local = get(handles.edit6,'String');
handles.fs = fs_local;
guidata(hObject, handles);


% --- Executes during object creation, after setting all properties.
function edit6_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit6 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pushbutton2.
function pushbutton2_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

f = figure('visible','off');
subplot(2,1,1);
imagesc(handles.T_global,handles.F_global/1000,handles.P_origin_global);
colormap(handles.style);
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
if isnan(handles.colorbarmax)
  set(handles.colorbarmax,'String','5');
end

if isnan(handles.colorbarmin)
  set(handles.colorbarmax,'String','0');
end

caxis([str2double(handles.colorbarmin) str2double(handles.colorbarmax)]);
% caxis([0 5]);

% yticks([0:1:fs/2000]);

subplot(2,1,2);
imagesc(handles.T_global,handles.F_global/1000,handles.P_final_global);

% imagesc(A);
colormap(handles.style);
hbar=colorbar();
% xlabel(hbar,'PSD (dB re 1\muPa^2 per Hz)');
% xlabel(hbar,'PSD');dolphintest1.wav
axis xy;
title("Spectrogram after detection");
% title("Spectrogram after median filter");
% title("Spectrogram before algorithm");
xlabel('Time (sec)');     
ylabel('Frequency (kHz)');     
% set(hbar,'ytick',[0:5:50]);
caxis([0 2]);
saveas(f,[handles.filepath , datestr(now,30),'_spectrogram'],'fig');
saveas(f,[handles.filepath , datestr(now,30),'_spectrogram'],'png');
set(handles.text4,'String','Save success, press Start to continue!');
disp("Save success!");
clear f;
pause(1.5);
guidata(hObject, handles);
% 

function edit7_Callback(hObject, eventdata, handles)
% hObject    handle to edit7 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit7 as text
%        str2double(get(hObject,'String')) returns contents of edit7 as a double
filepath_local = get(hObject,'String');
handles.filepath = filepath_local;
guidata(hObject, handles);



% --- Executes during object creation, after setting all properties.
function edit7_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit7 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes during object creation, after setting all properties.
function axes3_CreateFcn(hObject, eventdata, handles)
% hObject    handle to axes3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: place code in OpeningFcn to populate axes3


% --- Executes on mouse press over axes background.
function axes3_ButtonDownFcn(hObject, eventdata, handles)
% hObject    handle to axes3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes during object creation, after setting all properties.
function axes5_CreateFcn(hObject, eventdata, handles)

% hObject    handle to axes5 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: place code in OpeningFcn to populate axes5

