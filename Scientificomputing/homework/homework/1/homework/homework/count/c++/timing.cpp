#include <stdio.h>
#include "timing.h"

//----------------------------------------------------------------------
void Timer::start(int type) 
{
	this->type = type;
	tStart = 0.0;

	if (type == CHILDREN) {
		tStart = etimeChildren();
	} else if (type == SELF) {
		tStart = etimeSelf();
	}
}

//----------------------------------------------------------------------
void Timer::end() 
{
	float tEnd;
	if (type == CHILDREN) {
		tEnd = etimeChildren();
	} else if (type == SELF) {
		tEnd = etimeSelf();
	}
	//printf("tStart, tEnd= %f, %f\n", (float) tStart, (float) tEnd);
	store.addTiming(name, tEnd - tStart);
	tStart = 0;
}

//----------------------------------------------------------------------
double Timer::etimeChildren()
{
    struct rusage ru;

    int status = getrusage(RUSAGECHILDREN, &ru);
	//printf("children status= %d\n", status);

    tmp[0] = ru.ru_utime.tv_sec+ ru.ru_utime.tv_usec * 1e-6;
    tmp[1] = ru.ru_stime.tv_sec+ ru.ru_stime.tv_usec * 1e-6;

    return (double) (tmp[0] + tmp[1]);
}

//----------------------------------------------------------------------
double Timer::etimeSelf()
{
    struct rusage ru;

    int status = getrusage(RUSAGESELF, &ru);
	//printf("self status= %d\n", status);

    tmp[0] = ru.ru_utime.tv_sec + ru.ru_utime.tv_usec * 1e-6;
    tmp[1] = ru.ru_stime.tv_sec + ru.ru_stime.tv_usec * 1e-6;

    return (double) (tmp[0] + tmp[1]);
}

//----------------------------------------------------------------------
Timer::~Timer() 
{
	if(tStart) {
		end();
	}
}

//----------------------------------------------------------------------
void Timings::addTiming(std::string name, double diff)
{
	Timing &last = timings[name];
		last.count ++;
		last.sum += diff;
		last.max = std::max(last.max, diff);
		last.min = std::min(last.min, diff);
}

//----------------------------------------------------------------------
void Timings::dumpTimings(int steps, int n)
{
	printf("\nTimings (given in milliseconds)\n");
	printf("Count\t\tN\t\tTotal\t\tMean\n");

	for(std::map<std::string, Timing >::const_iterator iter = timings.begin();
		iter != timings.end(); iter++) {
		printf("%d\t\t%d\t\t%.4f\t\t%.4f\n", iter->second.count, n, iter->second.sum*1000, (iter->second.sum/iter->second.count)*1000);

		/*
		std::cout << '"' << iter->first 
		<< "\",\"" << iter->second.count
		<< "\",\"" << iter->second.sum *1000
		<< "\",\"" << (iter->second.sum/iter->second.count)*1000
		<< "\",\"" << iter->second.min*1000
		<< "\",\"" << iter->second.max*1000 << "\"\n";
		*/
	}
}

//----------------------------------------------------------------------
