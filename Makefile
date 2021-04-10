IDIR=include
CC=g++
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	CFLAGS= -std=c++11 `pkg-config opencv4 --cflags`
	LIBS=`pkg-config opencv4 --libs` -framework OpenAL
else
	CFLAGS=`pkg-config --cflags freealut opencv --cflags`
	LIBS=`pkg-config freealut opencv --libs`
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


