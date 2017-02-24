%Homework 8 
%Problem 2
clear all;
close all;

x = [1 2 3.5 4 7 7.3 8.6 12.4 13.8 18.1];

x_sort = sort(x);

x_trimmed = x_sort(3:8);

mean_trim = mean(x_trimmed);

%do bootstrap
for t=1:50
    
B = 2000;

bx = zeros(1,6,B);

for i=1:B
   ind = randsample(6,6,'true');
   bx(1,:,i) = x_trimmed(1,ind);
end;

%estimate the theta;
for i=1:B
    bt_theta(i) = mean(bx(1,:,i));
end;

%sd
bt_se(t) = std(bt_theta);
end;
std(bt_se)