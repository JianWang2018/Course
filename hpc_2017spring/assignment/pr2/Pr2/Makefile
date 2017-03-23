
# Default matrix layout (see gauss.c)
LAYOUT = JAGBYCOL

# Default wall-clock timer
CMFLAGS = -DUSE_GETTIMEOFDAY
FMFLAGS = -DUSE_GETTIMEOFDAY

# Default compiler set to use, see file make.<PLATFORM>-<COMP>
COMP = suncc

# C compilation flags (opt, warn, include, macro)
COFLAGS = -O3
CWFALGS = -Wall
CIFLAGS = -I.

# F77/F95 compilation flags (opt, warn, include, macro)
FOFLAGS = -O3
FWFALGS = -Wall
FIFLAGS = -I.

# Objects and libs to link
OBJS = cputime.o
LIBS = -lm
LDFLAGS = 

# Include platform-specific settings from make.<PLATFORM>-<COMP>
# This may override all of the above
PLATFORM = $(shell ./config.guess)
include make.$(PLATFORM)-$(COMP)

# Extra flags to pass to program source code
CXFLAGS = "-DCOMPILER=\"$(CC)\"" "-DOFLAGS=\"$(COFLAGS)\"" "-DMFLAGS=\"$(CMFLAGS)\"" "-DPLATFORM=\"$(PLATFORM)\""
FXFLAGS = "-DCOMPILER=\"$(F95)\"" "-DOFLAGS=\"$(FOFLAGS)\"" "-DMFLAGS=\"$(CMFLAGS)\"" "-DPLATFORM=\"$(PLATFORM)\""

CFLAGS = $(CWFLAGS) $(COFLAGS) $(CIFLAGS) $(CMFLAGS) $(CXFLAGS)
FFLAGS = $(FWFLAGS) $(FOFLAGS) $(FIFLAGS) $(FMFLAGS) $(FXFLAGS)

# Build all: all executables to build
.PHONY: all
all:	gauss

gauss:	$(OBJS) gauss.c
	$(CC) -D$(LAYOUT) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)

gaussp:	$(OBJS) gauss.c
	$(CC) -xpg -D$(LAYOUT) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)

.c.o:	
	$(CC) $(CFLAGS) -c $<

.f.o:	
	$(F95) $(FFLAGS) -c $<

# prof: collect performance stats with Sun Performance Analyzer
.PHONY: prof gprof
prof:	gauss
	rm -rf prof.er
	collect -o prof.er -p on ./$^
	echo "Run 'analyzer prof.er' to view profile results"
gprof:	gaussp
	./gaussp
	gprof gaussp

# Cleaning: remove non-essential files or clean all
.PHONY:	clean allclean
clean:
	rm -rf *.o gmon.out prof.er
allclean:	clean
	rm -rf gauss gaussp
