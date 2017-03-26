#include <limits> // For numeric_limits
#include <map>
#include <string>
#include <sys/time.h>
#include <sys/resource.h>
#include <iostream>

using namespace std;


extern __inline__ unsigned long long int rdtsc()
{
  unsigned long long int x;
  __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
  return x;
}

class Timings;

//----------------------------------------------------------------------
class Timer {
  Timings &store;
  std::string name;
  struct rusage ru;
  float tmp[2];
  int type;

  //unsigned long long tStart;
  double tStart;

public:
  enum {CHILDREN=0, SELF=1};
  enum {RUSAGESELF=0, RUSAGECHILDREN=-1};

public:
  Timer(Timings &st, std::string n) : store(st), name(n) 
  { tStart = 0.; }
  //{ start(); } 

  ~Timer();

  void start(int type=SELF);
  void end();
  double etimeSelf();
  double etimeChildren();
};

//----------------------------------------------------------------------

class Timings {
  struct Timing {
    int count;
    double sum;
    double min;
    double max;

    Timing() : count(0), sum(0), min(std::numeric_limits<double>::max()), max(0) {}
  };

  std::map<std::string, Timing > timings;

 public:
  Timings() {}

  void dumpTimings(int steps,int n);

  void addTiming(std::string name, double diff);
};

//----------------------------------------------------------------------
