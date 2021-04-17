IDIR=include
CC=g++
UNAME_S := $(shell uname -s)
HERE := $(shell pwd)

ifeq ($(UNAME_S),Darwin)
	CFLAGS= -std=c++11 -I$(HERE)/dependencies/opencv/include/opencv4
	LIBS=-Wl,-rpath,$(HERE)/dependencies/opencv/lib -L$(HERE)/dependencies/opencv/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -framework OpenAL
	CORES := $(shell sysctl -n hw.ncpu)
else
	CFLAGS=`pkg-config --cflags freealut opencv --cflags`
	LIBS=`pkg-config freealut opencv --libs`
	CORES := $(shell grep -c ^processor /proc/cpuinfo)
endif

ODIR=obj
SDIR=source

_DEPS = Note.h Staff.h MezzoUtilities.h Symbol.h Alphabet.h MezzoPlayer.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = Note.o Staff.o Symbol.o MezzoUtilities.o MezzoReader.o MezzoPlayer.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: MezzoReader

$(ODIR)/.:
	mkdir -p $@

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS) $(ODIR)/.
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

MezzoReader: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) 

.PHONY: clean

clean:
	rm -r $(ODIR)
	rm ./MezzoReader
	
.PHONY: dependencies
	
ifeq ($(UNAME_S),Darwin)
dependencies:
	mkdir tmp
	mkdir dependencies
	mkdir dependencies/opencv
	curl -L https://github.com/opencv/opencv/archive/4.2.0.zip --output tmp/opencv.tar.gz
	tar -zxvf tmp/opencv.tar.gz -C ./tmp/
	mkdir tmp/opencv-4.2.0/build
	cd tmp/opencv-4.2.0/build && cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=$(HERE)/dependencies/opencv/ -D INSTALL_C_EXAMPLES=OFF -D BUILD_EXAMPLES=OFF $(HERE)/tmp/opencv-4.2.0/ && make -j$(CORES) && make install
	rm -r tmp
else
		
endif
	


