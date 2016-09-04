


_borrar;
_lugar(10,10);

escribir_cadena('Introduce tu nombre --> ');
leer_cadena(nombre);

_borrar;
_lugar(10,10);

escribir_cadena(' Bienvenido/a << ');
escribir_cadena(nombre);
escribir_cadena(' >> al intérprete de pseudocódigo en español: \'ipe.exe\'.\n');


repetir


	_borrar;
	_lugar(10,10);

	escribir_cadena(' Factorial de un número --> 1 ');

	_lugar(11,10);
	escribir_cadena(' Máximo común divisor ----> 2 ');

	_lugar(12,10);
	escribir_cadena(' Finalizar ---------------> 0 ');

	_lugar(15,10);
	escribir_cadena(' Elige una opcion: ');
	leer(opcion);
	_borrar;


	si (opcion = 0)
	entonces

		_lugar(10,10);
        escribir_cadena(nombre);
        escribir_cadena(': gracias por usar el intérprete ipe.exe ');


	si_no

		si (opcion = 1)
		entonces

		    _lugar(10,10);
			escribir_cadena(' Factorial de un numero \n');

		    _lugar(11,10);
			escribir_cadena(' Introduce un numero entero: ');
			leer(N);

			factorial := 1;

	   		para i desde 2 hasta N paso 1 hacer
		        factorial := factorial * i;
			fin_para;

		    _lugar(15,10);
			escribir_cadena(' El factorial de ');
			escribir(N);
			escribir_cadena(' es ');
			escribir(factorial);



		si_no

			si (opcion = 2)
			entonces

				_lugar(10,10);
				escribir_cadena(' Máximo común divisor de dos números ');

				_lugar(11,10);
				escribir_cadena(' Algoritmo de Euclides ');

				_lugar(12,10);
				escribir_cadena(' Escribe el primer número ');
				leer(a);

				_lugar(13,10);
				escribir_cadena(' Escribe el segundo número ');
				leer(b);

				si (a < b)
				entonces 
					auxiliar := a;
					a := b;
					b := auxiliar;
				fin_si;


				A1 := a;
				B1 := b;

				resto := a _mod b;

				mientras (resto <> 0)
				hacer

					a :=  b;
					b := resto;
					resto := a _mod b;

				fin_mientras;

				_lugar(15,10);
				escribir_cadena(' Máximo común divisor de ');
				escribir(A1);
				escribir_cadena(' y ');
				escribir(B1);
				escribir_cadena(' es ---> ');
				escribir(b);

	 		si_no

				_lugar(15,10);
				escribir_cadena(' Opcion incorrecta ');

			fin_si;

		fin_si;

	fin_si;

 _lugar(40,10); 
 escribir_cadena('\n Pulse una tecla para continuar --> ');
 leer_cadena(pausaa);
 

hasta (opcion = 0);


_borrar;
_lugar(10,10);
escribir_cadena('El programa ha concluido');



