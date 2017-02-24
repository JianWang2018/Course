% for homework 1
% problem 1

clear; close all;

PI = [0.2 0.2 0.5 0.1; 0.2 0.3 0.4 0.1; 0.4 0.2 0.3 0.1; 0.1 0 0 0.9];  % transition matrix
  
M = 3;      % number of chains
N = 4;      % number of states
K = 200;    % number of time steps in each chain

for m = 1:M
    x(1,m) = ceil(N*rand);  % random initial
    for k = 2:K
        % generate a chain
        P = PI(x(k-1,m),:); %pick i-th row
        U = rand;
        if U < P(1)
            x(k,m) = 1;
            
        elseif (P(1)<U&&U<P(1)+P(2))
            x(k,m) = 2;
            
        elseif(P(1)+P(2)<U&&U<P(1)+P(2)+P(3))
                    
                    x(k,m) = 3;
        else
            x(k,m) = N; 
        end
    end
end

figure(1);
for m = 1:M
    subplot(3,1,m);
    plot(x(:,m));
    ylim([0 4]);
end




