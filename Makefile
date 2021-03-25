IDIR=include
CC=g++
LIBS=`pkg-config --cflags --libs opencv` -framework OpenAL

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
	$(CC) -c -o $@ $< $(LIBS)

MezzoReader: $(OBJ)
	$(CC) -o $@ $^ $(LIBS) 

.PHONY: clean

clean:
	rm -r $(ODIR)
	rm ./MezzoReader
