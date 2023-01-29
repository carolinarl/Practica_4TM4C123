clear all;
close all;
clc; 

s = serialport('COM4', 57600, 'timeout', 1);
muestras = 1000; 
i = 1;

figure;
title('Grafica voltaje 6 ADC');
xlabel('muestras [k]');
ylabel('voltaje [V]');
grid on;
hold on;
ylim([0 4]);
xlim([0 100]);

while i<= muestras
   if mod(i,100) == 0
       xlim([i i+100])
   end
   
   % data = readline(device) reads ASCII data until the first occurrence of
   % the terminator from the Bluetooth® connection device and returns data 
   % as a string without the terminator. The function suspends MATLAB® 
   % execution until the terminator is reached or a timeout occurs.
   
   %% LECTURA DE LOS DATOS TIPO STRING
   str1 = readline(s);
   str2 = readline(s);
   str3 = readline(s);
   str4 = readline(s);
   str5 = readline(s);
   str6 = readline(s);
   
   % X = str2double(str) converts the text in str to double precision 
   % values. str contains text that represents real or complex numeric 
   % values. str can be a character vector, a cell array of character 
   % vectors, or a string array. If str is a character vector or string 
   % scalar, then X is a numeric scalar. If str is a cell array of 
   % character vectors or a string array, then X is a numeric array that is
   % the same size as str.
   
   %% CONVERSIÓN DE LA STRING EN NÚMERO
   data1 = str2double(str1);
   data2 = str2double(str2);
   data3 = str2double(str3);
   data4 = str2double(str4);
   data5 = str2double(str5);
   data6 = str2double(str6);
   
   %% CONVERSIÓN DE LOS DATOS A VOLTAJE
   % 12 bits de resolución del ADC
   volt1(i) = (data1*3.3)/4095;
   volt2(i) = (data2*3.3)/4095;
   volt3(i) = (data3*3.3)/4095;
   volt4(i) = (data4*3.3)/4095;
   volt5(i) = (data5*3.3)/4095;
   volt6(i) = (data6*3.3)/4095;
   
   
   %% GRÁFICA DE LOS 6 CANALES
   plot(volt1);
   drawnow
   plot(volt2);
   drawnow
   plot(volt3);
   drawnow
   plot(volt4);
   drawnow
   plot(volt5);
   drawnow
   plot(volt6);
   drawnow
end