% for homework 2
% problem 2

clear; close all;

PI = [0.1 0.3 0.4 0.2; 0.2 0.4 0 0.4; 0 0.3 0.5 0.2; 0.5 0.3 0.2 0];  % transition matrix

f=[2.0 1.0 2.5 -1.0];

N=4; % state #
K=1000;

%generate the chain
 x(1,1) = ceil(4*rand);
    for k = 2:K
        % generate a chain
        P = PI(x(k-1,1),:); %pick i-th row
        U = rand;
        if U < P(1)
            x(k,1) = 1;
            
        elseif (P(1)<U&&U<(P(1)+P(2)))
            x(k,1) = 2;
            
        elseif((P(1)+P(2))<U&&U<(P(1)+P(2)+P(3)))
                    
                    x(k,1) = 3;
        else
            x(k,1) = N; 
        end
    end
 
[V, D] = eig(PI'); 
ind = find(abs(diag(D)-1)< 1e-6);
P = V(:,ind)/sum(V(:,ind));
    
% generate the estimator

for i=1:10:1000
Est1(i)=(1/i)*sum(f(x(1:i,1)));
Est2(i)=sum(f*P);
end;

plot(Est1(1:10:1000),'r');
hold on, plot(Est2(1:10:1000),'--b')
    
    
