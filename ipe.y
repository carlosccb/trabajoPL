/*

 Ejemplo 9
 
  Se ha ampliado el ejemplo 8 para permitir 
  - operadores relacionales y l�gicos
  - sentencia condicional: if
  - sentencia iterativa: while
  - ejecutar las sentencias contenidas en un fichero indicado en la l�nea de comandos
    Por ejemplo:
       > ./ejemplo9.exe factorial.txt
*/

%{
#include <stdio.h>
#include <math.h>

#include "ipe.h"

#include "macros.h"

#define code2(c1,c2)         code(c1); code(c2)
#define code3(c1,c2,c3)      code(c1); code(c2); code(c3)
%}

%union{             
       Symbol *sym;    /* puntero a la tabla de simbolos */
       Inst *inst;     /* instruccion de maquina */
}

%token <sym> NUMBER VAR CONSTANTE CADENA FUNCION0_PREDEFINIDA FUNCION1_PREDEFINIDA FUNCION2_PREDEFINIDA INDEFINIDA PRINT WHILE IF ELSE READ /*   */ O Y NO LEER LEER_CADENA ESCRIBIR ESCRIBIR_CADENA SI ENTONCES SI_NO FIN_SI MIENTRAS HACER FIN_MIENTRAS REPETIR HASTA PARA DESDE PASO FIN_PARA TOKEN_BORRAR TOKEN_LUGAR FIN
%type <inst> stmt asgn expr stmtlist cond while dowhile for if end var
%right ASIGNACION
%left O_LOGICO
%left Y_LOGICO
%left MAYOR_QUE MENOR_QUE MENOR_IGUAL MAYOR_IGUAL DISTINTO IGUAL CONCATENACION
%left '+' '-'
%left '*' '/' MODULO DIVISION_ENTERA
%left UNARIO NEGACION
%right POTENCIA   
%%

list :    /* nada: epsilon produccion */ 
        | list stmt  FIN	{code(STOP); return 1;}
        | list error FIN	{yyerrok;} 
        ;

stmt :    /* nada: epsilon produccion */  {$$=progp;}
        | asgn						{code(pop2);}
		| ESCRIBIR '(' expr ')'		{code(escribir); $$ = $3;}
		| ESCRIBIR_CADENA '(' expr ')'	{code(escribir_cadena); $$ = $3;}
        | LEER '(' VAR ')'			{code2(leervariable,(Inst)$3);}
		| LEER_CADENA '(' VAR ')'	{code2(leer_cadena,(Inst)$3);}
		| TOKEN_BORRAR              {BORRAR;}
		| TOKEN_LUGAR '(' expr ',' expr  ')'  {}
											/*{code3(varpush,(Inst)$3, eval); code3(varpush,(Inst)$5,eval); funcion2();} /*func_lugar();  }*/
											/*  printf("_ %s  _ $3 -> %d . %d\n", $3, (int)$3, (int)$5);  }
											/*{$$=$3;code2(funcion2,(Inst)$1->u.ptr);}*/
        | while cond HACER stmtlist FIN_MIENTRAS end
                  {
                   ($1)[1]=(Inst)$4; /* cuerpo del bucle */
                   ($1)[2]=(Inst)$6; /* siguiente instruccion al bucle */
                  }

		| dowhile stmtlist end HASTA cond end
				  {
                   ($1)[1]=(Inst)$5; /* condicion del bucle */
                   ($1)[2]=(Inst)$6; /* siguiente instruccion al bucle */
				  }

		| for var DESDE expr end HASTA expr end PASO expr end HACER stmtlist FIN_PARA end
				  {
					/* No se cuantos STOPS hay que poner (se ponen con end salvo para las condiciones) */
					($1)[1]=(Inst)$7;
					($1)[2]=(Inst)$10;
					($1)[3]=(Inst)$13;
					($1)[4]=(Inst)$15;

				  }
        | if cond ENTONCES stmtlist FIN_SI end /* proposicion if sin parte else */
                  {
                   ($1)[1]=(Inst)$4; /* cuerpo del if */
                   ($1)[3]=(Inst)$6; /* siguiente instruccion al if */
                  }
        | if cond ENTONCES stmtlist end SI_NO stmtlist FIN_SI end /* proposicion if con parte else */
                  {
                   ($1)[1]=(Inst)$4; /* cuerpo del if */
                   ($1)[2]=(Inst)$7; /* cuerpo del else */
                   ($1)[3]=(Inst)$9; /* siguiente instruccion al if-else */
                  }
        ;


