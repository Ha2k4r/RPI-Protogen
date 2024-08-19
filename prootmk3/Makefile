CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter
CXXFLAGS=$(CFLAGS)
OBJECTS=Matrix_Drw.o main.o
BINARIES=proot

# Where our library resides. You mostly only need to change the
# RGB_LIB_DISTRIBUTION, this is where the library is checked out.
RGB_LIB_DISTRIBUTION=$(CURDIR)
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread

# OpenCV flags
OPENCV_CXXFLAGS=$(shell pkg-config --cflags opencv4)
OPENCV_LDFLAGS=$(shell pkg-config --libs opencv4)

all: $(BINARIES)

$(RGB_LIBRARY): FORCE
	$(MAKE) -C $(RGB_LIBDIR)

proot: $(OBJECTS) $(RGB_LIBRARY)
	$(CXX) $(OBJECTS) -o proot $(LDFLAGS) $(OPENCV_LDFLAGS)

%.o: %.cc
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) $(OPENCV_CXXFLAGS) -c -o $@ $<

%.o: %.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) $(OPENCV_CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(BINARIES)

FORCE:
.PHONY: FORCE
