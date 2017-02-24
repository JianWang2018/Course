
clear all; close all;

n=10;
x=randn(1,n);

theta = mean(x);

B = 2000;

for t = 1:50
    
    bx = zeros(1,n,B);
    for i = 1:B
        ind =  randsample(n,n,'true');
        bx(1,:,i) = x(1,ind);
    end
   
    for i = 1:B
        BTS_theta(i) = median(bx(1,:,i));
    end

    bias(t) = mean(BTS_theta)-theta;
    
end

hist(bias);
xlim([-0.15,0.15]);