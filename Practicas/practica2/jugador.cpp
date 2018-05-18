#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <queue>
using namespace std;

int contador = 0;


///////////////////////REINICIO/////////////////////
void ComportamientoJugador::Reiniciar(){
	fil = col = 99;
	ultimaAccion = actIDLE;
	bien_situado = false;
	brujula = 0;
	misObjetos.clear();
	mi_hueso = mi_regalo = mi_bikini = mi_calzado = mi_llave = false;
	girar = false;

      //reinicio de mapa auxiliar
      for (int i = 0; i < 200; ++i)
        for (int j = 0; j < 200; ++j)
          mapaAuxiliar[i][j] = '?';

	estoy_ejecutando_plan =false;
	error_plan =false;
}


/////////////PATHFINDING/////////////////
int ComportamientoJugador::distanciaManhattan(const estado &origen, const estado &destino){
	return (origen.fila - destino.fila) + (origen.columna - destino.columna);
}

bool ComportamientoJugador::pathFinding(const estado &origen, const estado &destino, list<Action> &plan){

	//Borro la lista
	plan.clear();



	list<nodo> nodos_abiertos, nodos_cerrados;
	list<nodo>::iterator it, mejor_nodo;

	estado st = origen;

	int distManh = distanciaManhattan(origen,destino);

	//nooo inicial
	nodo inicial(origen.fila, origen.columna, NULL, distManh);
	
	//ponemos el nodo inicial en abiertos
	nodos_abiertos.push_back(inicial);

	//mientras haya elementos en los nodos abiertos
	while(!nodos_abiertos.empty()){
		//iteramos sobre todos los nodos abiertos y buscamos el de menor distancia manhattan
		mejor_nodo = nodos_abiertos.begin();
		for(it = nodos_abiertos.begin(); it != nodos_abiertos.end(); it++){
			if((*it).distancia < (*mejor_nodo).distancia){
				mejor_nodo = it;
			}
		}

		//lo sacamos de nodos abiertos y lo ponemos en el cerrado
		nodos_abiertos.erase(mejor_nodo);
		nodo nodoActual = *mejor_nodo;

		nodos_cerrados.push_back(nodoActual);

		// si el ultimo elemento enviado al nodo cerrado coincide con el destino
		if (nodoActual.fila == destino.fila && nodoActual.columna == destino.columna){
			list<nodo>::iterator it2,it3;

			nodo* nod = &nodoActual;

			list<nodo> camino;
			for ( it3 = nodos_cerrados.end(); it3 != nodos_cerrados.begin() ; it3--){
				camino.push_front(*nod);
				nod = nod->padre;
			}

			

			

			for (it2 = camino.begin(); it2 != camino.end(); it2++){
				if (st.fila < it2->fila){
					switch(st.orientacion){
						case 0: plan.push_back(actFORWARD); break;
						case 1: plan.push_back(actTURN_L); break;
						case 2: plan.push_back(actTURN_L); plan.push_back(actTURN_L); break;
						case 3: plan.push_back(actTURN_R); break;
					}
					st.fila -= 1;
					st.orientacion = 0;
				}else if(st.fila > it2->fila){
					switch(st.orientacion){
						case 2: plan.push_back(actFORWARD); break;
						case 3: plan.push_back(actTURN_L); break;
						case 0: plan.push_back(actTURN_L); plan.push_back(actTURN_L); break;
						case 1: plan.push_back(actTURN_R); break;
					}
					st.fila += 1;
					st.orientacion = 2;
				}else if(st.columna < it2->columna){
					switch(st.orientacion){
						case 1: plan.push_back(actFORWARD); break;
						case 0: plan.push_back(actTURN_L); break;
						case 3: plan.push_back(actTURN_L); plan.push_back(actTURN_L); break;
						case 2: plan.push_back(actTURN_R); break;
					}
					st.columna -= 1;
					st.orientacion = 3;
				}else if(st.columna > it2->columna){
					switch(st.orientacion){
						case 3: plan.push_back(actFORWARD); break;
						case 2: plan.push_back(actTURN_L); break;
						case 1: plan.push_back(actTURN_L); plan.push_back(actTURN_L); break;
						case 0: plan.push_back(actTURN_R); break;
					}
					st.columna += 1;
					st.orientacion = 1;
				}


			}
			
			return true;
		}else{
			return false;
		}


		//hijos posibles
		//norte
		if (!hayObstaculo(nodoActual.fila-1,nodoActual.columna)){
			estado posible_padre;
			posible_padre.fila = nodoActual.fila-1;
			posible_padre.columna = nodoActual.columna;
			nodo nuevoNodo(posible_padre.fila,posible_padre.columna, &nodoActual,  distanciaManhattan(posible_padre,destino));
			nodos_abiertos.push_back(nuevoNodo);
		}

		//sur
		if (!hayObstaculo(nodoActual.fila+1,nodoActual.columna)){
			estado posible_padre;
			posible_padre.fila = nodoActual.fila+1;
			posible_padre.columna = nodoActual.columna;
			nodo nuevoNodo(posible_padre.fila,posible_padre.columna, &nodoActual,  distanciaManhattan(posible_padre,destino));
			nodos_abiertos.push_back(nuevoNodo);
		}

		//este
		if (!hayObstaculo(nodoActual.fila,nodoActual.columna+1)){
			estado posible_padre;
			posible_padre.fila = nodoActual.fila;
			posible_padre.columna = nodoActual.columna+1;
			nodo nuevoNodo(posible_padre.fila,posible_padre.columna, &nodoActual,  distanciaManhattan(posible_padre,destino));
			nodos_abiertos.push_back(nuevoNodo);
		}

		//oeste
		if (!hayObstaculo(nodoActual.fila,nodoActual.columna-1)){
			estado posible_padre;
			posible_padre.fila = nodoActual.fila;
			posible_padre.columna = nodoActual.columna-1;
			nodo nuevoNodo(posible_padre.fila,posible_padre.columna, &nodoActual,  distanciaManhattan(posible_padre,destino));
			nodos_abiertos.push_back(nuevoNodo);
		}


	}

}

