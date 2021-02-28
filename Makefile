IDIR=include
CC=g++
CFLAGS=`pkg-config --cflags --libs opencv`

ODIR=obj
SDIR=source

_DEPS = Note.h Staff.h Utilities.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = Note.o Staff.o Utilities.o MezzoReader.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

MezzoReader: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) 

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
