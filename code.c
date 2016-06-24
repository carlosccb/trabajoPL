#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "ipe.h"
#include "ipe.tab.h"

#include "macros.h"

#define NSTACK 256              /* Dimension maxima de la pila */
static Datum stack[NSTACK];     /* La pila */
static Datum *stackp;           /* siguiente lugar libre en la pila */

#define NPROG 2000 
Inst prog[NPROG];  /* La maquina */
Inst *progp;       /* Siguiente lugar libre para la generacion de codigo */

Inst *pc; /* Contador de programa durante la ejecucion */

void initcode() /* inicializacion para la generacion de codigo */
{
 stackp = stack;
 progp = prog;
}

void push(Datum d) /* meter d en la pila */
{
 
/* Comprobar que hay espacio en la pila para el nuevo valor o variable */
 
 if (stackp >= &stack[NSTACK])
     execerror (" Desborde superior de la pila ", (char *) 0);
 
 *stackp++ =d; /* Apilar la variable o el numero y */
               /* desplazar el puntero actual de la pila */
}


Datum pop() /* sacar y devolver de la pila el elemento de la cima */
{
 
/* Comprobar que no se intenta leer fuera de la pila */ 
/* En teoria no ocurrira nunca */
 
 if (stackp <= stack)
     execerror (" Desborde inferior de la pila ", (char *) 0);
 
 --stackp;          /* Volver hacia atras una posicion en la pila */
 return(*stackp);   /* Devolver variable o numero */
}

void pop2() /* sacar y  NO devolver el elemento de la cima de la pila */
{
 
/* Comprobar que no se intenta leer fuera de la pila */ 
/* En teoria no ocurrira nunca */
 
 if (stackp <= stack)
     execerror (" Desborde inferior de la pila ", (char *) 0);
 
 --stackp;          /* Volver hacia atras una posicion en la pila */
}

Inst *code(Inst f) /* Instalar una instruccion u operando */
{
 Inst *oprogp = progp;   /* Puntero auxiliar */
 
/* Comprobar que hay espacio en el vector de instrucciones */ 

 if (progp >= &prog[NPROG])
     execerror (" Programa demasiado grande", (char *) 0);
 
 *progp=f;        /* Asignar la instruccion o el puntero a la estructura */
 progp++;         /* Desplazar una posicion hacia adelante */
 return (oprogp);
}


void execute(Inst *p)  /* Ejecucion con la maquina */
{
 
/* El contador de programa pc se inicializa con la primera instruccion a */ 
/* ejecutar */
 
 for (pc=p; *pc != STOP;   )
    (*(*pc++))();              /* Ejecucion de la instruccion y desplazar */
}                              /* el contador de programa pc */

/****************************************************************************/
/****************************************************************************/

void assign() /* asignar el valor superior al siguiente valor */
{
 Datum d1,d2;
 d1=pop();    /* Obtener variable */
 d2=pop();    /* Obtener numero   */
 
 if (d1.sym->tipo != VAR && d1.sym->tipo != CADENA && d1.sym->tipo != INDEFINIDA)
   execerror(" asignacion a un elemento que no es una variable ", d1.sym->nombre);

  if(d1.sym->cadena == 0){
	  d1.sym->u.val=d2.val;   /* Asignar valor   */
	  d1.sym->tipo=VAR;
  }

  else{

	  d1.sym->u.str=d2.str;   /* Asignar valor   */
	  d1.sym->tipo=CADENA;
  }

  push(d1);               /* Apilar variable */
}

void constpush()  /* meter una constante en la pila */
{
 Datum d;
 
 d.val= ((Symbol *)*pc++)->u.val;
 push(d);
}


void stringpush()  /* meter una cadena en la pila */
{
 Datum d;
 
 d.str= ((Symbol *)*pc++)->u.str;
 push(d);
}





void dividir() /* dividir los dos valores superiores de la pila */
{
 Datum d1,d2;
 
 d2=pop();      /* Obtener el primer numero  */
 d1=pop();      /* Obtener el segundo numero */
 
/* Comprobar si hay division por 0 */ 
 
 if (d2.val == 0.0)
     execerror (" Division por cero ", (char *) 0);
 
 d1.val = d1.val / d2.val;    /* Dividir             */
 push(d1);                    /* Apilar el resultado */
}

void escribir() /* sacar de la pila el valor superior y escribirlo */
{
 Datum d;
 
 d=pop();  /* Obtener numero */
 
 printf("%.8g\n",d.val);
}


