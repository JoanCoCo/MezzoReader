IDIR=include
CC=g++
UNAME_S := $(shell uname -s)
HERE := $(shell pwd)
VERSION=v.0.1

ifeq ($(UNAME_S),Darwin)
	CFLAGS= -std=c++11 -I$(HERE)/dependencies/opencv/include/opencv4
	LIBS=-Wl,-rpath,./dependencies/opencv/lib -L./dependencies/opencv/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -framework OpenAL
	CORES := $(shell sysctl -n hw.ncpu)
else
	CFLAGS= -std=c++11 -I./dependencies/opencv/include/opencv4 -I/usr/include/AL
	LIBS=-Wl,-rpath,./dependencies/opencv/lib -L./dependencies/opencv/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lalut -lopenal
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
	curl -L https://github.com/opencv/opencv/archive/4.2.0.tar.gz --output tmp/opencv.tar.gz
	tar -zxvf tmp/opencv.tar.gz -C ./tmp/
	mkdir tmp/opencv-4.2.0/build
	cd tmp/opencv-4.2.0/build && cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=$(HERE)/dependencies/opencv/ -D INSTALL_C_EXAMPLES=OFF -D BUILD_EXAMPLES=OFF $(HERE)/tmp/opencv-4.2.0/ && make -j$(CORES) && make install
	rm -r tmp
else
dependencies:
	mkdir tmp
	mkdir dependencies
	mkdir dependencies/opencv
	sudo apt-get install libopenal-dev
	curl -L https://github.com/opencv/opencv/archive/4.2.0.zip --output tmp/opencv.tar.gz
	unzip tmp/opencv.tar.gz -d tmp/
	mkdir tmp/opencv-4.2.0/build
	cd tmp/opencv-4.2.0/build && cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=$(HERE)/dependencies/opencv/ -D INSTALL_C_EXAMPLES=OFF -D BUILD_EXAMPLES=OFF $(HERE)/tmp/opencv-4.2.0/ && make -j$(CORES) && make install
	rm -r tmp
endif
	
.PHONY: release

release:
	make clean
	make
	mkdir $(VERSION)
	cp MezzoReader $(VERSION)/MezzoReader
	cp -r sounds $(VERSION)/sounds
	rm -r $(VERSION)/sounds/flute
	cp -r templates $(VERSION)/templates
	mkdir $(VERSION)/images
	cp images/Star\ wars\ Flauta-1.jpeg $(VERSION)/images/star_wars.jpeg
	cp images/notation.png $(VERSION)/images/notation.png
	cp images/notes.png $(VERSION)/images/notes.png
	cp images/scanned/when_im_gone.jpg $(VERSION)/images/when_im_gone.jpg
	cp images/a_dormir.jpeg $(VERSION)/images/a_dormir.jpeg
	mkdir $(VERSION)/dependencies
	mkdir $(VERSION)/dependencies/opencv
	mkdir $(VERSION)/dependencies/opencv/lib
	cp dependencies/opencv/lib/libopencv_core.4.2.0.dylib $(VERSION)/dependencies/opencv/lib/libopencv_core.4.2.0.dylib
	cp dependencies/opencv/lib/libopencv_imgproc.4.2.0.dylib $(VERSION)/dependencies/opencv/lib/libopencv_imgproc.4.2.0.dylib
	cp dependencies/opencv/lib/libopencv_highgui.4.2.0.dylib $(VERSION)/dependencies/opencv/lib/libopencv_highgui.4.2.0.dylib
	cp dependencies/opencv/lib/libopencv_imgcodecs.4.2.0.dylib $(VERSION)/dependencies/opencv/lib/libopencv_imgcodecs.4.2.0.dylib
	cp dependencies/opencv/lib/libopencv_videoio.4.2.0.dylib $(VERSION)/dependencies/opencv/lib/libopencv_videoio.4.2.0.dylib
	cp -a dependencies/opencv/lib/libopencv_core.4.2.dylib $(VERSION)/dependencies/opencv/lib/libopencv_core.4.2.dylib
	cp -a dependencies/opencv/lib/libopencv_imgproc.4.2.dylib $(VERSION)/dependencies/opencv/lib/libopencv_imgproc.4.2.dylib
	cp -a dependencies/opencv/lib/libopencv_highgui.4.2.dylib $(VERSION)/dependencies/opencv/lib/libopencv_highgui.4.2.dylib
	cp -a dependencies/opencv/lib/libopencv_imgcodecs.4.2.dylib $(VERSION)/dependencies/opencv/lib/libopencv_imgcodecs.4.2.dylib
	cp -a dependencies/opencv/lib/libopencv_videoio.4.2.dylib $(VERSION)/dependencies/opencv/lib/libopencv_videoio.4.2.dylib
	
