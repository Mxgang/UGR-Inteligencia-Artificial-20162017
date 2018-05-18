#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

using namespace std;

class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
    	//inicializacion de las variables de estado
    	fil = 99;
    	col = 99;
    	brujula = 0; //0 norte, 1 este, 2 sur, 3 oeste
    	ultimaAccion = actIDLE;
    	girar_derecha = false;
    	bien_situado = false;
      reinicio = false;
      pasos = 0;

      for(int i = 0; i < filP; i++)
        for(int j = 0; j < colP; j++)
          mapaAuxiliar[i][j] = '?';


    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);

    Action movimientoAleatorio(Sensores sensores);

    int interact(Action accion, int valor);


    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}



  private:
  	//Variables de estado, se usan las que hagan falta
    static const int filP = 200;
    static const int colP = 200;
  	int fil, col, brujula, pasos;
  	Action ultimaAccion;
  	bool girar_derecha, bien_situado, reinicio;
    char mapaAuxiliar[filP][colP];

};


#endif
