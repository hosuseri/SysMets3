TARGET = sysmets3
OBJS = \
	sysmets3.o \
	MyWindow.o \
	sysmets.o

CC = gcc
CFLAGS = -Wall -O0 -g
CXX = g++
CXXFLAGS = $(CFLAGS)
LDFLAGS = -g
LIBS = -lgdi32

all: $(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

sysmets3.o: SysMets3.cpp
	$(CXX) $(CFLAGS) -c SysMets3.cpp

MyWindow.o: MyWindow.cpp MyWindow.h SysMets.h
	$(CXX) $(CFLAGS) -c MyWindow.cpp

sysmets.o: SysMets.cpp SysMets.h
	$(CXX) $(CFLAGS) -c SysMets.cpp
