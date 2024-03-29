/*----------------------------------------------------------------------------------*/
//This code is part of a larger project whose main purpose is to entretain either	//
//by working on it and helping it be better or by playing it in it's actual state	//																			
//																					//
//Copyright (C) 2011 Three Legged Studio											//
//																					//
//    This program is free software; you can redistribute it and/or modify			//
//    it under the terms of the GNU General Public License as published by			//
//    the Free Software Foundation; either version 2, or (at your option)			//
//    any later version.															//
//																					//
//    This program is distributed in the hope that it will be useful,				//
//    but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the					//
//    GNU General Public License for more details.									//
//																					//
//    You should have received a copy of the GNU General Public License				//
//    along with this program; if not, write to the Free Software Foundation,		//
//    Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.			//
//																					//
//	  You can contact us at projectpgz.blogspot.com									//
/*----------------------------------------------------------------------------------*/

#pragma once
#ifndef __DBI_H__
#define __DBI_H__

#include <string>
#include <fstream>
#include <set>
#include <vector>

#include "GameEntity.h"
#include "Paths.h"

#define MAX_STR_LENGTH 255

//enum ItemType { ieNONE, ieHP, ieMP, ieMONEY, ieKEY, ieMAXHP, ieMAXMP, ieBOSSKEY, ieKEYITEM, ieTOOL, iePIGEON };

using namespace std;

/**
	Por supuesto falta todo el tema de parsing de archivos o conexi�n con DBase 
																				**/

class DataBaseInterface
{

public:

	/* ** Estructuras de Datos con la info de los elementos ** */
	// Notas:
	// + Se mantienen las ids en las structs por si hacen falta para checks
	// + Todos los ids son int de forma tentativa, y as� con el resto de atributos
	
	// Player: Nombre | idGfx | Atributos[HpMax?, Strength, ...]
	struct HeroData { short id; string nombre; int gfxId; int hpMax; int mpMax; int strength; int defense; /*Temporal*/string gfxPath; };

	struct GfxData {short id; string path;};

	// Enemy: idEnemy | Nombre | idGfx | [Hp?, Mp?, Str, Def] | [Componente1..N]
	struct EnemyData { int idEnemy; int gfxId; string nombre; int hpMax; int mpMax; int strength; int defense; /*Temporal*/string gfxPath; };

	// Tools: idTool | Nombre | idGfx | ...?
	struct ToolData { int idTool; int gfxId; int dmgType; int gfxAmmo; int maxAmmo; int strength; int type; string nombre; /*Temporal*/ string gfxPath; };

	// Items: idItem | idGfx | Tipo | Power (? nombre ?)
	struct ItemData { int idItem; int power; int effect; int gfxId; string name; };

	// Tsets: idTset | idGfx (o en vez de idGfx guardamos el path?)
	struct TsetData { int idTset; string gfxPath; };

	// PowUp: idPowUp | idGfx | Tipo | Pow
	struct PowerUpData { int idPowUp; string gfxPath; int tipo; int pow; };

	// Bosss: idBoss | Nombre | y m�s! (o quiz�s esto va en Instancias?)
	struct BossData { int idBoss; string nombre; int hp; string keyPath; };

	// Elementos esenciales: id | tipo | path
	struct EssentialElemData { int id; int type; string gfxPath; };

	// NPCs: id | gfxId | sfxId | name | confPath | posiblemente m�s adelante m�s campos necesarios
	struct NPCData { int id; int gfxId; int sfxId; string name; int movComp; };

	// Blockades
	struct BlockadeData { int id; int type; int gfxId; int dmgType; };

	// Pigeon y Objeto clave
	struct ObjData { int id; int gfxId; string name; };

	// PuzzleElems: id | type | gfxId
	struct PuzzleElemData { int id; int type; int gfxId; };

private:
		vector<GfxData>* graphics;			// Vector con los datos de los gr�ficos del juego	
		vector<TsetData>* tileSets;			// Vector con los datos de los tileSets del juego
		vector<EssentialElemData>* essentialElems;
		vector<EnemyData>* enemies;			// Vector con los datos de los enemigos del juego
		vector<NPCData>* npcs;				// Vector con los datos de los NPCs del juego
		vector<ToolData>* tools;			// vector con los datos de las herramientas del juego
		vector<ItemData>* items;			// Vector con los datos de los items del juego
		vector<ItemData>* powUps;			// Vector con los datos de los powerups del juego
		vector<BlockadeData>* blockades;	// Vector con los datos de los bloqueos que aparecen en el juego
		vector<HeroData>* players;			// Vector con los datos de los h�roes del juego
		vector<PuzzleElemData>* puzzle_elems;

		ObjData keyObj, pigeon, finalElem;	// Informaci�n de los pigeon, objetos clave y elemento final que aparecen en el juego

		string doorPath;
		string bossDoorPath;

protected:
	// Temporal, por supuesto
	// Se declaran contenedores temporales a devolver cuando se pida algo
	HeroData hero;
	EnemyData enemy;
	ToolData tool;
	ItemData item;
	TsetData tset;
	//KeyItemData keyItem;
	PowerUpData powUp;
	//PigeonData pidgey;
	//ExchangeItemData xItem;
	BossData boss;
	string bossKey;
	
public:

	/* M�todos */

	DataBaseInterface(void);
	~DataBaseInterface(void);

	// Obtenci�n de recursos
	string getImagePath(int idGfx);
	string getSoundPath(int idSnd);
	string getMusicPath(int idMus);

	// Obtenci�n de datos de elementos
	string getItemName(int idItem);

	// Obtenci�n de elementos
	HeroData getHeroData(int heroNumber = 0); // Por defecto el jugador 1. En caso de multijugador, especificar
	EnemyData getEnemyData(int idEnemy);
	string getEnemyComponentsPath(int idEnemy);
	ToolData getToolData(int idTool);
	ItemData getItemData(int idItem);
	TsetData getTilesetData(int idTset);
	//KeyItemData getKeyItemData();
	ObjData getKeyObjData();
	//PowerUpData getPowerUpData(int idPowUp);
	ItemData getPowerUpData(int idPowUp);
	//PigeonData getPigeonData();
	ObjData getPigeonData();
	ObjData getFinalElemData();
	//ExchangeItemData getExchangeItemData(int idIItem);
	//BossData getBossData(int idBoss);
	string getBossKeyData();

	std::string getShadowGfxPath(GameEntity::Size size);

	void loadData();
	void loadTileSets();
	void loadEssentialElems();
	void loadGfx();
	void loadHeroes();
	void loadTools();
	void loadItems();
	void loadEnemies();
	void loadPowerUps();
	void loadBlockades();
	void loadNPCs();
	void loadDoors();
	void loadPigeon();
	void loadKeyObj();
	void loadFinalElem();
	void loadPuzzleElems();
	
	string getSystem();
	string getSystemDummy();
	string getCursor();
	string getToolCursor();
	string getMainMenu();
	string getLoadMenu();
	string getSShadow();
	string getMShadow();
	string getMinimap();
	string getStateMenuBorders();
	string getFont();
	string getToolMenuBackground();
	string getDoorPath();
	string getBossDoorPath();
	string getCursorMiniMap();
	string getCursorStateMap();
	string getCursorStateSave();
	string getCursorLoad();
	string getRoom();
	string getFloorButton();
	string getHud();
	int getItemNumber();
	int getToolNumber();
	int getToolAtPosition(int position);
	int getItemAtPosition(int position);
	
	ItemData getRandomItem();
};

#endif