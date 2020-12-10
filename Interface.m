clear all
clc

rowMat =    [0,0,0,1,1,0,0,0;
            0,0,1,1,1,1,0,0;
            0,1,1,1,1,1,1,0;
            1,1,1,1,1,1,1,1;
            1,1,1,1,1,1,1,1;
            0,1,1,1,1,1,1,0;
            0,0,1,1,1,1,0,0;
            0,0,0,1,1,0,0,0];

arduinoObj = serialport("COM5",115200);
configureTerminator(arduinoObj,"CR/LF");
% configureTerminator(arduinoObj,"LF");
flush(arduinoObj);
% write(arduinoObj, 5, "uint8");
% arduinoObj.UserData = struct("TENG",[],"ACC",[],"Count", 1);

% configureCallback(arduinoObj,"terminator",@readData);

figure

for i= 1:1000
tic
[TENG, ACC] = readData(arduinoObj, 1);
t1 = TENG(1:8);
t2 = TENG(9:16);

matrix = (t1'*t2)/(675^2);
matrix = matrix.*rowMat;

Z = zeros(9,9);
% index = matrix > 0;
x=1:9;
y=1:9;
[X,Y]=meshgrid(x,y);
Z(1:8,1:8)=matrix;
surf(X,Y,Z);
view(2);
toc
% arduinoObj.NumBytesAvailable
end

