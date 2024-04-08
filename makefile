PROJECT = ssl_mod

CXX      = x86_64-w64-mingw32-g++-posix
CXXFLAGS = -Ofast -std=c++2a \
           -Iinclude \
           -Wall -Wextra -Werror -Wshadow -Wpedantic -Wconversion \
		   -fPIE -funsafe-math-optimizations -fomit-frame-pointer \
           -funroll-loops -funsafe-loop-optimizations -funswitch-loops -floop-parallelize-all \
           -finline-functions -falign-functions -falign-loops -falign-jumps -fno-function-sections \
           -fno-ident -fvisibility=hidden -fstrict-aliasing \
		   -DVC_EXTRALEAN -DWIN32_LEAN_AND_MEAN

LD      = x86_64-w64-mingw32-ld
LDFLAGS = -static -shared -s --entry=DllMain $(addprefix -l,kernel32 user32)

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(patsubst src/%.cpp,build/%.obj,$(SOURCES))

HEADERS = $(wildcard include/*.hpp)

all: target
target: $(PROJECT)

$(PROJECT): bin build lib $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o lib/$(PROJECT).dll

$(OBJECTS): build/%.obj : src/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: bin
bin:
	mkdir -p bin

.PHONY: build
build:
	mkdir -p build

.PHONY: lib
lib:
	mkdir -p lib
