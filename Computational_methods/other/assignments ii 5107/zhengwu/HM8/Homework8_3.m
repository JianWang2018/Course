%answer for hm8 problem 3



clear all;
close all;

n = 20;
x = randn(1,n);

theta = mean(x);

%do bootstrap

B = 2000;

for t =1:50

bx = zeros(1,n,B);

for i=1:B
   ind = randsample(n,n,'true');
   bx(1,:,i) = x(1,ind);
end;

%estimate the theta;
for i=1:B
    bt_theta(i) = median(bx(1,:,i));
end;

biases(t) = mean(bt_theta)-theta;

end;

hist(biases)
axis([-0.08 0.08 0 12]);