clear all, close all;

K=1000;
M=4;
N=4;
pi=[0.5 0.5 0.0 0.0;0.1 0.9 0.0 0.0;0.0 0.0 0.3 0.7;0.0 0.0 0.2 0.8];
%pi=[0.0 0.5 0.0 0.5;0.5 0.0 0.5 0.0;0.0 0.5 0.0 0.5;0.5 0.0 0.5 0.0];

rand('seed',0);

for j=1:M
    i=j;
   % randsample(1:4,1);
    for k=2:K-1
        x(k+1,j)= randsample(1:4,1,true,pi(i,:));
        i=x(k+1,j);
    end
   j=j+1;
end

figure (5);
for m=1:M
  freq(:,m)=histc(x(:,m),1:N);
end
y=freq./1000;
plot(y,'LineWidth',2);
xlabel('four states');
ylabel('probability');

figure (6);
for m=1:M
    subplot(2,2,m);
    hist(x(:,m),1:N);
    xlabel('four states');
    ylabel('relative frequency');
end

% produce eigenvalues (D) and eigenvectors (V) of matrix pi
[V,D]=eig(pi');
   
ind=find(abs(diag(D)-1)<1e-6);
for k=1:length(ind)
    % rde is the rescaled dominant eigenvector
    rde(:,k)=V(:,ind(k))/sum(V(:,ind(k)));
end

% plot the relative freency
figure (7);
plot(y(:,M),'--','LineWidth',3);
xlabel('four states');
ylabel('probability');
hold on;

% plot the dominant eigenvector of pi
plot(rde,'--rs');
legend('relative frequency','dominant eigenvector');
xlabel('four states');
ylabel('frequency');





   



