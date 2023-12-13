CC = g++
CPPFLAGS = -Wall -pthread

nomePrograma = workTimer
objetos = main.o Timer.o

all: $(nomePrograma)

$(nomePrograma): $(objetos)
	$(CC)  $(CPPFLAGS)  -o $(nomePrograma) $(objetos)

%.o: %.cpp %.hpp
	$(CC)  $(CPPFLAGS)  -c $<

clean:
	rm -rf *.o $(nomePrograma)
