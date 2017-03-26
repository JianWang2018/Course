#include "integrate_simpson.h"
#include "integrate_trapezoid.h"

#include <vector>
using namespace std;

int main()
{
	Integrate* integs = new IntegrateSimpson [5];
	Integrate* integt = new IntegrateTrapezoid [5];

	vector<Integrate*> v;

	for (int i=0; i < 5; i++) {
		v.push_back(integs+i);
	}
	for (int i=0; i < 5; i++) {
		v.push_back(integt+i);
	}

	// v now has 5 Simpson integrators and 5 Trapezoidal integrators

	// Execute all integrators: 

	// This is not possible without polymorphism! Why? 
	// Polymorphism only works with pointers
	for (int i=0; i < v.size(); i++) {
		v[i]->execute();
	}

	// Now assume that you write a server, that accepts integrator
	// objects from the web (all kinds of integrators), and they all 
	// have the execute function. Polymorphism is what makes a 
	// a simple and clean implmentation possible!
}
