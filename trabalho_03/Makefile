BINDIR = bin
INCDIR = include
MODDIR = mod
OBJDIR = obj
SRCDIR = src
DOCDIR = doc

SDLDIR = /usr/include/SDL

CXXFLAGS = -ansi -pedantic -Wall -Wextra -g -I $(INCDIR)/ -I $(SDLDIR)/

LIB = -lSDL -lSDL_image -lSDL_gfx
MOD = 
EXE = trabalho_03

OBJ0 = $(OBJDIR)/main.o $(OBJDIR)/SDLBase.o $(OBJDIR)/Sprite.o $(OBJDIR)/Animation.o
OBJ1 = $(OBJ0) $(OBJDIR)/TileSet.o $(OBJDIR)/TileMap.o $(OBJDIR)/GameObject.o
OBJ2 = $(OBJ1) $(OBJDIR)/Camera.o $(OBJDIR)/Geometry.o $(OBJDIR)/InputManager.o
OBJ3 = $(OBJ2) $(OBJDIR)/Button.o $(OBJDIR)/GameManager.o $(OBJDIR)/Planet.o
OBJ4 = $(OBJ3) $(OBJDIR)/FollowerObject.o $(OBJDIR)/AccObject.o $(OBJDIR)/configfile.o $(OBJDIR)/linearalgebra.o $(OBJDIR)/simplestructures.o

OBJ  = $(OBJ4)

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
