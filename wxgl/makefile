# Makefile

### global variable definition
CXX=g++
STRIP=strip

#CXXFLAGS += -O2 -fomit-frame-pointer -funroll-loops -finline-functions `wx-config --cxxflags`
CXXFLAGS += -O2 `wx-config --cxxflags`

LIBS=
WXWIN_LIBS = `wx-config --libs`

### guess platform
ifeq ($(shell uname -o),Cygwin)
	OPENGL_LIBS=-lglut32 `wx-config --gl-libs`
	EXE_SUFFIX=.exe
	CXXFLAGS+= -mfpmath=387 
	LIBS = $(OPENGL_LIBS) $(WXWIN_LIBS)
endif

ifeq ($(shell uname -s),Linux)
	OPENGL_LIBS=`wx-config --gl-libs` -lglut
	EXE_SUFFIX=
	CXXFLAGS+= -mfpmath=387
	LIBS = $(OPENGL_LIBS) $(WXWIN_LIBS) -L/usr/X11R6/lib -lXi -lXmu
endif

### targets
.PHONY: all
all: hello gl0 gl1

hello: hello.o
	$(CXX) -o $@ $^ $(LIBS)
	$(STRIP) $@$(EXE_SUFFIX)

gl0: gl0.o
	$(CXX) -o $@ $^ $(LIBS)
	$(STRIP) $@$(EXE_SUFFIX)

gl1: gl1.o
	$(CXX) -o $@ $^ $(LIBS)
	$(STRIP) $@$(EXE_SUFFIX)

.PHONY: clean
clean:
	find . -type f -maxdepth 1 -name "*.bak" -exec rm -f {} \;
	find . -type f -maxdepth 1 -name "*.o" -exec rm -f {} \;
	find . -type f -maxdepth 1 -name "*~" -exec rm -f {} \;
	find . -type f -maxdepth 1 -name "*.exe" -exec rm -f {} \;
	find . -type f -maxdepth 1 -name "*.stackdump" -exec rm -f {} \;
	find . -type f -maxdepth 1 -name "hello" -exec rm -f {} \;
	find . -type f -maxdepth 1 -name "gl0" -exec rm -f {} \;
	find . -type f -maxdepth 1 -name "gl1" -exec rm -f {} \;

### implicit rules
.cpp.o:
	$(CXX) $(CXXFLAGS) -o $@ -c $<

# EOF