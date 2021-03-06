%{
#include "macros.h"
#include "ipe.h"
#include "ipe.tab.h"
extern char *progname;
extern int lineno;
%}
/* definiciones regulares */
digito			[0-9]
letra			[a-zA-Z]
alfanum			{letra}|{digito}

identificador	{letra}({alfanum}*[_]?{alfanum}+)*
numero			{digito}+(\.{digito}+)?(E[+-]?{digito}+)?

cadena			'.*(\\'.*)*.*'

comentario_una_linea			@.*$
comentario_varias_lineas		\#([\n]|.)*\#


%%

[ \t] { ; } /* saltar los espacios y los tabuladores */
{comentario_una_linea}		{ ; }
{comentario_varias_lineas}	{ ; }



(?i:_mod)				{return MODULO;}
(?i:_div)				{return DIVISION_ENTERA;}
(?i:_o)					{return O_LOGICO;}
(?i:_y)					{return Y_LOGICO;}
(?i:_no)				{return NEGACION;}
(?i:leer)				{return LEER;}
(?i:leer_cadena)		{return LEER_CADENA;}
(?i:escribir)			{return ESCRIBIR;}
(?i:escribir_cadena)	{return ESCRIBIR_CADENA;}
(?i:si)					{return SI;}
(?i:entonces)			{return ENTONCES;}
(?i:si_no)				{return SI_NO;}
(?i:fin_si)				{return FIN_SI;}
(?i:mientras)			{return MIENTRAS;}
(?i:hacer)				{return HACER;}
(?i:fin_mientras)		{return FIN_MIENTRAS;}
(?i:repetir)			{return REPETIR;}
(?i:hasta)				{return HASTA; /* Funciona de una forma u otra segun que token lo preceda (es el mismo) */}
(?i:para)				{return PARA;}
(?i:desde)				{return DESDE;}
(?i:paso)				{return PASO;}
(?i:fin_para)			{return FIN_PARA;}
(?i:_borrar)			{return TOKEN_BORRAR;}
(?i:_lugar)				{return TOKEN_LUGAR;}



{numero}		{


				  double d;

					sscanf(yytext,"%lf",&d);
					/* Instala el numero en la tabla de simbolos */
					yylval.sym=install("",NUMBER,d); 

				  return NUMBER;
				}

{cadena}		{

				  char *cad = malloc(strlen(yytext) - 2);
				  int i;

					for(i = 1; i < (strlen(yytext) - 1); i++)
						cad[i - 1] = yytext[i];


					yylval.sym = install_string("", CADENA, cad);

				  return CADENA;

				}


{identificador}	{

				  Symbol *s;

					if ((s=lookup(yytext)) == 0)
						s = install (yytext, VAR, 0.0);		//No se si deberia hacer esto (antes INDEFINIDA)
					yylval.sym = s;

				  return s->tipo == INDEFINIDA ? VAR : s->tipo;

				}


">="	{return MAYOR_IGUAL;}
"<="	{return MENOR_IGUAL;}
"="	    {return IGUAL;}
"<>"	{return DISTINTO;}
">"		{return MAYOR_QUE;}
"<"		{return MENOR_QUE;}
"||"	{return CONCATENACION;}
"**"	{return POTENCIA;}
":="	{return ASIGNACION;}
"^#"       {
 	  BORRAR; 
	  LUGAR(10,10);
	  INTENSIDAD;
	  printf(">>>>>>> Fin del programa <<<<<<<\n");
	  APAGA;
	  LUGAR(24,1);

  	  return 0;
	}
;		{return FIN;}
\n      {lineno++; } /* no se devuelve nada, contin�a el analisis l�xico */
.       {return yytext[0];}