asgn :    VAR ASIGNACION expr { $$=$3; code3(varpush,(Inst)$1,assign);}
        | CONSTANTE ASIGNACION expr 
          {execerror(" NO se pueden asignar datos a constantes ",$1->nombre);}
	;

cond :    '(' expr ')' {code(STOP); $$ =$2;}
        ;

while:    MIENTRAS      {$$= code3(whilecode,STOP,STOP);}
        ;

dowhile:  REPETIR		{$$= code3(dowhilecode,STOP,STOP);}
		;

for:	  PARA			{$$= code2(forcode,STOP); code3(STOP,STOP,STOP);}
		;

if:       SI         	{$$= code(ifcode); code3(STOP,STOP,STOP);}
        ;

end :    /* nada: produccion epsilon */  {code(STOP); $$ = progp;}
        ;

var:	  VAR 				{$$=code3(varpush,(Inst)$1,eval);}	// Dejar code((Inst)$1 si no hay forma)
		;

stmtlist:  /* nada: prodcuccion epsilon */ {$$=progp;}
        | stmtlist stmt FIN
        ;

expr :    NUMBER     		{$$=code2(constpush,(Inst)$1);}
        | VAR        		{$$=code3(varpush,(Inst)$1,eval);} 
        | CONSTANTE      	{$$=code3(varpush,(Inst)$1,eval);}
		| CADENA			{$$=code2(stringpush,(Inst)$1);}
        | asgn
        | FUNCION0_PREDEFINIDA '(' ')'      {code2(funcion0,(Inst)$1->u.ptr);}
        | FUNCION1_PREDEFINIDA '(' expr ')' {$$=$3;code2(funcion1,(Inst)$1->u.ptr);}
        | FUNCION2_PREDEFINIDA '(' expr ',' expr ')'
                                            {$$=$3;code2(funcion2,(Inst)$1->u.ptr);}
        | '(' expr ')'			{$$ = $2;}
        | expr '+' expr			{code(sumar);}
        | expr '-' expr			{code(restar);}
        | expr '*' expr			{code(multiplicar);}
        | expr '/' expr			{code(dividir);}
        | expr MODULO expr		{code(modulo);}
        | expr POTENCIA expr 	{code(potencia);}
        |'-' expr %prec UNARIO 	{$$=$2; code(negativo);}
        |'+' expr %prec UNARIO 	{$$=$2; code(positivo);}
        | expr MAYOR_QUE expr 	{code(mayor_que);}
        | expr MAYOR_IGUAL expr {code(mayor_igual);}
        | expr MENOR_QUE expr 	{code(menor_que);}
        | expr MENOR_IGUAL expr {code(menor_igual);}
        | expr IGUAL expr		{code(igual);}
        | expr DISTINTO expr 	{code(distinto);}
        | expr Y_LOGICO expr 	{code(y_logico);}
        | expr O_LOGICO expr 	{code(o_logico);}
        | NEGACION expr			{$$=$2; code(negacion);}
      	;

%%

#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <setjmp.h>

jmp_buf begin;
char *progname;
int lineno = 1;
/* Dispositivo de entrada est�ndar de yylex() */
extern FILE * yyin;

void fpecatch();

int main(int argc, char *argv[])
{



 /* Si se invoca el int�rprete con un fichero de entrada */
 /* entonces se establece como dispositivo de entrada para yylex() */
 if (argc == 2) yyin = fopen(argv[1],"r");


 progname=argv[0];

 /* inicializacion de la tabla de simbolos */
 init();

/* Establece un estado viable para continuar despues de un error */
 setjmp(begin);

 /* Establece cual va a ser la funcion para tratar errores de punto flotante */
 signal(SIGFPE,fpecatch); /* Excepcion de punto flotante*/

/* initcode inicializa el vector de intrucciones y la pila del interprete */
 for (initcode(); yyparse(); initcode()) execute(prog);

 return 0;

}

void yyerror(char *s)
{
 warning(s,(char *) 0);
}

void warning(char *s, char *t)
{
 fprintf(stderr," ** %s : %s", progname,s);
 if (t) fprintf(stderr," ---> %s ",t);
 fprintf(stderr,"  (linea %d)\n",lineno);
}

void execerror(s,t) /* recuperacion de errores durante la ejecucion */
char *s,*t;
{
 warning(s,t);
  longjmp(begin,0);
}

void fpecatch()     /*  atrapa errores de punto flotante */
{
 execerror("error de punto flotante ",(char *)0);
}

