%homework 3
% Problem 2
clear all;

f=inline('exp(-abs(x-6).^0.5)','x');

f0=inline('exp(-abs(x-6).^0.5-(x-5).^2./8)','x');

c=quad(f0,-2,14);

f1=inline('exp(-abs(x-6).^0.5-(x-5).^2./8)/c','x','c');
% the M-H algorithm
K=5000;
x=zeros(1,K);
x(1)=8;
t=-2:0.1:15;

for k=2:K
    y=5+2*randn(1); %nomal distribution with mu=5, var=4;
    rho=min(f(y)/f(x(k-1)),1);
    U=rand;
    x(k)=y*(U<rho) + x(k-1)*(U>rho);
end;

%compute histogram

h=histc(x,t)/K;

figure(2);
hold on; bar(t,h);hold on;
plot(t,f1(t,c)/sum(f1(t,c)),'b','linewidth',3);

ex=mean(x);
varx=var(x);