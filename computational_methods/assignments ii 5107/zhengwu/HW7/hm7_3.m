%%answer for HW7
%problem 3

clear all;
close all;


%interval = 0.1
T = 10;
t = 0:0.1:T;

namta = 2 + sin(t) + sin(2.*t)./2;


%F(s)

f = inline('5/4 + 2*s - cos(s) - cos(2*s)/4');

df = inline('2 + sin(s) + sin(2*s)/2');

% sample {t1, t2, ... tn}
M = 30;

for i=1:M
    
    % sample {t1, t2, ... tn}
    s(i) = poissrnd(f(T));
    x = rand(1,s(i))*f(T);
    tmp{i} = sort(x);
    %out put f-1(t);
    %N-R algorithm
    L = length(tmp{i});
    
    for j = 1:L
        
        x = T/2; % initial position
        ind = 0;
        while ind < 100
            ind = ind+1;
            x_new = x - (f(x)-tmp{i}(j))/df(x);
            d = abs(x_new-x);
            if d<1e-3;
                break;
            end
            x = x_new;
        end
        ipp{i}(j) = x; 
    end;
       
end;

subplot(4,1,1);
plot(t, namta);
for i = 1:30
subplot(4,1,2:4)
hold on,plot(ipp{i},i,'*');
end;




    

