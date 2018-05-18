#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include <stdlib.h>
using namespace std;



Action ComportamientoJugador::movimientoAleatorio(Sensores sensores){
	Action accion;

	if (!girar_derecha){
			accion = actTURN_L;
			if (rand()%2==0) girar_derecha = true;
			else girar_derecha = false;
	}
	else{
			accion = actTURN_R;
			if (rand()%2==0) girar_derecha = true;
			else girar_derecha = false;
	}
	return accion;

}
Action ComportamientoJugador::think(Sensores sensores){

	Action accion = actIDLE;


	// En esta matriz de tamano 100x100 hay que escribir el mapa solucion
	// mapaResultado[fila][columna] = lo que hay en fila columna

	//si el personaje se ha reiniciado, cambiar el valor de estas variables
	if(reinicio){
		fil = 99;
		col = 99;
		brujula = 0;
		bien_situado = false;
		ultimaAccion = actIDLE;
		reinicio = false;
		girar_derecha = false;
	}


	//brújula, saber dónde estoy orientado
	switch (ultimaAccion){
		case actFORWARD:
			if(!sensores.colision){
				switch(brujula){
					case 0: //norte
						fil--;
						break;
					case 1: //este
						col++;
						break;
					case 2: //sur
						fil++;
						break;
					case 3: //oeste
						col--;
						break;
				}
				
			}
			break;
		case actTURN_L:
			brujula = (brujula+3)%4;
			if(rand()%2==0) girar_derecha = true;
			else girar_derecha = false;
			break;
		case actTURN_R:
			brujula = (brujula+1)%4;
			if(rand()%2==0) girar_derecha = true;
			else girar_derecha = false;
			break;
	}

	cout << "Terreno: ";
	for (int i=0; i<sensores.terreno.size(); i++)
		cout << sensores.terreno[i];
	cout << endl;

	cout << "Superficie: ";
	for (int i=0; i<sensores.superficie.size(); i++)
		cout << sensores.superficie[i];
	cout << endl;

	cout << "Colisión: " << sensores.colision << endl;
	cout << "Mochila: " << sensores.mochila << endl;
	cout << "Reset: " << sensores.reset << endl;
	cout << "Vida: " << sensores.vida << endl;
	cout << "objetoActivo: " << sensores.objetoActivo << endl;
	cout << endl;

	//Dibuja el mapa según lo que vean los sensores
	if(bien_situado){
		mapaResultado[fil][col] = sensores.terreno[0];

		

		int indice = 1;
		switch(brujula){
			case 0:
				for(int fila = 1; fila <= 3; ++fila){
						for(int columna = -fila; columna <= fila; ++columna){
							mapaResultado[fil-fila][col+columna] = sensores.terreno[indice];
							indice++;
						}
				}
			break;
			case 1:
				for(int columna = 1; columna <= 3; ++columna){
						for(int fila = -columna; fila <= columna; ++fila){
							mapaResultado[fil+fila][col+columna] = sensores.terreno[indice];
							indice++;
						}
				}
			break;
			case 2:
				for(int fila = 1; fila <= 3; ++fila){
						for(int columna = fila; columna >= fila; --columna){
							mapaResultado[fil+fila][col+columna] = sensores.terreno[indice];
							indice++;
						}
				}
			break;
			case 3:
				for(int columna = 1; columna <= 3; ++columna){
						for(int fila = -columna; fila <= columna; ++fila){
							mapaResultado[fil-fila][col-columna] = sensores.terreno[indice];
							indice++;
						}
				}
			break;
		}

	}
	else{ //mapa auxiliar que dibuja cuando no estoy en casilla K
		mapaAuxiliar[fil][col] = sensores.terreno[0];
		

		int indice = 1;
		switch(brujula){
			case 0:
				for(int fila = 1; fila <= 3; ++fila){
						for(int columna = -fila; columna <= fila; ++columna){
							mapaAuxiliar[fil-fila][col+columna] = sensores.terreno[indice];
							indice++;
						}
				}
			break;
			case 1:
				for(int columna = 1; columna <= 3; ++columna){
						for(int fila = -columna; fila <= columna; ++fila){
							mapaAuxiliar[fil+fila][col+columna] = sensores.terreno[indice];
							indice++;
						}
				}
			break;
			case 2:
				for(int fila = 1; fila <= 3; ++fila){
						for(int columna = fila; columna >= fila; --columna){
							mapaAuxiliar[fil+fila][col+columna] = sensores.terreno[indice];
							indice++;
						}
				}
			break;
			case 3:
				for(int columna = 1; columna <= 3; ++columna){
						for(int fila = -columna; fila <= columna; ++fila){
							mapaAuxiliar[fil-fila][col-columna] = sensores.terreno[indice];
							indice++;
						}
				}
			break;
		}
	}

	//volcado de mapa
	if(sensores.terreno[0]=='K' && !bien_situado){
		int df = fil - sensores.mensajeF;
		int dc = col - sensores.mensajeC;
		fil = sensores.mensajeF;
		col = sensores.mensajeC;

		for(int i = 0; i < 100; ++i){
			for(int j = 0; j < 100; ++j){
				if(mapaResultado[i][j]=='?'){
					mapaResultado[i][j] = mapaAuxiliar[df+i][dc+j];
				}
			}
		}

		bien_situado = true;
	}

	//if-else terreno
	if(sensores.reset == true)
		reinicio = true;
	//BOSQUE
	else if (sensores.terreno[2]=='B')//en el caso de haber bosque
		if(sensores.objetoActivo=='2')//si tengo las zapatillas en la mano
			accion = actFORWARD;
		else if(sensores.mochila=='2' && (sensores.objetoActivo=='0' || sensores.objetoActivo=='1' || sensores.objetoActivo=='3')) //si tengo las zapatillas y algo en la mano, tiro lo que tenga en mano
			accion = actTHROW;
		else if(sensores.mochila=='2' && sensores.objetoActivo=='-') //si tengo las zapatillas en la mochila y nada en la mano me lo equipo
			accion = actPOP;
		else{ //si no, esquivo el bosque (de manera aleatoria)
			accion = movimientoAleatorio(sensores);
		}

	//Precipicio
	else if(sensores.terreno[2]=='P') //esquivar precipicio
		accion = movimientoAleatorio(sensores);
	//AGUA
	else if (sensores.terreno[2]=='A')//en el caso de haber agua
		if(sensores.objetoActivo=='1')//si tengo el biquini en la mano
			 accion = actFORWARD;
		else if(sensores.mochila=='1' && (sensores.objetoActivo=='0' || sensores.objetoActivo=='2' || sensores.objetoActivo=='3')) //si tengo el biquini y algo en la mano, tiro lo que tenga en mano
			accion = actTHROW;
		else if(sensores.mochila=='1' && sensores.objetoActivo=='-') //si tengo el biquini en la mochila y nada en la mano me lo equipo
			accion = actPOP;
		else //si no, esquivo el agua (de manera aleatoria)
			accion = movimientoAleatorio(sensores);
	//MURO
	else if(sensores.terreno[2]=='M') //si me encuentro un muro
		accion = movimientoAleatorio(sensores);
	//PUERTA
	else if(sensores.terreno[2]=='D')
		if(sensores.objetoActivo=='3')//si tengo las zapatillas en la mano
			accion = actGIVE;
		else if(sensores.mochila=='3' && (sensores.objetoActivo=='0' || sensores.objetoActivo=='1' || sensores.objetoActivo=='2')) //si tengo las zapatillas y algo en la mano, tiro lo que tenga en mano
			accion = actTHROW;
		else if(sensores.mochila=='3' && sensores.objetoActivo=='-') //si tengo las zapatillas en la mochila y nada en la mano me lo equipo
			accion = actPOP;
		else //si no, esquivo la puerta (de manera aleatoria)
			accion = movimientoAleatorio(sensores);
		
	else
		accion = actFORWARD;




	//if-else de objetos y npc
	if(sensores.superficie[2]=='0' || sensores.superficie[2]=='1' || sensores.superficie[2]=='2' || sensores.superficie[2]=='3') //recoge un objeto
		//si tiene un objeto en uso lo guarda en la mochila
		if(sensores.objetoActivo=='0' || sensores.objetoActivo=='1' || sensores.objetoActivo=='2' || sensores.objetoActivo=='3')
			accion = actPUSH;
		else //si no tiene  un objeto en la mano lo coge
			accion = actPICKUP;
	else if (sensores.superficie[2]=='l')//en el caso de haber un lobo
		if(sensores.objetoActivo=='0')//si tengo el el hueso en la mano
			accion = actGIVE;
		else if(sensores.mochila=='0' && (sensores.objetoActivo=='1' || sensores.objetoActivo=='2' || sensores.objetoActivo=='3')) //si tengo el hueso y algo en la mano, tiro lo que tenga en mano
			accion = actTHROW;
		else if(sensores.mochila=='0' && sensores.objetoActivo=='-') //si tengo el hueso en la mochila y nada en la mano me lo equipo
			accion = actPOP;
		else //si no, esquivo el lobo (de manera aleatoria)
			accion = movimientoAleatorio(sensores);
	else if(sensores.superficie[2]=='a') //si me encuentro un aldeano esquivar a la izquierda
		accion = actTURN_L;


	cout << pasos << endl;

	//Rellenar terreno "?" con tierra
	if(pasos == 19999){
		for(int i = 0; i < 99; i++)
			for(int j = 0; j < 99; j++)
				if(mapaResultado[i][j]=='?')
					mapaResultado[i][j]='T';
	}

	pasos++;
	ultimaAccion = accion;
	return accion;
						
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
