rand1 = rand(200,19);
close all

rowMat =    [0,0,0,1,1,0,0,0;
            0,0,1,1,1,1,0,0;
            0,1,1,1,1,1,1,0;
            1,1,1,1,1,1,1,1;
            1,1,1,1,1,1,1,1;
            0,1,1,1,1,1,1,0;
            0,0,1,1,1,1,0,0;
            0,0,0,1,1,0,0,0]     
figure(1)
figSelect = 0;
matSize = 1;

TENGline = zeros(1,16);
timestamp = 1;
for i = 1:8
    for j = 1:8 
        figSelect = rowMat(i,j);
        TENGnum = TENGline(i)+TENGline(8+j);
        my_field = strcat('ts',num2str(matSize))
        TENG.(my_field) = TENGnum;
        if figSelect == 1
            subplot(8,8,matSize)
            plot(TENG.(my_field),'YDataSource',strcat('TENG.',my_field))
%             linkdata on
            title(strcat('E',num2str(i),'-E',num2str(j)))
        end
        drawnow limitrate
        matSize = matSize+1;
    end
end
matSize = 1;


% t = timer; 
% t.StartDelay = 1; 
% t.TimerFcn = @(~,~)drawnow; 
% t.Period = 1; %update every 1 sec.
% start(t)                        % start drawnow timer
dataGrabCount = 1;
stopCommand = 0;
timeChunk = 10;
while stopCommand < 1
    tic
    TENGline = rand1(dataGrabCount:dataGrabCount+(timeChunk-1),4:end);
    for i = 1:8
        for j = 1:8
            figSelect = rowMat(i,j);
            TENGnum(1:timeChunk) = TENGline(1:timeChunk,i)+TENGline(1:timeChunk,8+j);
            my_field = strcat('ts',num2str(matSize));
            if figSelect == 1
                TENG.(my_field)(1,end+1:end+timeChunk) = TENGnum;
                subplot(8,8,matSize)
                plot(TENG.(my_field))
                title(strcat('E',num2str(i),'-E',num2str(j)))
                
            end
            matSize = matSize+1;
        end
    end
    drawnow limitrate
    dataGrabCount = dataGrabCount+timeChunk;
    matSize = 1;
    if dataGrabCount == length(rand1)-1
        stopCommand = 1
    end
    toc
end
% stop(t) %stop timer