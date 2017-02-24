clear all; close all;
 
load('C:\Documents and Settings\goober\Desktop\hw8_1_data.mat');

n=15; m=2;

%randomly select 15 sample pairs;
ind0 = floor(rand(1,15)*length(X));
x = X(ind0(1,:),:);

% estimate the means and correlation coefficient from the sampled data;
mean_x= mean(X);
temp = corrcoef (x(:,1), x(:,2));
cc0 = temp(1, 2);
 

% Bootstrap method to estimate the standard errors of these estimators
B = 25;

bx = zeros(n,m,B);
for i = 1:B
    ind = randsample(n,n,'true');
    bx(:,:,i) = x(ind,:);
end;
 
% calculate correlation coefficient
for i = 1:B
    temp = corrcoef(bx(:,1,i),bx(:,2,i));
    cc(i) = temp(1,2);
end;

% standard error of the correlation coefficient and means
sdcc = std(cc)


BST_mean = mean(bx);
SE_mean_LSAT = std(bx(1,1,:))
SE_mean_GPA = std(bx(1,2,:))
 
% histgram of bootstrap replicates of correlation coefficients
hist(cc)