void escribir_cadena() /* sacar de la pila el valor superior y escribirlo */
{
 Datum d;
 
 d=pop();  /* Obtener numero */
 
// printf("%s\n",d.str);

	int i;

	for(i = 0; i < strlen(d.str); i++){

		if(i == strlen(d.str) - 1)
			printf("%c", d.str[i]);

		else{

			if(d.str[i] == '\\'){

				if(d.str[i+1] == 'n'){

					printf("\n");
					i++;
				}

				if(d.str[i+1] == 't'){

					printf("\t");
					i++;
				}
			}

			else
				printf("%c", d.str[i]);

		}

	}

	printf("\n");

}



void eval() /* evaluar una variable en la pila */
{
 Datum d;
 
 d=pop();  /* Obtener variable de la pila */
 
/* Si la variable no esta definida */ 
 if (d.sym->tipo == INDEFINIDA) 
     execerror (" Variable no definida ", d.sym->nombre);
 
 if(d.sym->cadena == 0)
	d.val=d.sym->u.val;  /* Sustituir variable por valor */

 else
	d.str=d.sym->u.str;  /* Sustituir variable por valor */

 push(d);             /* Apilar valor */
}

void funcion0() /* evaluar una funcion predefinida sin parametros */
{
 Datum d;
 
 d.val= (*(double (*)())(*pc++))();
 push(d);
}

void funcion1() /* evaluar una funcion predefinida con un parametro */
{
 Datum d;
 
 d=pop();  /* Obtener parametro para la funcion */

 d.val= (*(double (*)())(*pc++))(d.val);
 push(d);
}

void funcion2() /* evaluar una funcion predefinida con dos parametros */
{
 Datum d1,d2;
 
 d2=pop();  /* Obtener parametro para la funcion */
 d1=pop();  /* Obtener parametro para la funcion */

 d1.val= (*(double (*)())(*pc++))(d1.val,d2.val);
 push(d1);
}

/* resto de la division entera del segundo valor de la pila */
/* por el valor de la cima */
void modulo() 
{
 Datum d1,d2;
 
 d2=pop();      /* Obtener el divisor */
 d1=pop();      /* Obtener el dividendo */
 
/* Comprobar si hay division por 0 */ 
 
 if (d2.val == 0.0)
     execerror (" Division por cero ", (char *) 0);
 
 d1.val = (int) d1.val % (int)  d2.val;  /* Resto */
 push(d1);                               /* Apilar el resultado */
}

void multiplicar() /* multiplicar los dos valores superiores de la pila */
{
 Datum d1,d2;
 
 d2=pop();                   /* Obtener el primer numero  */
 d1=pop();                   /* Obtener el segundo numero */
 d1.val = d1.val * d2.val;   /* Multiplicar               */
 push(d1);                   /* Apilar el resultado       */
}

void negativo() /* negacion del valor superior de la pila */
{
 Datum d1;
 
 d1=pop();              /* Obtener numero   */
 d1.val = - d1.val;     /* Aplicar menos    */
 push(d1);              /* Apilar resultado */
}

/* Esta funcion se puede omitir   */
void positivo() /* tomar el valor positivo del elemento superior de la pila */
{
 Datum d1;
 
 d1=pop();              /* Obtener numero   */
 /* d1.val = + d1.val;*/     /* Aplicar mas    */
 push(d1);              /* Apilar resultado */
}

void potencia()  /* exponenciacion de los valores superiores de la pila */
{
 Datum d1,d2;
 
 d2=pop();                      /* Obtener exponente   */
 d1=pop();                      /* Obtener base        */
 
 if ( (d1.val>=0) || ((int)d2.val == d2.val) )
  {
   d1.val = pow(d1.val,d2.val);   /* Elevar a potencia   */
   push(d1);                      /* Apilar el resultado */
  }
 else 
  {
   char digitos[20];
   sprintf(digitos,"%lf",d1.val);
   execerror(" radicando negativo ", digitos);
  }

}

void restar()   /* restar los dos valores superiores de la pila */
{
 Datum d1,d2;
 
 d2=pop();                   /* Obtener el primer numero  */
 d1=pop();                   /* Obtener el segundo numero */
 d1.val = d1.val - d2.val;   /* Restar                    */
 push(d1);                   /* Apilar el resultado       */
}

void sumar()   /* sumar los dos valores superiores de la pila */
{
 Datum d1,d2;
 
 d2=pop();                   /* Obtener el primer numero  */
 d1=pop();                   /* Obtener el segundo numero */
 d1.val = d1.val + d2.val;   /* Sumar                     */
 push(d1);                   /* Apilar el resultado       */
}

void varpush()  /* meter una variable en la pila */
{
 Datum d;

 d.sym=(Symbol *)(*pc++);
 push(d);
}
/****************************************************************************/
/****************************************************************************/

