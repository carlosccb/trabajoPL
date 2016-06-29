#ifndef EJEMPLO9_H
#define EJEMPLO9_H

 /* ejemplo9.h */


int yylex();

void yyerror(char *s);

void warning(char *s, char *t);

void execerror(char *s,char *t);

void init();

typedef struct Symbol 
            { /* elementos de la tabla de simbolos */
	        char *nombre;
	        short tipo; /* NUMBER,VAR,FUNCION,INDEFINIDA,CONSTANTE */
			int cadena;
	        union {
		       double val;      /* VAR, NUMBER, INDEFINIDA, CONSTANTE */
			   char * str;
		       double (*ptr)(); /* FUNCION  */
		      } u;
                 struct Symbol *siguiente;
	     } Symbol;

Symbol *install(), *install_string(), *lookup();

typedef union Datum { /* tipo de la pila del interprete */ 
                     double val;
					 char * str;
                     Symbol *sym;
                    } Datum;

void push(Datum d);
extern Datum pop();
extern void pop2();

typedef void (*Inst)(); /* instruccion maquina */
#define STOP (Inst) 0

extern Inst prog[];

/* NOVEDAD */
extern Inst* progp;

void initcode();
 
Inst *code(Inst f);

void execute(Inst *p);

/**************************/
extern void assign();
extern void constpush();
extern void stringpush();
void dividir();
void escribir();
void escribir_cadena();
void eval();

void funcion();
void funcion0();
void funcion1();
void funcion2();


void modulo();
void multiplicar();
void negativo();
void positivo();
void potencia(); 
void restar();
void sumar();
void varpush();

void ifcode();
void whilecode();
void dowhilecode();
void forcode();

void mayor_que();
void menor_que();
void mayor_igual();
void menor_igual();
void igual(); 
void distinto();
void y_logico();
void o_logico();
void negacion();

void leervariable();
void leer_cadena();

void func_lugar();
void func_borrar();
#endif
