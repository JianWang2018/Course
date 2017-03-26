--Introduction:

Build classes, including a base class and two derived classes. In the base class, including two virtual functions: f and fDerivative, which will be overrided in the derived classes.
And writing a loop of Newton's method to find the root of functions.

--Design and Implementation

In the RealFunction, we design a loop to find out the root of our functions using Newton's method:

x_(n+1) = x_(n)-f(x_n)/f'(x_n)

As the method converges on the root, the difference between the root and the approximation is squared at each step. However, there are some difficulties with the method:
1.Difficulty in calculating derivative of a function, as it requires that the derivative be calculated directly.
2.Failure of the method to converge to the root. For the situations where the method fails to converge, it is because the assumptions made in the proof of quadratic convergence of Newton's method are not met.
3.Overshoot. If the first derivative is not well behaved in the neighborhood of a particular root, the method may overshoot, and diverge from that root.
4.Stationary point. If a stationary point of the function is encountered, the derivative is zero and the method will terminate due to division by zero.
5.Poor initial estimate. A large error in the initial estimate can contribute to non-convergence of the algorithm.
6.Mitigation of non-convergence. In a robust implementation of Newton's method, it is common to place limits on the number of iterations, bound the solution to an interval known to contain the root, and combine the method with a more robust root finding method.
7.Slow convergence for roots of multiplicity greater than 1.

Newton's method is only guaranteed to converge if the assumptions made in the proof of quadratic convergence are met. For the following subsections, failure of the method to converge will happen.

1.Bad starting points.
2.Iteration point is stationary.
3.Starting point enters a cycle.
4.Derivative issues. If the function is not continuously differentiable in a neighborhood of the root then it is possible that Newton's method will always diverge and fail, unless the solution is guessed on the first try.
5.Derivative does not exist at root.
6.Discontinuous derivative.
7.Non-quadratic convergence.
8.Zero derivative. For example, f(x)=x^2 , convergence will not be quadratic in this case.
9.No second derivative.

The following is the psuedo-code of using Newton's method to find a root of a function f which has derivative fDerivative.
The initial guess will be x0=2.0 
Each new iterative of Newton's method will be denoted by x1. We will check during the computation whether the denominator becomes too small, since otherwise a large amount of error could be introduced.

double epsilon = pow(10.0, -14);
double x0=2.0;
if(abs(fDerivative(x0)) < epsilon)
	std::cout << " Warning: denominator is too small" << std::endl;
	break;
end;
	double x1 = x0-f(x0)/fDerivative(x0);
if(fabs(x1-x0)<0.000001) break;
x0 = x1;
end;	
return x0;

Need to be carefully choose the initial value x0.

In my code there are three classes:
Base class -- RealFunction.h RealFunction.cpp -- define virture functions f(double)=0 and fDerivative(double)=0, define member function getRoot() which using the Newton's method.
Derived class -- SinFunction.h SinFunction.cpp -- define function f(x)=sin(x), fDerivative(x)=cos(x). And override the base class mehtod. 
Derived class -- MyFunction.h MyFunction.cpp -- define function f(x)=x*x-1, fDerivative(x)=2*x.
main function -- main.cpp -- instantiate an object of SinFunction and an object of MyFunction, call getRoot() to find the result.

--Results
In the SinFunction class, define function f(x)=sin(x), fDerivative(x)=cos(x).
In the MyFunction class, define function f(x)=x*x-1, fDerivative(x)=2*x.
Override the virtual functions f and fDerivative in the RealFunction class. And return the result of getRoot().

--Conclusions
Learned polymorphism, the relationship between base class and derived classes, and using the virtual functions for their dynamic binding.
Learned how to use Newton's method to get the root of a function, especially one satisfying teh quadratic convergence for Newton's iterative method.



