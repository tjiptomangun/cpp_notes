INCDIR=-I .
GCOMP=g++ -Wall -ggdb3

all: main

main: main.o Vector.o Tiff.o Objects.o Trace.o Parser.o \
	Lights.o Ellipsoid.o EllParser.o ParserTri.o SweepParser.o \
	Mountain.o
	g++ -o main main.o Vector.o Tiff.o Objects.o Trace.o Parser.o \
	Lights.o Ellipsoid.o EllParser.o ParserTri.o SweepParser.o \
	Mountain.o

main.o: main.cpp
	$(GCOMP) $(INCDIR) -c main.cpp -o main.o

Vector.o: Vector.cpp
	$(GCOMP) $(INCDIR) -c Vector.cpp -o Vector.o

Tiff.o: Tiff.cpp
	$(GCOMP) $(INCDIR) -c Tiff.cpp -o Tiff.o

Objects.o: Objects.cpp
	$(GCOMP) $(INCDIR) -c Objects.cpp -o Objects.o

Trace.o: Trace.cpp
	$(GCOMP) $(INCDIR) -c Trace.cpp -o Trace.o

Parser.o: Parser.cpp
	$(GCOMP) $(INCDIR) -c Parser.cpp -o Parser.o

Lights.o: Lights.cpp
	$(GCOMP) $(INCDIR) -c Lights.cpp -o Lights.o

Ellipsoid.o: Ellipsoid.cpp
	$(GCOMP) $(INCDIR) -c Ellipsoid.cpp -o Ellipsoid.o

EllParser.o: EllParser.cpp
	$(GCOMP) $(INCDIR) -c EllParser.cpp -o EllParser.o

ParserTri.o: ParserTri.cpp
	$(GCOMP) $(INCDIR) -c ParserTri.cpp -o ParserTri.o

SweepParser.o: SweepParser.cpp
	$(GCOMP) $(INCDIR) -c SweepParser.cpp -o SweepParser.o

Mountain.o: Mountain.cpp
	$(GCOMP) $(INCDIR) -c Mountain.cpp -o Mountain.o


.PHONY: main main.o Vector.o
