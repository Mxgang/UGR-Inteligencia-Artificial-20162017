#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H
#include <list>
#include "comportamientos/comportamiento.hpp"
using namespace std;

class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
     
      fil = col = 99;
      brujula = 0;
      ultimaAccion = actIDLE;
      girar = false;
      bien_situado = false;
      mi_hueso = mi_regalo = mi_bikini = mi_calzado = mi_llave = false;

      //mapa auxiliar
      for (int i = 0; i < 200; ++i){
        for (int j = 0; j < 200; ++j){
          mapaAuxiliar[i][j] = '?';
        }
      }

      //mapa pulgarcito
      for (int i = 0; i < 200; ++i){
        for (int j = 0; j < 200; ++j){
          mapaPulgarcito[i][j] = 0;
        }
      }
      tiempo = 0;

      
      estoy_ejecutando_plan =false;
      error_plan =false;
      
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}
    void Reiniciar();
    void asignarOrientacionPulgarcito(const int &valor);
    int consultarOrientacionPulgarcito();
    int mochila( const char& objetoQueBusco);
    bool tengounObjeto(const Sensores &sensores, const char &objetoQueBusco);
    Action equiparEnMano();
    Action meterEnMochila(const char &objeto);
    Action caminarPulgarcito(const Sensores &sensores,const int &brujula);
    int hayObstaculo(const int &fila, const int &columna);


    Action think(Sensores sensores);

    int interact(Action accion, int valor);


    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}



  private:
    struct nodo{
      int fila;
      int columna;
      int distancia;//distancia hacia objetivo

      nodo* padre;

      nodo(int fila, int columna, nodo* padre, int distancia){
        this->fila = fila;
        this->columna = columna;
        this->distancia = distancia;
        this->padre = padre;
      }


    };

    struct estado{
    	int fila;
    	int columna;
    	int orientacion;
    };

    vector<char> misObjetos;

    // variables de estado
    int fil, col, brujula;
    Action ultimaAccion;
    bool girar;
    bool bien_situado;

    char mapaAuxiliar[200][200];

    //mapa de pulgarcito
    int mapaPulgarcito[200][200];
    int tiempo;


    char casilla_actual;
    
    bool mi_regalo, mi_bikini, mi_calzado, mi_llave, mi_hueso, FrenteUnObjeto, FrenteUnNpc, objetoMarcado;

    
    bool estoy_ejecutando_plan;
    bool error_plan;
    list <Action> plan;
    bool pathFinding(const estado &origen, const estado &destino, list<Action> &plan);
    int distanciaManhattan(const estado &origen, const estado &destino);


};


#endif
