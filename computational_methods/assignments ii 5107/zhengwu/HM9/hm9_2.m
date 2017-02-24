%answer for homework 9
%problem 2

clear all;
close all;

f = inline('(x.*sin(20*y)+y.*sin(20*x)).^2.*cosh(sin(10*x).*x) + (x.*cos(10*y)-y.*sin(10*x)).^2.*cosh(cos(20*y).*y)','x','y');

[X Y] = meshgrid(-1:0.01:1);

figure(1);
Z = f(X,Y);

% mesh plot
meshc(X,Y,Z);

%for (b)
T(1) = 10;
sigma = [0.001 0; 0 0.001];

%initial point
theta(1,:)=[0 0];

for i=2:1000;
    
    %simulate chi
    z = mvnrnd(theta(i-1,:),sigma);
    
    temp = z;
    
    T(i) = T(i-1)*0.99;
    
    temp_p = f(temp(1),temp(2))-f(theta(i-1,1),theta(i-1,2));
    
    temp_tho = exp(temp_p/T(i));
    tho = min(1,temp_tho);
    
    r = rand;
    
    if(r>tho) theta(i,:) = theta(i-1,:);
    else theta(i,:) = temp;
    end;
    
end;

figure(2);
subplot(2,2,1)
contour(X,Y,Z); hold on;
colorbar;
for i=1:1000
    plot(theta(i,1),theta(i,2),'*r');
    hold on;
end;

%plot the function
subplot(2,2,2)
plot(1:1000,f(theta(:,1),theta(:,2)));


subplot(2,2,3)
plot(1:1000,theta(:,1));

subplot(2,2,4)
plot(1:1000,theta(:,2))

