% Problem 2
clear all, close all;
K=1000; % 1000 time steps
M=4;    % 4 samples
N=4;    % 4 states
pi=[0.2 0.2 0.1 0.5;0.1 0.3 0.4 0.2;0.3 0.2 0.3 0.2;0.1 0.3 0.1 0.5];
rand('seed',101);

for j=1:M
    i=randsample(1:4,1);
    for k=2:K-1
        x(k+1,j)= randsample(1:4,1,true,pi(i,:));
        i=x(k+1,j);
    end
   j=j+1;
end

% Part a
figure (2);
for m=1:M
  freq(:,m)=histc(x(:,m),1:N);
end
y=freq./1000;
plot(y);
xlabel('four states');
ylabel('probability');

% Plot the relative frequencies for four states
figure (3);
for m=1:M
    subplot(2,2,m);
    hist(x(:,m),1:N);
    xlabel('four states');
    ylabel('relative frequency');
end

% Part b
% produce eigenvalues (D) and eigenvectors (V) of matrix PI
[V,D]=eig(pi');
   
ind=find(abs(diag(D)-1)<1e-6);
for k=1:length(ind)
    % rde is the rescaled dominant eigenvector
    rde(:,k)=V(:,ind(k))/sum(V(:,ind(k)));
end

% Compare the estimated relative frequency with rde
figure(4);
% plot the relative frequency
plot(y(:,M),'--');
xlabel('four states');
ylabel('probability');
hold on;

% plot the dominant eigenvector of pi
plot(rde,'--rs');
legend('relative frequency','dominant eigenvector');
xlabel('four states');
ylabel('frequency');




   



