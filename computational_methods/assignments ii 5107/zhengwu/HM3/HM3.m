%homework 3 Problem1
% we are going to use exponential distribution as proposal denstiy

clear all;
f0=inline('x.^2.*abs(sin(pi*x)).*exp(-x.^3)','x');

c=quad(f0,0,4); 

f=inline('x.^2.*abs(sin(pi*x)).*exp(-x.^3)/c','x','c');

%q function

t=0:0.1:5;
%plot the density function of f(x);
plot(t,f(t,c));

% the M-H algorithm
K=5000;
x=zeros(1,K);
x(1)=0.5;

for k=2:K
    y=exprnd(1);
    rho=min((f(y,c)*exp(-x(k-1)))/(f(x(k-1),c)*exp(-y)),1);
    U=rand;
    x(k)=y*(U<rho) + x(k-1)*(U>rho);
end;

%compute histogram

h=histc(x,t)/K;

figure(2);
bar(t,h);hold on;
plot(t,f(t,c)/sum(f(t,c)),'m','linewidth',3);

ex=mean(x);
varx=var(x);