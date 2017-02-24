
clear all; close all;

N=200;
u = 0.1*randn(1);
v = 0.1*randn(1);

x(1) = rand(1);
y(1) = x(1)^2 + v;

% simulate state Xt and observation Yt
for i=2:N
    u = 0.1*randn(1);
    v = 0.1*randn(1);
    x(i)=sqrt(abs(x(i-1))) + u;
    y(i)=x(i)^2+v;
end

% display Xt and Yt
figure (1);
subplot(311);
plot(x,'r','LineWidth',2);
title('state Xt');
subplot(312);
plot(y,'LineWidth',2);
title('observation state Yt');

% Sequential Monte Carolo method
% generate 100 samples
x_s(1,:) = rand(1,100);

for i=2:200     
 % generate the  prediction set
    u = 0.1*randn(1,100);
    x_pred(i,:)=sqrt(abs(x_s(i-1))) + u;
           
 % compute the weights and normalize
    v = 0.1*randn(1,100);
      
    w(i,:) = normpdf(y(i),x_pred(i,:).^2,0.1);
    w_n(i,:) = w(i,:)/sum(w(i,:));
    W_n(i,:) = cumsum(w_n(i,:));
    
    theta_hat(i,:)= sum(w_n(i,:).*x_pred(i,:));
      
 % resample
     for t=1:100
         U = rand;
         num = find(U-W_n(i,:)<0);
         x_s(i,t) = x_pred(i,num(1));
     end
end

subplot(3,1,3);
plot(1:200,x,'r-','LineWidth',2);
title('true states VS estimated states');
hold on;
plot(1:200,theta_hat,'-.', 'LineWidth',2);
  
              
%compute R_square
 X_est = theta_hat(11:200)';
 R_square= 1-sum((x(11:200)-X_est).^2)/sum((x(11:200)-mean(X_est)).^2);
     
% plot the histogram of the samples from posterior at t=50,100,150,200
figure (2);  

for k=1:4
    subplot(2,2,k);
    data=x_s(50*k,:);
    hist(data);
    [h(k),p(k)]=kstest((data-mean(data))/std(data));
end
p
h

     
 






