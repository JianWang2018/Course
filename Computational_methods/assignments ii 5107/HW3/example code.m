clear all;
close all;
f0=inline('x.^4.*exp(-x.^3.)');

c=quad(f0,0,5);

f=inline('x.^4.*exp(-x.^3.)/d','x','d');

figure(1); % display density function f(x)
t=0:0.01:5;
N=length(t);
plot(t,f(t,c));
title('function of f(x)');

K=5000;
x=zeros(1,K);
x(1)=0.5; %initial
rand('seed',0);
for k = 2:K
    y = exprnd(1);
    rho = min((f(y,c)*exp(-x(k-1)))/(f(x(k-1),c)*exp(-y)),1);
    U = rand;
    
    x(k) = y*(U < rho)+ x(k-1)*(U > rho);

end

h=histc(x,t)/K;

figure(2);
bar(t,h);
hold on;
plot(t, f(t,c)/sum(f(t,c)),'r','linewidth',3);
