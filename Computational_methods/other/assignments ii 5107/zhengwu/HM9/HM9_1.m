%Homework 9 Problem 1

clear all;
close all;
f = inline('sin(2*x+y)+2.*cos(x-2*y)+0.1*(x+y)','x','y');
dx = inline('2*cos(2*x+y)-2*sin(x-2*y)+0.1','x','y');
dy = inline('cos(2*x+y)+4*sin(x-2*y)+0.1','x','y');
x = -3:0.01:3;
y = -3:0.01:3;

%mesh plot

[X,Y]= meshgrid(x,y);
Z = f(X,Y);
figure(1);
surf(X,Y,Z);
shading interp;

%coutour ...

%....
%...

%Gradient of decent
alpha = 1;
%initial point
theta(:,:,1) = [2,-1];

f_value(1) = f(theta(1,1,1),theta(1,2,1));

for i = 2:100
    theta(:,:,i) = theta(:,:,i-1) + alpha * [dx(theta(1,1,i-1),theta(1,2,i-1)),dy(theta(1,1,i-1),theta(1,2,i-1))];
    
    %compute the 
    f_value(i) = f(theta(1,1,i),theta(1,2,i));
end;
   
%display them
figure(2)
contour(X,Y,Z); 
colorbar; 
hold on;
for i=1:100
plot(theta(1,1,i),theta(1,2,i),'r*'); hold on;
end;

