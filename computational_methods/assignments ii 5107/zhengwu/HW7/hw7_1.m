%answer for HW7
%problem 1
clear all;
close all;
 
t = [1 0.1 0.01 0.001]
for n=1:4
T = t(n); 
%T = 0.1; 0.01; 0.001;

a = 1.0;
s = a*sqrt(T)

N = 10/T;
walk(1) = 0;
for i=2:N
   
    U = rand;
    if ( U>0.5)
        walk(i) = walk(i-1) + s;
        
    else walk(i) = walk(i-1) -s;
    end;
    
end;

subplot(2,2,n)
hold on,stairs(walk);
end;