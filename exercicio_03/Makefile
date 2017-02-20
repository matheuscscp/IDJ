CXXFLAGS = -ansi -pedantic -Wall -Wextra -g

BINDIR = bin
INCDIR = include
MODDIR = mod
OBJDIR = obj
SRCDIR = src
DOCDIR = doc

LIB = -lSDL -lSDL_image
MOD = 
OBJ = $(OBJDIR)/main.o $(OBJDIR)/SDLBase.o $(OBJDIR)/Sprite.o $(OBJDIR)/TileSet.o $(OBJDIR)/TileMap.o $(OBJDIR)/GameObject.o $(OBJDIR)/Camera.o $(OBJDIR)/Geometry.o $(OBJDIR)/InputManager.o $(OBJDIR)/Button.o $(OBJDIR)/GameManager.o $(OBJDIR)/Planet.o $(OBJDIR)/FollowerObject.o $(OBJDIR)/configfile.o $(OBJDIR)/linearalgebra.o $(OBJDIR)/simplestructures.o
EXE = exercicio_03

all: $(OBJ)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	g++ -I./include $(CXXFLAGS) -c $< -o $@

link:
	g++ $(CXXFLAGS) $(MOD) $(OBJ) -o $(BINDIR)/$(EXE) $(LIB)

build: $(OBJ) link

run: build
	$(BINDIR)/$(EXE) -fps

clean:
	rm -rf $(BINDIR)/$(EXE) $(OBJDIR)/*

dox:
	doxygen

doxclean:
	rm -rf $(DOCDIR)/html $(DOCDIR)/latex

gdb:
	gdb $(BINDIR)/$(EXE)