//////////////////////////PULGARCITO//////////////////
Action ComportamientoJugador::caminarPulgarcito(const Sensores &sensores,const int &brujula){

	Action action;
	int norte,sur,este,oeste;

	//actualizar casilla 0 en el mapa de pulgarcito
	mapaPulgarcito[fil][col] = tiempo;


	//Decidir siguiente accion:
	switch(brujula){
		case 0://norte
			norte = hayObstaculo(fil-1,col);
			este = hayObstaculo(fil,col+1);
			oeste = hayObstaculo(fil,col-1);
		break;

		case 1://este
			norte = hayObstaculo(fil,col+1);
			este = hayObstaculo(fil+1,col);
			oeste = hayObstaculo(fil-1,col);
		break;

		case 2://sur
			norte = hayObstaculo(fil+1,col);
			este = hayObstaculo(fil,col-1);
			oeste = hayObstaculo(fil,col+1);
		break;

		case 3://oeste
			norte = hayObstaculo(fil,col-1);
			este = hayObstaculo(fil-1,col);
			oeste = hayObstaculo(fil+1,col);
		break;
	}

	if (oeste == 9999999 && norte == 9999999 && este == 9999999){//Si nuestro pj esta encerrado
		action= actTURN_R;
	}else if(este < norte || oeste < norte){// Si la izq o la der es menor que el norte
		if (este <= oeste){				
			action= actTURN_R;
		}else{							
			action= actTURN_L;
		}		
	}else{	// si no, avanzamos
		action= actFORWARD;
	}

	return action;
}

void ComportamientoJugador::asignarOrientacionPulgarcito(const int &dato){
	switch(brujula){
		case 0://norte
			mapaPulgarcito[fil-1][col] = dato;
		case 1://este
			mapaPulgarcito[fil][col+1] = dato;
		break;

		case 2://sur
			mapaPulgarcito[fil+1][col] = dato;
		break;

		case 3:// oeste
			mapaPulgarcito[fil][col-1] = dato;
		break;
	}
}

