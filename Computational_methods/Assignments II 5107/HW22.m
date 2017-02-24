clear all; close all;
  
T = 10;
M = 30;
t = 0:0.1:T;
 
lambda = 2 + sin(t) + sin(2.*t)./2;

% display rate function
subplot(4,1,1);
plot(t, lambda);

%Ingegration of lambda(t)
F = inline('5/4 + 2*s - cos(s) - cos(2*s)/4');
%Derivative of F
DF = inline('2 + sin(s) + sin(2*s)/2');
 
for i=1:M
    
    k(i) = poissrnd(1*F(T));
    x = rand(1,k(i))*F(T);
    npp{i} = sort(x);
    
    %Newton-Raphson algorithm to estimate F^(-1)
 
    for j = 1:length(npp{i})
        
        x = T/2; % initial position
        ind = 0;
        while ind < 100
            ind = ind+1;
            x_new = x - (F(x)-npp{i}(j))/DF(x);
            d = abs(x_new-x);
            if d<1e-3;
                break;
            end
            x = x_new;
        end
        ipp{i}(j) = x; 
    end;
       
end;
 
for i = 1:30
subplot(4,1,2:4)
hold on;
plot(ipp{i},i,'.');
end;


