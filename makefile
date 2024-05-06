PROJECT  = ssl_mod

CXX      = x86_64-w64-mingw32-g++-posix
CXXFLAGS = -Ofast -std=c++2a \
          -Iinclude \
          -Wall -Wextra -Wshadow -Wpedantic \
          -fPIE -funsafe-math-optimizations -fomit-frame-pointer \
          -funroll-loops -funsafe-loop-optimizations -funswitch-loops -floop-parallelize-all \
          -finline-functions -falign-functions -falign-loops -falign-jumps -fno-function-sections \
          -fno-ident -fvisibility=hidden -fstrict-aliasing \
          -DVC_EXTRALEAN -DWIN32_LEAN_AND_MEAN

LD       = x86_64-w64-mingw32-g++-posix
LDFLAGS  = -static -shared -s $(addprefix -l,ws2_32 gdi32 dwmapi) #--stack=65536 --entry=DllMain $(addprefix -l,kernel32 user32)

SOURCES  = $(wildcard src/*.cpp)
OBJECTS  = $(patsubst src/%.cpp,build/%.obj,$(SOURCES))

HEADERS  = $(wildcard include/*.hpp) $(wildcard include/imgui/*.h)

INCLUDES = $(addprefix -I,include include/imgui)

all: target
target: $(PROJECT)
$(PROJECT): lib/$(PROJECT).dll

lib/$(PROJECT).dll: bin build lib $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $@

$(OBJECTS): build/%.obj : src/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: clean
clean:
	rm -fr ./lib/*.dll
	rm -fr ./build/*.obj

bin:
	mkdir -p bin

build:
	mkdir -p build

lib:
	mkdir -p lib
