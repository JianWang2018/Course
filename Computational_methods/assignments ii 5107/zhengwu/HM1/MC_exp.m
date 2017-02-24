% example code on Markov Chain

clear; close all;

PI = [0.1 0.9; 0.8 0.2];  % transition matrix
  
M = 3;      % number of chains
N = 2;      % number of states
K = 100;    % number of time steps in each chain

for m = 1:M
    x(1,m) = ceil(N*rand);  % random initial
    for k = 2:K
        % generate a chain based on the Markov property
        P = PI(x(k-1,m),:);
        U = rand;
        if U < P(1)
            x(k,m) = 1;
        else
            x(k,m) = N; 
        end
    end
end

figure(1);
for m = 1:M
    subplot(3,1,m);
    plot(x(:,m));
    ylim([0 3]);
end

% compute the simulated probability
for m = 1:M
    for n = 1:N
        p(m,n) = sum(x(:,m)==n);
    end
end
p = p/K;  % normalization

% compute the true probability
[V, D] = eig(PI');     % compute eigenvalues
ind = find(abs(diag(D)-1)< 1e-6); % find the eigenvalue 1
P = V(:,ind)/sum(V(:,ind)); % rescale the eigenvector


