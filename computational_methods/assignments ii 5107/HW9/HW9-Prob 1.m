clear all; close all;
h = inline('sin(2*x+y)+2.*cos(x-2*y)+0.1*(x+y)','x','y');
dhx = inline('2*cos(2*x+y)-2*sin(x-2*y)+0.1','x','y');
dhy = inline('cos(2*x+y)+4*sin(x-2*y)+0.1','x','y');

x = -3:0.01:3;
y = -3:0.01:3;
 
for i=1:length(x)
    z(:,i)=h(x(i),y);
end

figure(1);
mesh(x,y,z);
xlabel('X','fontsize',14);
ylabel('Y','fontsize',14);

figure(2);
contour(x,y,z);
xlabel('X','fontsize',14);
ylabel('Y','fontsize',14);
colorbar;


alpha = 1;
%try initial point
theta(:,:,1) = [-1,-2];
h_value(1) = h(theta(1,1,1),theta(1,2,1));
 
for i = 2:100
    theta(:,:,i) = theta(:,:,i-1) + alpha*[dhx(theta(1,1,i-1),theta(1,2,i-1)),dhy(theta(1,1,i-1),theta(1,2,i-1))];
    h_value(i) = h(theta(1,1,i),theta(1,2,i));
end;
   
%display them
figure(3);
[X,Y] = meshgrid(x,y);
Z=h(X,Y);
contour(X,Y,Z); 
colorbar; 

hold on;
for i=1:100
plot(theta(1,1,i),theta(1,2,i),'r.');
hold on;
end;