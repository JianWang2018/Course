clear all;
 
K=50000;
M=4;    
N=4;    
pi= [0.1 0.3 0.4 0.2;0.2 0.4 0.0 0.4;0.0 0.3 0.5 0.2;0.5 0.3 0.2 0.0];

rand('seed',101);
 
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
y=freq./K;
plot(y);
 
% produce eigenvalues (D) and eigenvectors (V) of matrix PI
[V,D]=eig(pi');
   
ind=find(abs(diag(D)-1)<1e-6);
for k=1:length(ind)
    % nv is the rescaled dominant eigenvector
    nv(:,k)=V(:,ind(k))/sum(V(:,ind(k)));
end
 
 
f=[2.0 1.0 2.5 -1.0];
right = dot(f,nv);
 
x(1)=ceil(4*rand);
 
for i=1:K
    [m,n]= sort(pi(x(i),:));
    sign=0;
    j=1;
    u=rand;
    while sign==0;
        if u<sum(m(1:j));
            x(i+1)=n(j);
            sign=1;
        else
            j=j+1;
        end
    end
end
 
for i=1:K
    left(i)=sum(f(x(i)));
end
 
avgleft=sum(left)/K;
