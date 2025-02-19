% for homework 1
% problem 2

clear; close all;

%PI = [0.2 0.2 0.1 0.5; 0.1 0.3 0.4 0.2; 0.3 0.2 0.3 0.2; 0.1 0.3 0.1 0.5];  % transition matrix

PI = [0.5 0.5 0 0; 0.1 0.9 0 0; 0 0 0.3 0.7; 0 0 0.2 0.8];  % transition matrix

%PI = [0 0.5 0 0.5; 0.5 0 0.5 0; 0 0.5 0 0.5; 0.5 0 0.5 0];  % transition matrix
  
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

figure(1);
for m = 1:M
    subplot(4,1,m);
    plot(squeeze(p0(m,1,:)),'r');
        hold on, plot(1:1000,P(1),'-.r')
    hold on,plot(squeeze(p0(m,2,:)),'g');
        hold on, plot(1:1000,P(2),'-.g')
    hold on,plot(squeeze(p0(m,3,:)),'b');
        hold on, plot(1:1000,P(3),'-.b')
    hold on,plot(squeeze(p0(m,4,:)),'k');
        hold on, plot(1:1000,P(4),'-.k')
    ylim([0 1]);
end

