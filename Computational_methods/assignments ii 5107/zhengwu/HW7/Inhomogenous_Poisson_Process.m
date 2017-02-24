% inHomogenous Poisson Process

clear all; close all; clc;

T = 10;
M = 30;
F = inline('2*t-(cos(2*t)-1)/2');
dF = inline('2+sin(2*t)');

for i = 1:M
    
    % generate hpp in [0, F(T)]
    n(i) = poissrnd(1*F(T));
    x = rand(1, n(i))*F(T);
    hpp{i} = sort(x);
    
    % use Newton-Raphson method
    for j = 1:length(hpp{i})
        x = T/2; % initial position
        ind = 0;
        while ind < 100
            ind = ind+1;
            x_new = x - (F(x)-hpp{i}(j))/dF(x);
            d = abs(x_new-x);
            if d<1e-3;
                break;
            end
            x = x_new;
        end
        ipp{i}(j) = x;
    end
end

% hpp in [0 F(T)]
figure(1); clf;
for i = 1:M
    plot(hpp{i}, i, 'bd');
    hold on;
    axis([0 F(T) 0 M+1]);
end

% ipp in [0 T]
figure(2); clf;
subplot(411);
t = 0:0.1:T;
plot(t, dF(t));
subplot(4, 1, 2:4);
for i = 1:M
    plot(ipp{i}, i , 'bd');
    hold on;
    axis([0 T 0 M+1]);
end
    