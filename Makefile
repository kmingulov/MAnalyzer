ifeq ($(debug), true)
	DEBUG_MODE = -g -DMANALYZER_DEBUG=1
else
	DEBUG_MODE =
endif

LFLAGS = -Lanalyzer/ $(DEBUG_MODE)
CFLAGS = -c -O3 $(DEBUG_MODE)
LIBS = -lmanalyzer

ma: main.o
	g++ $(LFLAGS) -o ma main.o $(LIBS)

cache: with_cache.o
	g++ $(LFLAGS) -o cache with_cache.o $(LIBS)

main.o: main.cpp
	g++ $(CFLAGS) main.cpp

with_cache.o: with_cache.cpp
	g++ $(CFLAGS) with_cache.cpp

clean:
	rm -f *.o ma
