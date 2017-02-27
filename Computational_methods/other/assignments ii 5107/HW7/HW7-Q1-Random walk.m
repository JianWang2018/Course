clear all; close all;

alpha = 1.0;
%T = 1;
% T = 0.1;
% T = 0.01;
T = 0.001;
s = alpha*sqrt(T);
samples = 20;

n = 10/T;

for k = 1:20
    
    for i=1:n
        z(i)=(2*floor(2*rand)-1)*s;
    end

    x= cumsum(z);
    x=[0 x];
    t=0:T:10;

    stairs(t,x);
    hold on;
    axis([0 10 -10 10]);
    xlabel('t');
    ylabel('sample path of Xt');
    
end