void leervariable() /* Leer una variable numerica por teclado */
{
 Symbol *variable;
 char c;

 variable = (Symbol *)(*pc); 

 /* Se comprueba si el identificador es una variable */ 
  if ((variable->tipo == INDEFINIDA) || (variable->tipo == VAR))
    { 
//    printf("Valor--> ");
    while((c=getchar())=='\n') ;
    ungetc(c,stdin);
    scanf("%lf",&variable->u.val);
    variable->tipo=VAR;
	variable->cadena=0;
    pc++;

   }
 else
     execerror("No es una variable numerica",variable->nombre);
}           



void leer_cadena() /* Leer una variable de tipo cadena por teclado */
{
 Symbol *variable;
 char c;

 variable = (Symbol *)(*pc); 

 variable->u.str = (char *) calloc(1024, sizeof(char));
 /* Se comprueba si el identificador es una variable */ 
  if ((variable->tipo == INDEFINIDA) || (variable->tipo == CADENA) || (variable->tipo == VAR))
    { 
//      printf("Valor--> ");
	  //Coge \ns
      while((c=getchar())=='\n');
	  //Coge la primera letra que se queda atrás
      ungetc(c,stdin);
	  fgets(variable->u.str, 1023, stdin);
      //scanf("%s",variable->u.str);
      variable->tipo=CADENA;
	  variable->cadena=1;
      pc++;
   }
 else
     execerror("No es una variable cadena",variable->u.str);
}           




void mayor_que()
{
 Datum d1,d2;
 
 d2=pop();   /* Obtener el primer numero  */
 d1=pop();   /* Obtener el segundo numero */
 
 if (d1.val > d2.val)
   d1.val= 1;
 else
   d1.val=0;
 
 push(d1);  /* Apilar resultado */
}


void menor_que()
{
 Datum d1,d2;
 
 d2=pop();    /* Obtener el primer numero  */
 d1=pop();    /* Obtener el segundo numero */
 
 if (d1.val < d2.val)
   d1.val= 1;
 else
   d1.val=0;
 
 push(d1);    /* Apilar el resultado */
}


void igual()
{
 Datum d1,d2;
 
 d2=pop();    /* Obtener el primer numero  */
 d1=pop();    /* Obtener el segundo numero */
 
 if (d1.val == d2.val)
   d1.val= 1;
 else
   d1.val=0;
 
 push(d1);    /* Apilar resultado */
}

void mayor_igual()
{
 Datum d1,d2;
 
 d2=pop();    /* Obtener el primer numero  */
 d1=pop();    /* Obtener el segundo numero */
 
 if (d1.val >= d2.val)
   d1.val= 1;
 else
   d1.val=0;
 
 push(d1);    /* Apilar resultado */
}


void menor_igual()
{
 Datum d1,d2;
 
 d2=pop();     /* Obtener el primer numero  */
 d1=pop();     /* Obtener el segundo numero */
 
 if (d1.val <= d2.val)
   d1.val= 1;
 else
   d1.val=0;
 
 push(d1);     /* Apilar resultado */
}

void distinto()
{
 Datum d1,d2;
 
 d2=pop();    /* Obtener el primer numero  */
 d1=pop();    /* Obtener el segundo numero */
 
 if (d1.val != d2.val)
   d1.val= 1;
 else
   d1.val=0;
 
 push(d1);    /* Apilar resultado */
}


void y_logico()
{
 Datum d1,d2;
 
 d2=pop();    /* Obtener el primer numero  */
 d1=pop();    /* Obtener el segundo numero */
 
 if (d1.val==1 && d2.val==1)
   d1.val= 1;
 else 
   d1.val=0;
 
 push(d1);    /* Apilar el resultado */
}


void o_logico()
{
 Datum d1,d2;
 
 d2=pop();    /* Obtener el primer numero  */
 d1=pop();    /* Obtener el segundo numero */
 
 if (d1.val==1 || d2.val==1)
   d1.val= 1;
 else
   d1.val=0;
 
 push(d1);    /* Apilar resultado */
}


void negacion()
{
 Datum d1;
 
 d1=pop();   /* Obtener numero */
 
 if (d1.val==0)
   d1.val= 1;
 else
   d1.val=0;
 
 push(d1);   /* Apilar resultado */
}


