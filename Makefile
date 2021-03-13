IDIR=include
CC=g++
CFLAGS=`pkg-config --cflags --libs opencv`

ODIR=obj
SDIR=source

_DEPS = Note.h Staff.h MezzoUtilities.h Symbol.h Alphabet.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = Note.o Staff.o Symbol.o MezzoUtilities.o MezzoReader.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: MezzoReader

$(ODIR)/.:
	mkdir -p $@

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS) $(ODIR)/.
	$(CC) -c -o $@ $< $(CFLAGS)

MezzoReader: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) 

.PHONY: clean

clean:
	rm -r $(ODIR)
	rm ./MezzoReader
