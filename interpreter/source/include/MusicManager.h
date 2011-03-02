#pragma once

#ifndef _MUSICMANGER_H_
#define _MUSICMANGER_H_

#include <map>
using std::string;
//#include "SFML/Music.hpp"
#include "MusicManagerItem.h"

//! Mantiene la colecci�n de m�sicas en memoria.
/*!
	Dentro de un mismo juego distintos elementos pueden compartir
	un mismo archivo de audio musical. Este elemento
	no deber�a cargarse varias veces en memoria,
	sino compartirse entre todos con el consecuente ahorro.
	
	De esta tarea se encarga MusicManager, que mantiene la lista
	de sonidos cargados en memoria de forma transparente al programador.

	Se asegura de que las m�sicas cargadas desde disco a memoria
	se mantienen sin duplicados y que se eliminan las que no se 
	necesiten, mediante un contador de enlaces asociado a cada
	m�sica.
	
	Proporciona funcionalidad para comprobar si una m�sica ya
	est� cargada, obtenerla en tal caso o a�adir una nueva entrada
	en otro caso.
*/
class MusicManager {
	private:
		//! Mapa de m�sicas enlazadas por nombre de archivo fuente
		map<string,MusicManagerItem*>* list;

	public:
		//! Instancia el MusicManager
		MusicManager();
		
		//! Destruye el MusicManager
		~MusicManager();
		
		//! Comprueba si la m�sica obtenida a partir del archivo fname ya est� cargada.
		/*!
			Este m�todo tiene un coste computacional reducido (el mapa se indexa por nombre de archivo)
			\param fname Nombre de archivo fuente
			\return true si la m�sica ya ha sido cargada
		*/
		bool isLoaded(string fname);
		
		//! Obtiene una m�sica almacenada en el manager
		/*!
			\param fname Nombre de archivo fuente solicitado
			\return Puntero a la m�sica cargada en memoria
		*/
		const Music* getMusic(string fname);
		
		//! A�ade una nueva m�sica a la lista
		/*!
			\param fname Nombre de archivo fuente
			\param music Puntero a la m�sica
			\return false si la m�sica o el fname ya estaban en memoria
		*/
		bool setMusic(string fname, Music* music);
		
		//! Elimina una entrada de la lista
		/*!
			Elimina la entrada de la lista y tambi�n la m�sica de memoria
			\param fname Nombre de archivo fuente
			\return true si se ha eliminado correctamente
		*/
		bool remove(string fname);
};

#endif // _SURFACEMANAGER_H_