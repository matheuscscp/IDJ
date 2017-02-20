CXXFLAGS = -ansi -pedantic -Wall -Wextra -g

BINDIR = bin
INCDIR = include
MODDIR = mod
OBJDIR = obj
SRCDIR = src
DOCDIR = doc

LIB = -lSDL -lSDL_image
MOD = 
OBJ = $(OBJDIR)/SDLBase.o $(OBJDIR)/Sprite.o $(OBJDIR)/GameManager.o $(OBJDIR)/main.o $(OBJDIR)/Point.o $(OBJDIR)/Planet.o $(OBJDIR)/TileSet.o $(OBJDIR)/TileMap.o $(OBJDIR)/configfile.o $(OBJDIR)/mexception.o
EXE = trabalho_01

all: build

build: $(OBJ)
	g++ $(CXXFLAGS) $(MOD) $(OBJ) -o $(BINDIR)/$(EXE) $(LIB)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	g++ -I./include $(CXXFLAGS) -c $< -o $@

run:
	$(BINDIR)/$(EXE)

clean:
	rm -rf $(BINDIR)/$(EXE) $(OBJDIR)/*

dox:
	doxygen

doxclean:
	rm -rf $(DOCDIR)/html $(DOCDIR)/latex

gdb:
	gdb $(BINDIR)/$(EXE)
