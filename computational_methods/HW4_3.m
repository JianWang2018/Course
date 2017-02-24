clear all, close all;
 
f0 = inline('(x.^2).*abs(sin(pi.*x.^1)).*exp(-x.^3)','x')
 
c=quad(f0,0,5);
f=inline('(x.^2).*abs(sin(pi.*x.^1)).*exp(-x.^3)/d','x','d');
 
figure(1); % plot the density function f(x)
t=0:0.01:5;
N=length(t);
plot(t,f(t,c),'b','linewidth',3);
title('f(x)');
 
K=10000;
x=zeros(1,K);
x(1)=0.5; %initial
for k = 2:K
    y = exprnd(1);
    rho = min((f(y,c)*exp(-x(k-1)))/(f(x(k-1),c)*exp(-y)),1);
    U = rand;
    x(k) = y*(U < rho)+ x(k-1)*(U > rho);
        
    exp_x(k) = mean(x);
    var_x(k) = var(x);
end
 
exp_x = mean(x);
var_x = var(x);
 
h=histc(x,t)/K;
 
figure(2);
bar(t,h);
hold on;
plot(t,f(t,c)/sum(f(t,c)),'r','linewidth',3);
