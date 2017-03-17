
# Default compiler set to use, see file make.<PLATFORM>-<COMP>
COMP = suncc

# C compilation flags (opt, warn, include, macro)
COFLAGS =
CWFALGS = -Wall
CIFLAGS = -I.
CMFLAGS =

# F77/F95 compilation flags (opt, warn, include, macro)
FOFLAGS =
FWFALGS = -Wall
FIFLAGS = -I.
FMFLAGS =

# Objects and libs to link
OBJS = cputime.o bench.o timeres.o
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
all:	timer sqmat sqmatb sqmatf sqmatw sqblas

timer:	cputime.o timeres.o timer.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)

sqmat:	$(OBJS) sqmat.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)

sqmatb:	$(OBJS) sqmatb.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)

sqmatf:	$(OBJS) sqmatfc.o sqmatf.o
	$(F95) $(FFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)

sqmatw:	$(OBJS) sqmatfc.o sqmatw.o
	$(F95) $(FFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)

sqblas:	$(OBJS) sqblas.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)

.c.o:	
	$(CC) $(CFLAGS) -c $<

.f.o:	
	$(F95) $(FFLAGS) -c $<

# Plotting: generate graph data (add new executables after plot:...)
.PHONY: plot
plot:	sqmat sqmatb sqmatf sqmatw sqblas
	./timing.sh $^

# Cleaning: remove non-essential files or clean all
.PHONY:	clean allclean
clean:
	rm -f *.o
allclean:	clean
	rm -f *.data timing.gnuplot timer sqmat sqmatb sqmatf sqmatw sqblas
