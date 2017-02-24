clear all;close all;
 
lambda = 0.1;
T=100;
M=50;

%%%Method 1 for generating 50 samplepaths from exp(0.1)
for i=1:M
    x = exprnd(1/lambda, 2*lambda*T,50);
    y(:,i)= cumsum(x(:,i));
    
    y=y(:,i);
    PP{i} = y(y<T);
    hold on, 
    figure (3);
    plot(PP{i}',i,'.');
end;
 

%%%%Method 2 for generating 50 samplepaths from exp(0.1)
% 
% for i=1:M
%     n(i)=Poissrnd(lambda*T);
%     x=rand(1,n(i))*T;
%     PP{i}=sort(x);
%     hold on, 
%     figure (1);
%     plot(PP{i}',i,'.');
% end

figure(4);
HPP = [];

for i = 1:M
    HPP = [HPP PP{i}'];
end
hist(HPP,2.5:5:97.5);
ylim([0 60]);

 
figure (5);
for i=1:M
    r(i) = sum(PP{i}>10 & PP{i}<60);
end;
 
hist(r, 0:50);


%check whether sample follows Poisson distribution(KS test)
mu = mean(r);
x = 0:50;
est_cdf = poisscdf(x, mu);
[h,p] = kstest(r, [x' est_cdf']);
