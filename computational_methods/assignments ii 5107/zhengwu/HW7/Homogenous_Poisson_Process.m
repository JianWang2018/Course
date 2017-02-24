
% Homogenous Poisson Process

clear all; close all; clc;

lambda = 0.2;
T = 100;
M = 30;

% % Method 1
% for i = 1:M
%     x = exprnd(1/lambda, 1, 2*lambda*T);
%     y = cumsum(x);
%     pp(i) = y(y<T);
% end

% Method 2
for i = 1:M
    n(i) = poissrnd(lambda*T);
    x = rand(1, n(i))*T;
    pp{i} = sort(x);
end

% plot the process
figure(1); clf;
for i = 1:M
    plot(pp{i}, i, 'bd');
    hold on;
    axis([0 T 0 M+1]);
end

figure(2);
PP = [];
for i = 1:M
    PP = [PP pp{i}];
end
hist(PP, 2.5:5:97.5);

for i = 1:M
    r(i) = sum(pp{i}>10 & pp{i}<80);
end
figure(3);
hist(r, 0:50);

% do the ks test
mu = mean(r);
x = 0:50;
est_cdf = poisscdf(x, mu);
[h,p] = kstest(r, [x' est_cdf']);