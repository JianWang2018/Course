#ifndef _TORNADO_H_
#define _TORNADO_H_

class Tornado
{
private:
	float wind_speed;

public:
	Tornado() { wind_speed = -1.; printf("inside default constructor\n"); }
	Tornado(float speed) { wind_speed = speed; printf("inside constructor\n"); }
	Tornado(const Tornado& t) { 
		this->wind_speed = t.wind_speed;
		printf("inside copy constructor\n");
	}
	~Tornado() { printf("inside tornado destructor\n"); }

	float getSpeed() { return wind_speed; }

private:

};

#endif