int ComportamientoJugador::consultarOrientacionPulgarcito(){
	switch(brujula){
		case 0:// norte
			return mapaPulgarcito[fil-1][col];
		case 1://este
			return mapaPulgarcito[fil][col+1];
		break;

		case 2://sur
			return mapaPulgarcito[fil+1][col];
		break;

		case 3:// oeste
			return mapaPulgarcito[fil][col-1];
		break;
	}
}

////////////////////////OBSTÁCULOS////////////////////
int ComportamientoJugador::hayObstaculo(const int &fila, const int &columna){
	if (bien_situado){
		bool hay_obstaculo1 = mapaResultado[fila][columna] == 'P' || mapaResultado[fila][columna] == 'M' || FrenteUnObjeto || FrenteUnNpc;
		bool hay_obstaculo2_1 = !mi_bikini && mapaResultado[fila][columna] == 'A';
		bool hay_obstaculo2_2 = mi_bikini && casilla_actual == 'B' && mapaResultado[fila][columna] == 'A';
		bool hay_obstaculo3 = (!mi_calzado && mapaResultado[fila][columna] == 'B')  || ( mi_calzado && casilla_actual == 'A' && mapaResultado[fila][columna] == 'B');
		bool hay_obstaculo4 = !mi_llave && mapaResultado[fila][columna] == 'D';
		
		if (hay_obstaculo1 || hay_obstaculo2_1 || hay_obstaculo2_2 || hay_obstaculo3 || hay_obstaculo4){
			return 9999999;
		}else{
			return mapaPulgarcito[fila][columna];	
		}
	}else{
		bool hay_obstaculo1 = mapaAuxiliar[fila][columna] == 'P' || mapaAuxiliar[fila][columna] == 'M' || FrenteUnObjeto || FrenteUnNpc;
		bool hay_obstaculo2_1 = !mi_bikini && mapaAuxiliar[fila][columna] == 'A';
		bool hay_obstaculo2_2 = mi_bikini && casilla_actual == 'B' && mapaAuxiliar[fila][columna] == 'A';
		bool hay_obstaculo3 = (!mi_calzado && mapaAuxiliar[fila][columna] == 'B')  || ( mi_calzado && casilla_actual == 'A' && mapaAuxiliar[fila][columna] == 'B');
		bool hay_obstaculo4 = !mi_llave && mapaAuxiliar[fila][columna] == 'D';
		
		if (hay_obstaculo1 || hay_obstaculo2_1 || hay_obstaculo2_2 || hay_obstaculo3 || hay_obstaculo4){
			return 9999999;
		}else{
			return mapaPulgarcito[fila][columna];		
		}
	}
}

bool hayObstaculoDelante(const vector<unsigned char> & terreno, const vector<unsigned char> & superficie){
	if ( (terreno[2]=='T' || terreno[2]=='S' || terreno[2]=='K') &&	superficie[2]=='_'){
		return false;
	}
	else{
		return true;
 	}
}

////////////////////////////////OBJETOS//////////////////////////
bool ComportamientoJugador::tengounObjeto(const Sensores& sensores, const char &objetoQueBusco){
	if (sensores.objetoActivo == objetoQueBusco){
		return true;
	}else if (mi_bikini && objetoQueBusco == '1'){
		return true;
	}else if(mi_calzado && objetoQueBusco == '2'){
		return true;
	}else if(mi_hueso && objetoQueBusco == '0'){
		return true;
	}else if(mi_llave && objetoQueBusco == '3'){
		return true;
	}else{
		return false;
	}
}

Action ComportamientoJugador::meterEnMochila(const char &obj){
	misObjetos.push_back(obj);
	return actPUSH;
}

Action ComportamientoJugador::equiparEnMano(){
	for (int i = 0; i < misObjetos.size(); ++i){
		misObjetos[i]= misObjetos[i+1];
	}
	misObjetos.pop_back();
	return actPOP;
}


/////////////////////////PINTAR EL PLAN///////////////
void PintaPlan(list<Action> plan){
	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			cout << "A ";
		}
		else if (*it == actTURN_R){
			cout << "D ";
		}
		else if (*it == actTURN_L){
			cout << "I ";
		}
		else {
			cout << "- ";
		}
		it++;
	}
	cout << endl;
}

