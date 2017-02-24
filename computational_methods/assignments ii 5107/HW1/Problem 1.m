% Problem 1
clear all;
K=200;      % 200 time steps
M=3;        % 3 samples
N=4;        % 4 states
pi=[0.2 0.2 0.5 0.1;0.2 0.3 0.4 0.1;0.4 0.2 0.3 0.1;0.1 0.0 0.0 0.9];
rand('seed',0);

for j=1:M
    i=randsample(1:4,1);
    for k=2:K-1
       x(k+1,j)= randsample(1:4,1,true,pi(i,:));
       i=x(k+1,j);
    end
   j=j+1;
end

for m=1:M
  freq(:,m)=histc(x(:,m),1:N);
end
y=freq./200;
figure (1);
plot(y);
xlabel('four states');
ylabel('probability');



   



