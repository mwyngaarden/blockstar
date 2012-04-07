CXX = g++
CXXFLAGS = -Wall -Wextra $(shell sdl-config --cflags) -O2 -std=c++0x
LDFLAGS = $(shell sdl-config --libs) -lSDL_image -lSDL_ttf -lSDL_gfx
OBJS = blockstar.o main.o utility.o

all: blockstar

blockstar: $(OBJS)
	$(CXX) $(OBJS) -o blockstar $(LDFLAGS)

.cpp.o:	
	$(CXX) $(CXXFLAGS) -c $<
  
clean:
	rm *.o
