#ifndef _TORNADO_H_
#define _TORNADO_H_

class Tornado
{
public:
	Tornado() { printf("inside constructor\n"); }
	Tornado(const Tornado&) { printf("inside copy constructor\n"); }
	~Tornado() { printf("inside tornado destructor\n"); }

private:

};

#endif