void whilecode()
{
 Datum d;
 Inst *savepc = pc;    /* Puntero auxiliar para guardar pc */

 execute(savepc+2);    /* Ejecutar codigo de la condicion */
 
 d=pop();    /* Obtener el resultado de la condicion de la pila */
 
 while(d.val)   /* Mientras se cumpla la condicion */
    {
     execute(*((Inst **)(savepc)));   /* Ejecutar codigo */
     execute(savepc+2);               /* Ejecutar condicion */
     d=pop();              /* Obtener el resultado de la condicion */
    }
 
/* Asignar a pc la posicion del vector de instrucciones que contiene */  
/* la siguiente instruccion a ejecutar */ 
 
 pc= *((Inst **)(savepc+1));  
}



void dowhilecode(){


 Datum d;
 Inst *savepc = pc;    /* Puntero auxiliar para guardar pc */

	do{   /* Mientras se cumpla la condicion */

     execute(savepc+2);               /* Ejecutar el cuerpo del bucle */

     execute(*((Inst **)(savepc)));   /* Ejecutar la condición */

     d=pop();              /* Obtener el resultado de la condicion */

    } while(! d.val);

 pc= *((Inst **)(savepc+1));
}


void forcode(){


 Symbol *variable;

 Datum d, dexpr1, dexpr2, dexpr3;
 Inst *savepc = pc;   /* Puntero auxiliar para guardar pc */

	variable = *(Symbol **)(savepc+5);

	execute(savepc+7);  				//Ejecutamos primera expresion
	dexpr1 = pop();

	execute(*((Inst **)(savepc)));		//Ejecutamos segunda expresion
	dexpr2 = pop();

	execute(*((Inst **)(savepc+1)));	//Ejecutamos tercera expresion
	dexpr3 = pop();

	/* 
		Habra que comprobar antes los signos de las expresiones 2 y 3 
		y quizas luego modificarles los valores, ya que sino el for no
		se puede hacer siempre con un menor que

																	*/
	if(dexpr3.val == 0){

		execerror (" Bucle for con aumentos de 0. Bucle infinito ", (char *) 0);
		return;
	}

	else if(dexpr3.val < 0){

		if(dexpr2.val > dexpr1.val){

			execerror (" Expresiones bucle for incorrectas. Bucle infinito ", (char *) 0);
			return;
		}

		else{


			/* FALLA PORQUE LA VARIABLE DEL BUCLE TIENE DE TIPO INDEFINIDA, POR LO QUE DA ERROR AL USARLA */


			for(variable->u.val = dexpr1.val; variable->u.val >= dexpr2.val; variable->u.val += dexpr3.val){

				execute(*((Inst **)(savepc+2)));	// Ejecutar cuerpo del bucle

				execute(*((Inst **)(savepc)));		// Ejecutar condicion de parada
				dexpr2 = pop();
			}
		}

	}

	else{

		for(variable->u.val = dexpr1.val; variable->u.val <= dexpr2.val; variable->u.val += dexpr3.val){

			execute(*((Inst **)(savepc+2)));	// Ejecutar cuerpo del bucle

			execute(*((Inst **)(savepc)));		// Ejecutar condicion de parada
			dexpr2 = pop();
		}
	}

 pc= *((Inst **)(savepc+3));

}


void ifcode()
{
 Datum d;
 Inst *savepc = pc;   /* Puntero auxiliar para guardar pc */

 execute(savepc+3);   /* Ejecutar condicion */
 d=pop();             /* Obtener resultado de la condicion */
 
 
/* Si se cumple la condici\A2n ejecutar el cuerpo del if */
 
 if(d.val)
   execute(*((Inst **)(savepc)));
 
/* Si no se cumple la condicion se comprueba si existe parte else   */
/* Esto se logra ya que la segunda posicion reservada contendria el */
/* puntero a la primera instruccion del cuerpo del else en caso de  */
/* existir, si no existe sera\A0 STOP, porque a la hora de generar    */
/* codigo se inicializa con STOP.                                   */

 else if  (*((Inst **)(savepc+1)))  /* parte else */
   execute(*((Inst **)(savepc+1)));
 

/* Asignar a pc la posicion del vector de instrucciones que contiene */  
/* la siguiente instruccion a ejecutar */ 
 
 pc= *((Inst **)(savepc+2));
}

/************************************/
/* Codigo hecho por mi para macros */
/************************************/

//void _borrar() {}

void func_lugar() {
  Datum d;
  Inst *savepc = pc;    /* Puntero auxiliar para guardar pc */
  int fil, col;

  d = pop();              /* Obtener el resultado de la condicion */
  fil = d.val;
  d = pop();              /* Obtener el resultado de la condicion */
  col = d.val;

  printf("fil: %d, col %d\n", fil, col);

  LUGAR(fil, col);
}

/************************************/
/* Codigo hecho por mi para macros */
/************************************/

