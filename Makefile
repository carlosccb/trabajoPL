FUENTE = ipe
LEXICO = ipe_lexico


CC = gcc
YFLAGS = -d -v 
LFLAGS = -lm -lfl 
OBJS= $(FUENTE).tab.o lex.yy.o  init.o math.o symbol.o  code.o

$(FUENTE)3.exe: $(OBJS) 
	$(CC) $(OBJS) $(LFLAGS) -o $(FUENTE).exe

code9.o:  code9.c $(FUENTE).h
	$(CC) -c code.c

init9.o:  init9.c $(FUENTE).h $(FUENTE).tab.h
	$(CC) -c init.c

symbol9.o:  symbol9.c $(FUENTE).h $(FUENTE).tab.h
	$(CC) -c symbol.c

math9.o:  math9.c $(FUENTE).h
	$(CC) -c math.c

lex.yy.o:  lex.yy.c $(FUENTE).tab.h $(FUENTE).h
	$(CC) -c lex.yy.c

lex.yy.c: $(LEXICO).l $(FUENTE).tab.h $(FUENTE).h
	flex $(LEXICO).l

$(FUENTE).tab.o:  $(FUENTE).tab.c $(FUENTE).tab.h $(FUENTE).h
	$(CC) -c $(FUENTE).tab.c

$(FUENTE).tab.c $(FUENTE).tab.h:  $(FUENTE).y $(FUENTE).h 
	bison $(YFLAGS) $(FUENTE).y

#Opcion para borrar los ficheros objetos y auxiliares
clean: 
	rm -f  $(OBJS) $(FUENTE).tab.[ch] lex.yy.c $(FUENTE).exe $(FUENTE).output
