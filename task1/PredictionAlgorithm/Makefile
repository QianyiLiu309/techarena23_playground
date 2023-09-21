#-------------------------------------------------------------------

# To enable support for dynamic instruction counting,
# the following tools must be used. Please do not modify. 
CC=/usr/bin/clang-16
CXX=/usr/bin/clang++-16
LD=/usr/bin/ld.lld-16

#-------------------------------------------------------------------

# Default compilation settings. Can be modified if required. 
CXXFLAGS=`llvm-config-16 --cxxflags --includedir` -fPIC 
LDFLAGS=`llvm-config-16 --ldflags` -lstdc++ -lboost_program_options -fPIC -shared

#-------------------------------------------------------------------

# To enable support dynamic instruction counting,
# the following Clang command-line options must be added.
# Please do not modify. 
CXXFLAGS += -Wno-unused-command-line-argument -fpass-plugin=../../common/DynamicInstructionCounting/libDynamicInstructionCounting.so

#-------------------------------------------------------------------
# Main makefile logic to prepare libTask1PredictionAlgorithm.so. 
# This part can be modified if needed. 

SRC_FILES := $(wildcard ./*.cpp)
OBJ_FILES := $(patsubst ./%.cpp,./%.o,$(SRC_FILES))

all: $(OBJ_FILES) 
	$(CC) $(LDFLAGS) $^ -o libTask1PredictionAlgorithm.so

./%.o: ./%.cpp
	$(CC) -c $(CXXFLAGS) -o $@ $<

clean:
	rm -rf *.o libTask1PredictionAlgorithm.so 
