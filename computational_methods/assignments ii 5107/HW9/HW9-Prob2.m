clear all; close all;
 
h = inline('(x.*sin(20*y)+y.*sin(20*x)).^2.*cosh(sin(10*x).*x) + (x.*cos(10*y)-y.*sin(10*x)).^2.*cosh(cos(20*y).*y)','x','y');
 
alpha=0.99;
x = -1:0.01:1;
y = -1:0.01:1;

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

% %% Optimization using M-H Simulated Annealing
T(1) = 10;
sigma = [.001 0; 0 .001];
 
%initial point
theta(1,:)=[0.5 0.5];
 
for i=2:1000;
    
    T(i) = T(i-1)*alpha;
    zeta = mvnrnd(theta(i-1,:),sigma);
    
    rho = min(1,exp((h(zeta(1),zeta(2))-h(theta(i-1,1),theta(i-1,2)))/T(i)));
    U = rand;
    
    if (U>rho) 
        theta(i,:) = theta(i-1,:);
    else
        theta(i,:) = zeta;
    end
    
end
 
figure (5);
subplot(2,2,1)
[X,Y]=meshgrid(x,y);
Z = h(X,Y);
contour(X,Y,Z); hold on;
colorbar;

for i=1:1000
    plot(theta(i,1),theta(i,2),'r.');
    hold on;
end;

%plot the function
subplot(2,2,2)
plot(1:1000,h(theta(:,1),theta(:,2)));
  
subplot(2,2,3)
plot(1:1000,theta(:,1));
 
subplot(2,2,4)
plot(1:1000,theta(:,2))
 
