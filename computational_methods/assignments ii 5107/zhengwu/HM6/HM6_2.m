%solution for HM 6
% Problem 2
close all;
clear all;

N = 100;

x(1) = 10*rand(1);

%sample from V
tmpv = randn(1);
y(1) = x(1)^2/20 + 2*x(1) + tmpv;

for i = 2:N
    
    %sample for ut
    tmp = 10*randn(1);
    x(i) = (1/2)*(x(i-1)) + 25*x(i-1)/(1+x(i-1)^2) + 8*cos(1.2*i)+tmp;
    
    tmpv = randn(1);
    y(i) = x(i)^2/20 + 2*x(i) + tmpv;
end;

%display xt and yt;

plot(x,'r','LineWidth',2);
hold on, plot(y,'-.','LineWidth',2);


%for part b

% for function g

g = inline('x','x');
%generate one hundred samples
x_e(1,:) = 10*rand(1,500);

for i=2:N
    
    %generate the prediction
    tmp = 10*randn(1,500);
    x_tilte(i,:) = (1/2).*(x_e(i-1,:)) + 25.*x_e(i-1,:)./(1+x_e(i-1,:).^2) + 8*cos(1.2*i)+tmp;
    
    %compute the weights
    tmpv = randn(1,500);
    %w(i,:) = x_tilte(i,:).^2 + tmpv;
    
    %normalize the weight;
    w(i,:) = normpdf(y(i),x_tilte(i,:).^2./20+2.*x_tilte(i,:),1);
    w_n(i,:) = w(i,:)/sum(w(i,:));
    W_n(i,:) = cumsum(w_n(i,:));
    
    %estimate theta
    theta_hat(i,:) = sum(w_n(i,:).*x_tilte(i,:));
    
    %resample
    for t = 1:500
        U = rand;
        ind = find(U-W_n(i,:)<0);
        x_e(i,t) = x_tilte(i,ind(1));
    end;
   % xx(i,:) = sum(w_n(i,:).*g(x_e(i,:)))
    
end;

%plot the estimation
figure(2);

%subplot(2,1,1)
plot(1:100,x,'r-')
hold on, plot(1:100,theta_hat,'-.');
%hold on, plot(1:200,x,'r-.');

%compute the R^2

X_r = theta_hat(11:100)';
M_X = mean(X_r);
R_X= 1-sum((x(11:100)-X_r).^2)/sum((x(11:100)-M_X).^2);
    

%plot the histogram of the values
figure(3),
subplot(2,2,1);
hist(x_e(25,:));
%standardized the data
temp_x = x_e(25,:);
temp_x = temp_x-mean(temp_x);
temp_x = temp_x/sqrt(var(temp_x));
h(1) = kstest(temp_x);

subplot(2,2,2);
hist(x_e(50,:));
temp_x = x_e(50,:);
temp_x = temp_x-mean(temp_x);
temp_x = temp_x/sqrt(var(temp_x));
h(2) = kstest(temp_x);

subplot(2,2,3);
hist(x_e(75,:));

temp_x = x_e(75,:);
temp_x = temp_x-mean(temp_x);
temp_x = temp_x/sqrt(var(temp_x));
h(3) = kstest(temp_x);

subplot(2,2,4);
hist(x_e(100,:));
temp_x = x_e(100,:);
temp_x = temp_x-mean(temp_x);
temp_x = temp_x/sqrt(var(temp_x));
h(4) = kstest(temp_x);

