
clear all; close all;

N=100;
u = 10*randn(1);
v = randn(1);

x(1) = 10*rand(1);
y(1) = x(1)^2/20 + 2*x(1) + v;

% simulate state Xt and observation Yt
for i=2:N
    u = 10*randn(1);
    v = randn(1);
    x(i) = (1/2)*(x(i-1)) + 25*x(i-1)/(1+x(i-1)^2) + 8*cos(1.2*i)+u;
    y(i) = x(i)^2/20 + 2*x(i) + v;
end

% display Xt and Yt
figure (3);
subplot(311);
plot(x,'r','LineWidth',2);
title('state Xt');
subplot(312);
plot(y,'LineWidth',2);
title('observation state Yt');

% Sequential Monte Carlo method
% generate 500 samples
x_s(1,:) = 10*rand(1,500);

for i=2:N
  % generate the  prediction set
    u = 10*randn(1,500);
    x_pred(i,:) = (1/2).*(x_s(i-1,:)) + 25.*x_s(i-1,:)./(1+x_s(i-1,:).^2) + 8*cos(1.2*i)+ u;
      
     
  % compute the weights and normalize
    v = randn(1,500);
      
    w(i,:) = normpdf(y(i),x_pred(i,:).^2./20+2.*x_pred(i,:),1);
    w_n(i,:) = w(i,:)/sum(w(i,:));
    W_n(i,:) = cumsum(w_n(i,:));
    
    theta_hat(i,:)= sum(w_n(i,:).*x_pred(i,:));
      
  %  resample
      for t=1:500
          U = rand;
          
          num = find(U-W_n(i,:)<0);
          x_s(i,t) = x_pred(i,num(1));
      end
end


subplot(3,1,3);
plot(1:100,x,'r-','LineWidth',2);
title('true states VS estimated states');
hold on;
plot(1:100,theta_hat,'-.', 'LineWidth',2);
  
              
%compute R_square
X_est = theta_hat(11:100)';
R_square= 1-sum((x(11:100)-X_est).^2)/sum((x(11:100)-mean(X_est)).^2);
     
   
     
% plot the histogram of the samples from posterior at t=25,50,75,100
figure (4); 

for k=1:4
    subplot(2,2,k);
    data=x_s(25*k,:);
    hist(data);
    [h(k),p(k)]=kstest((data-mean(data))/std(data));
end
p
h

     
 


