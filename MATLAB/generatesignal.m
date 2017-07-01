% Это MATLAB скрипт, который предназначен для упрощения процесса создания
% файлов сложноструктурированных сигналов (pi-null-mezon, 2017)
%% User input
    dTms = 50; % Присвойте значение интервалу времени в десятых долях миллисекунды (любое число от 1 до 160)
    channels = 3; % Присвойте значение количеству цветовых каналов (либо 1, либо 3)
    red = 1; % Включить выключить цветовой канал (либо 0, либо 1), работает только при channels == 1
    green = 1; % Включить выключить цветовой канал (либо 0, либо 1), работает только при channels == 1
    blue = 1; % Включить выключить цветовой канал (либо 0, либо 1), работает только при channels == 1    

%% Do not change this values
countsperchannel = 333; % !limited by the Atmega328P SRAM volume, do not change
if channels == 1
	signal = zeros(3*countsperchannel,1,'uint8'); % !do not change
else
    signal = zeros(countsperchannel,1,'uint8'); % !do not change
end

%% Signal generation
if channels == 1 % one color channels case
    for i=1:3*countsperchannel % when we produce counts for the one channel we have 3 times higher resolution
       signal(i) = 255.0*(1.0 + sin(2*pi*i/(3*countsperchannel))/2.0); % signal
    end
else % three color channels case
    for i=1:countsperchannel
       signal(i) = 255*(1.0 + sin(2*pi*i/countsperchannel))/2.0; % red
    end
    for i=1:countsperchannel
       signal(i+countsperchannel) = 255*(1.0 + sin(2*pi*i*2/countsperchannel))/2.0; % green
    end
    for i=1:countsperchannel
       signal(i+2*countsperchannel) = 255*(1.0 + sin(2*pi*i*3/countsperchannel))/2.0; % blue
    end
end
%% Visualization
figure % new plot
vT = zeros(countsperchannel,1);
if(channels == 1)
    for i=1:3*countsperchannel
           vT(i) = i*dTms; 
    end
    plot(vT,signal,'b');
else
    for i=1:countsperchannel
           vT(i) = i*dTms; 
    end
    vr = signal(1:countsperchannel);
    vb = signal(countsperchannel+1:2*countsperchannel);
    vg = signal(2*countsperchannel+1:3*countsperchannel);
    hold on
    plot(vT, vr, 'r');
    plot(vT, vg, 'g');
    plot(vT, vb, 'b');
    legend('Красный','Зелёный','Синий');
end
grid on
xlabel('Время, мс');
ylabel('Яркость');
axis([0, inf, 0, 255]);

%% Write to the output text file
filename = uiputfile({'*.csv';'*.txt'},'Save as'); % Укажите куда сохранить выходной файл (сделано ч/з системный диалог)
fileID = fopen(filename,'w');
fprintf(fileID,'%s\r\n','t');
fprintf(fileID,'%u\r\n',dTms);
fprintf(fileID,'%s\r\n','c');
fprintf(fileID,'%u\r\n',channels);
fprintf(fileID,'%s\r\n','r');
fprintf(fileID,'%u\r\n',red);
fprintf(fileID,'%s\r\n','g');
fprintf(fileID,'%u\r\n',green);
fprintf(fileID,'%s\r\n','b');
fprintf(fileID,'%u\r\n',blue);
fprintf(fileID,'%s\r\n','s');
fprintf(fileID,'%u\r\n',signal);
fclose(fileID);



