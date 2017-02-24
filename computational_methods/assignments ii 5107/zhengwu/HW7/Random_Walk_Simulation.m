% Random walk simulation

clear all; close all; clc;

alpha = 1.0;
T = 1;

s = alpha*sqrt(T);

n = 10/T;

for i = 1:n
    z(i) = (2*floor(2*rand)-1)*s;
end

x = cumsum(z);
x = [0, x];

t = 0:T:10;
stairs(t,x);