///////////////THINK/////////////////
Action ComportamientoJugador::think(Sensores sensores){
	Action accion = actIDLE;
	cout << contador << endl;
	tiempo++;
	contador++;


	//////////////////ORIENTACIÓN///////////////////
	if (sensores.reset){
		Reiniciar();
	}

	switch(ultimaAccion){
		case actFORWARD:
			//Si avanza:
			if (!sensores.colision){ 				
				switch(brujula){
					case 0: //norte
						fil--;
						break;
					case 1: //este
						col++;
						break;
					case 2: //Sur
						fil++;
						break;
					case 3: //oeste
						col--;
						break;
				} 
			}
		break;
		case actTURN_L:
			//si gira a la izquierda
			brujula = (brujula+3)%4;
			break;
		case actTURN_R:
			//si gira a la derecha
			brujula = (brujula+1)%4;
			break;
		case actPICKUP:
			//si coge un objeto
			switch(sensores.objetoActivo){
				case '0':
					mi_hueso = true;
				break;
				case '1':
					mi_bikini = true;
				break;
				case '2':
					mi_calzado = true;
				break;
				case '3':
					mi_llave = true;
				break;
				case '4':
					mi_regalo = true;
					estoy_ejecutando_plan = false;
				break;
			}
		break;
		case actGIVE:
			if (sensores.objetoActivo == '_' && sensores.superficie[2] == 'r'){
				mi_regalo = false;
			}
		break;
		case actPUTDOWN:
			switch(sensores.superficie[2]){
				case '0':
					mi_hueso = false;
				break;
				case '1':
					mi_bikini = false;
				break;
				case '2':
					mi_calzado = false;
				break;
				case '3':
					mi_llave = false;
				break;
			
			}
		break;
	}

	//////////////////ACTUALIZAR MAPA///////////////////
	if (tiempo == 19999){ //pintar con precipicio los bordes del final
		for (int i = 0; i < 100; ++i){
			mapaResultado[i][0] = 'P';
			mapaResultado[i][1] = 'P';
			mapaResultado[i][2] = 'P';

			mapaResultado[i][97] = 'P';
			mapaResultado[i][98] = 'P';
			mapaResultado[i][99] = 'P';

			mapaResultado[0][0] = 'P';
			mapaResultado[1][0] = 'P';
			mapaResultado[2][0] = 'P';

			mapaResultado[97][0] = 'P';
			mapaResultado[98][0] = 'P';
			mapaResultado[99][0] = 'P';
		}
	}

	if (bien_situado){
		mapaResultado[fil][col] = sensores.terreno[0];

		int indice=1;
		switch(brujula){
			case 0:
				for (int fila = 1; fila <= 3 ; ++fila){
					for (int columna = -fila; columna <= fila; ++columna){
						mapaResultado[fil-fila][col+columna] = sensores.terreno[indice];
						indice++;
					}
				}
			break;

			case 1:
				for (int columna = 1; columna <= 3 ; ++columna){
					for (int fila = -columna; fila <= columna; ++fila){
						mapaResultado[fil+fila][col+columna] = sensores.terreno[indice];
						indice++;
					}
				}
			break;

			case 2:
				for (int fila = 1; fila <= 3 ; ++fila){
					for (int columna = fila; columna >= -fila; --columna){
						mapaResultado[fil+fila][col+columna] = sensores.terreno[indice];
						indice++;
					}
				}

			break;

			case 3:
				for (int columna = 1; columna <= 3 ; ++columna){
					for (int fila = -columna; fila <= columna; ++fila){
						mapaResultado[fil-fila][col-columna] = sensores.terreno[indice];
						indice++;
					}
				}
			break;
		}
	}else{
		mapaAuxiliar[fil][col] = sensores.terreno[0];

		int indice=1;
		switch(brujula){
			case 0:
				for (int fila = 1; fila <= 3 ; ++fila){
					for (int columna = -fila; columna <= fila; ++columna){
						mapaAuxiliar[fil-fila][col+columna] = sensores.terreno[indice];
						indice++;
					}
				}
			break;


			case 1:
				for (int columna = 1; columna <= 3 ; ++columna){
					for (int fila = -columna; fila <= columna; ++fila){
						mapaAuxiliar[fil+fila][col+columna] = sensores.terreno[indice];
						indice++;
					}
				}
			break;

			case 2:
				for (int fila = 1; fila <= 3 ; ++fila){
					for (int columna = fila; columna >= -fila; --columna){
						mapaAuxiliar[fil+fila][col+columna] = sensores.terreno[indice];
						indice++;
					}
				}

			break;

			case 3:
				for (int columna = 1; columna <= 3 ; ++columna){
					for (int fila = -columna; fila <= columna; ++fila){
						mapaAuxiliar[fil-fila][col-columna] = sensores.terreno[indice];
						indice++;
					}
				}
			break;
		}
	}


	if (sensores.terreno[0]=='K' && !bien_situado){ 
		int df = fil- sensores.mensajeF;
		int dc = col- sensores.mensajeC;
		fil = sensores.mensajeF;
		col = sensores.mensajeC;
		
		for (int i = 0; i < 100; ++i){
			for (int j = 0; j < 100; ++j){
				if (mapaResultado[i][j] == '?'){
					mapaResultado[i][j] = mapaAuxiliar[df + i][dc + j];
				}
			}
		}
		bien_situado = true;
	}

	/////////////PLAN////////////////
	if (plan.size() > (sensores.vida+5) && estoy_ejecutando_plan) error_plan = true;

	if (error_plan) estoy_ejecutando_plan = false;

	if (estoy_ejecutando_plan && plan.front() == actFORWARD && hayObstaculoDelante(sensores.terreno,sensores.superficie)){
		if (sensores.terreno[2] == 'A' && sensores.terreno[0] != 'B' && mi_bikini){
				if (sensores.objetoActivo != '1'){
					if (sensores.objetoActivo != '_') accion = meterEnMochila(sensores.objetoActivo);
					else accion = equiparEnMano();
				}else{
					error_plan = false;
				}

		}else if (sensores.terreno[2] == 'B'
		 && sensores.terreno[0] != 'A' && mi_calzado){
				if (sensores.objetoActivo != '2'){
					if (sensores.objetoActivo != '_') accion = meterEnMochila(sensores.objetoActivo);
					else accion = equiparEnMano();
				}else{
					error_plan = false;
				}

		}else if (sensores.superficie[2] == 'a' || (sensores.superficie[2] == 'l' && !mi_hueso) ){
			plan.push_front(actIDLE);
			error_plan = false;

		}else if(sensores.superficie[2] == 'l' && mi_hueso && sensores.terreno[0] != 'A' && sensores.terreno[0] != 'B'){
			if (sensores.objetoActivo != '0'){
				if (sensores.objetoActivo != '_') accion = meterEnMochila(sensores.objetoActivo);
				else accion = equiparEnMano();		
			}else{
				error_plan = false;
			}

		}else{
			error_plan = true;
		}
	}else{
		error_plan = false;
	}


	if (accion != actIDLE) return accion;



	/////////////////////////////CONSTRUIR PLAN/////////////////////////


	if (bien_situado and !mi_regalo and !estoy_ejecutando_plan and sensores.regalos.size()>0){
		estado origen;
		origen.fila = fil;
		origen.columna = col;
		origen.orientacion = brujula;

		estado destino;
		destino.fila = sensores.regalos[0].first;
		destino.columna = sensores.regalos[0].second;

		estoy_ejecutando_plan = pathFinding(origen, destino, plan);
	}



	////////////////////////////COMPORTAMIENTO DEL AGENTE/////////////////////////////	


	casilla_actual = sensores.terreno[0];
	FrenteUnObjeto = sensores.superficie[2] == '0' || sensores.superficie[2] == '1' || sensores.superficie[2] == '2' || sensores.superficie[2] == '3' || sensores.superficie[2] == '4';
	FrenteUnNpc = sensores.superficie[2] == 'l' || sensores.superficie[2] == 'a' || sensores.superficie[2] == 'r';


	// si hay un regalo lo cogemos
	if (sensores.superficie[2] == '4' && sensores.objetoActivo == '_'){
		accion = actPICKUP;
	//si encontramos un rey y tenemos el regalo
	}else if (sensores.superficie[2] == 'r' && sensores.objetoActivo == '4'){
		accion = actGIVE;
	}else if(sensores.superficie[2] == 'r' && !mi_regalo){
		asignarOrientacionPulgarcito(tiempo+3);
		accion = caminarPulgarcito(sensores,brujula);




	// accion dependiendo del plan
	}else if (estoy_ejecutando_plan && !error_plan){
		accion = plan.front();
		plan.erase(plan.begin());
		PintaPlan(plan);
		if (plan.size()==0){
			estoy_ejecutando_plan = false;
		}

	//cuando muero soltar los objetos
	}else if (sensores.vida < 25 && (casilla_actual != 'B' && casilla_actual != 'A') && (sensores.terreno[2] != 'B' && sensores.terreno[2] != 'A' && sensores.terreno[2] != 'M' && sensores.terreno[2] != 'K') && (sensores.superficie[2] == '_' || girar ) && (misObjetos.size() > 0 || sensores.objetoActivo != '_') ){
		if (girar){
			accion = actTURN_R;
			girar = false;
		}else if (sensores.objetoActivo == '_'){
			accion = equiparEnMano();
		}else{
			accion = actPUTDOWN;
			girar = true;
		}

	//si hay una puerta
	}else if ( sensores.superficie[2] == 'd' && mi_llave && sensores.terreno[0] != 'A' && sensores.terreno[0] != 'B'){
		if (sensores.objetoActivo == '3'){
			accion = actGIVE;

		}else{
			if (sensores.objetoActivo != '_') accion = meterEnMochila(sensores.objetoActivo);
			else accion = equiparEnMano();
		}

	// si hay un lobo
	}else if ( sensores.superficie[2] == 'l' && mi_hueso && sensores.terreno[0] != 'A' && sensores.terreno[0] != 'B'){
		if (sensores.objetoActivo == '0'){
			accion = actGIVE;
			mi_hueso= false;			
		}else{
			if (sensores.objetoActivo != '_') accion = meterEnMochila(sensores.objetoActivo);
			else accion = equiparEnMano();
		}

	//si hay un npc o un lobo y no tenemos un objeto
	}else if( sensores.superficie[2] == 'a' || (sensores.superficie[2] == 'l' && !mi_hueso) ){
		asignarOrientacionPulgarcito(tiempo+3);
		accion = caminarPulgarcito(sensores,brujula);

	// si encontramos un objeto que no tenemos
	}else if (FrenteUnObjeto && sensores.vida > 25 && sensores.terreno[0] != 'A' && sensores.terreno[0] != 'B' && !tengounObjeto(sensores,sensores.superficie[2]))  { //Si tenemos un objeto delante, y no tenemos ninguno igual en la mochila o en la mano, lo cojeremos.
		if (sensores.objetoActivo != '_') accion = meterEnMochila(sensores.objetoActivo); // meterEnMochila si lo tenemos en la mano
		else accion = actPICKUP; // si la mano está vacia cogemos el objeto
		asignarOrientacionPulgarcito(0);			
		

	// si encontramos un objeto que tenemos
	}else if(FrenteUnObjeto && consultarOrientacionPulgarcito() < 12000){
		asignarOrientacionPulgarcito(9999999);
		accion = caminarPulgarcito(sensores,brujula);


	// si hay agua
	}else if (sensores.terreno[2] == 'A' && mi_bikini && sensores.terreno[0] != 'B'){
		if (sensores.objetoActivo == '1'){
			accion = caminarPulgarcito(sensores,brujula);
		}else{
			if (sensores.objetoActivo != '_') accion = meterEnMochila(sensores.objetoActivo);
			else accion = equiparEnMano();
		}


	// si hay bosque
	}else if (sensores.terreno[2] == 'B' && mi_calzado && sensores.terreno[0] != 'A'){
		if (sensores.objetoActivo == '2'){
			accion = caminarPulgarcito(sensores,brujula);
		}else{
			if (sensores.objetoActivo != '_') accion = meterEnMochila(sensores.objetoActivo);
			else accion = equiparEnMano();
		}

	// si no encontramos nada andamos con normalidad
	}else{
		accion = caminarPulgarcito(sensores,brujula);
	}


	ultimaAccion = accion;
	return accion;
}


int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
