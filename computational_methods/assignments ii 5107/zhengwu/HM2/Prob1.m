% for homework 2
% problem 1

clear; close all;

%PI = [0.1 0.3 0.4 0.2; 0.2 0.1 0.3 0.4; 0.4 0.2 0.1 0.3; 0.3 0.4 0.2 0.1];  % transition matrix
PI = [0.1 0.3 0.4 0.2; 0.2 0.4 0 0.4; 0 0.3 0.5 0.2; 0.5 0.3 0.2 0]; 

M = 4;      % number of chains
N = 4;      % number of states
K = 1000;    % number of time steps in each chain

for m = 1:M
    %x(1,m) = m;  % random initial
     x(1,m) = ceil(4*rand);
    for k = 2:K
        % generate a chain
        P = PI(x(k-1,m),:); %pick i-th row
        U = rand;
        if U < P(1)
            x(k,m) = 1;
            
        elseif (P(1)<U&&U<(P(1)+P(2)))
            x(k,m) = 2;
            
        elseif((P(1)+P(2))<U&&U<(P(1)+P(2)+P(3)))
                    
                    x(k,m) = 3;
        else
            x(k,m) = N; 
        end
        for n=1:N
        p0(m,n,k) = sum(x(:,m)==n)/k;
        end;
    end
end

plot(squeeze(p0(1,1,1:10:1000)),'r');
hold on,plot(squeeze(p0(1,2,1:10:1000)),'-g');
hold on,plot(squeeze(p0(1,3,1:10:1000)),'-.b');
hold on,plot(squeeze(p0(1,4,1:10:1000)),'-*k');


% Final Relative frequencies

for m = 1:M
    for n = 1:N
        p(m,n) = sum(x(:,m)==n);
    end
end
p = p/K;  % normalization

[V, D] = eig(PI'); 
ind = find(abs(diag(D)-1)< 1e-6);
P = V(:,ind)/sum(V(:,ind));

