TARGET = esq

override CFLAGS +=-Wall -Wextra -Wfatal-errors -std=c++17 -MD -MP -O3 -fconcepts
CPP = g++
LIBS = -lpthread -lstdc++ -lboost_regex
SRC = $(wildcard *.cpp bigint/*.cpp)
HEADERS = $(wildcard *.h *.inl)
OBJS = $(SRC:.cpp=.o)

all: $(TARGET) 

release: CFLAGS += -DNDEBUG
release: $(TARGET) 

$(TARGET): $(OBJS)
	$(CPP) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) $(LIBS)

-include $(SRC:.cpp=.d)

%.o: %.cpp pre.h.gch
	$(CPP) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.d *.stackdump esq