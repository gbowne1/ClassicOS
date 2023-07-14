# Compiler and linker options
CC = gcc
CXX = g++
LD = ld
NASM = nasm
CFLAGS = -Wall -O2 -std=c17
CXXFLAGS = -Wall -O2 -std=c++20
LDFLAGS = -melf_i386
NASMFLAGS = -f elf32

# Target architecture
ARCH = i386

# Libraries
LIBS = -lm

# Source files
SRCS = $(wildcard *.c) $(wildcard *.cpp) $(wildcard *.asm)
OBJS = $(SRCS:.c=.o) $(SRCS:.cpp=.o) $(SRCS:.asm=.o)

# Targets
all: kernel

kernel: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o: %.asm
	$(NASM) $(NASMFLAGS) -o $@ $<

clean:
	rm -f $(OBJS) kernel