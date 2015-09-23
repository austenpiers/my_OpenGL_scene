CC      = gcc -g
X11	= /usr/X11R6/
IFLAGS  = -I$(X11)/include 
LFLAGS  = -L$(X11)/lib 
LIBS    = -lglut -lGLU -lGL -lSM -lICE -lXext -lX11 -lm
OBJECTS = 
HEADERS = 

.c.o: 
	$(CC) $(IFLAGS) -c $<

sample: $(OBJECTS) $(HEADERS)
	$(CC) -o run firstCube.c $(IFLAGS) $(LFLAGS) $(LIBS)

