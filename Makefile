CXXFLAGS =	-g3 -Wall -fmessage-length=0 #-Werror

OBJS = util.o game.o globals.o cordinate.o token.o player.o  board.o 
LIBS = -L/usr/X11R6/lib -L/sw/lib -L/usr/sww/lib -L/usr/sww/bin -L/usr/sww/pkg/Mesa/lib -lglut -lGLU -lGL -lX11 -lfreeimage -pthread



TARGET =	game


$(TARGET):	$(OBJS) 
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)


all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
