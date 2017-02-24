clear all;close all;
rng(12345)
 
lambda = 0.1;
T=100;
M=50;

for i=1:M
    x1 = exprnd(1/lambda, 2*lambda*T,50);
    y1(:,i)= cumsum(x1(:,i));    
    y=y1(:,i);
    PP{i} = y(y<T);
    hold on, 
    figure (1);
    plot(PP{i}',i,'.');
end;

figure(2);
HPP = [];

for i = 1:M
    HPP = [HPP PP{i}'];
end
hist(HPP,2.5:5:97.5);
ylim([0 60]);

 
figure (3);
for i=1:M
    r(i) = sum(PP{i}>10 & PP{i}<60);
end;
 
hist(r, 0:50);


%check whether sample follows Poisson distribution(KS test)
mu = mean(r);
x = 0:50;
test_cdf = poisscdf(x, mu);
[h,p] = kstest(r, [x' test_cdf'])










