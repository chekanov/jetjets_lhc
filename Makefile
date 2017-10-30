# S.Chekanov

ifndef PYTHIA8_DIR 
$(error PYTHIA8_DIR env variable is not set. Run setup.sh first)
endif

ifndef ROOTSYS
$(error ROOTSYS env variable is not set. Run setup.sh first)
endif

ifndef FASTJET
$(error FASTJET env variable is not set. Run setup.sh first)
endif


#LHAPDF6 configuration.
LHAPDF6_USE=true
LHAPDF6_BIN=/opt/hepsim/lhapdf/bin//
LHAPDF6_INCLUDE=/opt/hepsim/lhapdf/include/
LHAPDF6_LIB=/opt/hepsim/lhapdf/lib



include ${ROOTSYS}/etc/Makefile.arch


# Root variables
ROOTCFLAGS    = $(shell root-config --nonew --cflags)
ROOTLIBS      = $(shell root-config --nonew --libs)
ROOTGTTLIBS   = $(shell root-config --nonew --glibs)
CXXFLAGS     += $(ROOTCFLAGS)

LIBDIRARCH=lib/
OutPutOpt     = -o  
LIBS         += -L$(PYTHIA8_DIR)/$(LIBDIRARCH) -lpythia8 -lz 
LIBS         += -L$(FASTJET)/lib -lfastjet

SOURCE_FILES1 := $(shell ls -1 main.cxx)

INCLUDE1=-I./src
INCLUDE2=-I.
INCLUDE4=-I$(FASTJET)/include -Iinc
INCLUDE5=-I$(PYTHIA8_DIR)/include


# build object files 
objects1       = $(patsubst %.cxx,%.o,$(SOURCE_FILES1))

%.o: %.cxx 
	$(CXX) $(OPT) $(CXXFLAGS) $(INCLUDE1) $(INCLUDE2) $(INCLUDE4) $(INCLUDE5) -o $@ -c $<

Tasks:     clean main.exe

LIBOBJS = $(patsubst %.cc,%.o,$(SOURCE_FILES))

main.exe: $(objects1)
	$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@

clean:
	        @rm -f *.o *~ main.exe src/*.o; echo "Clear.." 
