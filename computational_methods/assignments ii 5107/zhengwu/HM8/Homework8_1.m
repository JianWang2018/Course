%Homework 8
%problem 1
clear all;
close all;

load('hw8_1_data.mat');

%random selec 15;
ind = floor(rand(1,15)*length(X));

%new data
Data = X(ind(1,:),:);
clear ind;
%means and correlation coefficient;

mean_dt = mean(Data);

temp = corrcoef (Data(:,1), Data(:,2));
cc0 = temp(1, 2);


B = 2000;

%bootstrap
bx = zeros(15,2,B);
for i = 1:B
    ind = randsample(15,15,'true');
    bx(:,:,i) = Data(ind,:);
end;

%coef
for i = 1:B
    temp = corrcoef(bx(:,1,i),bx(:,2,i));
    cc(i) = temp(1,2);
end;
%se of coef
sdcc = std(cc)

%mean
btmean = mean(bx);
semean_lsat = std(bx(1,1,:))
semean_gpa = std(bx(1,2,:))

hist(cc)
