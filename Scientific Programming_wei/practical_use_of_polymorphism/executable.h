#ifndef _EXECUTABLE_H_
#define _EXECUTABLE_H_

// abstract class. Keyword "abstract" is not necessary. 
class Executable
{
public:
	// NO implementation, but polymorphic
	Executable() {}
	~Executable() {}
	virtual void execute() = 0;

	// all subclasses of Executable MUST implement an execute() command. 
};

#endif
