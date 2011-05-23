#pragma once

#ifndef _WORLDDECORATOR_H_
#define _WORLDDECORATOR_H_

#include <list>

#include "Decorator.h"
#include "WorldAutoTiler.h"
#include "Screen.h"
#include "OwScreen.h"
#include "DBManager.h"

using namespace std;

class WorldDecorator : public Decorator
{
	private:
		static const int N_ATTEMPTS = 4; // n�mero de veces que va a intentar colocar una decoraci�n

	protected:

		// Informaci�n de con qu� terrenos (y decoraciones) se est�n decorando las mazmorras
		struct screenInfo{
			short solidId;
			short waterId;
			short pathId;
			short floorId;
		};

		// guarda informaci�n sobre los terrenos de la pantalla
		screenInfo info;

	public:

		// Constructora
		WorldDecorator(DBManager* db);

		// Destructora
		~WorldDecorator();

		// Inicia el decorador con el tipo de zona, el tema del juego, y el tileset
		void init(string zone, string theme, short tileSetId);

		// Decora la pantalla pasada por par�metro (en funci�n de la incializaci�n anterior)
		virtual void decorate(Screen* screen);

		// coloca tantas decoraciones como indique nDecos, posUsed es el vector de posiciones ya utilizadas
		void place_decos(Screen* screen, int nDecos, Decoration::DecorationSize size, std::vector<int>* posUsed);

		// Devuelve la id de la decoraci�n que coloca
		int place_deco(Screen* s, Decoration::DecorationSize size, Decoration::DecorationType type,int idDeco, int pos);
};

#endif