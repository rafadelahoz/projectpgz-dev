#include "Overworld.h"

// Constructora.
Overworld::Overworld(int worldS, vector<ZoneInfo>* zonesI, vector<DungeonInfo>* dungeonsI,
                     vector<SafeZoneInfo>* safeZonesI){
	// Asignamos variables a atributos.
	worldSize = worldS;
	zonesInfo = zonesI;
	dungeonsInfo = dungeonsI;
	safeZonesInfo = safeZonesI;

	mapTileMatrix = new vector<MapTile*>();

	// Calculamos un tama�o del mundo a partir de worldSize.
	worldSizeH = screenHeight * 20;//screenHeight * ((rand() % (5 * worldSize)) + 5); 	// Aseguramos un mundo de al menos 5 x 5 pantallas.
	worldSizeW = screenWidth * 20;//screenWidth *  ((rand() % (5 * worldSize)) + 5);

	// Inicializamos tileMapMatrix
	for (int i=0; i< (worldSizeH*worldSizeW); i++)
		mapTileMatrix->push_back(new MapTile());
}


Overworld::~Overworld(){
	vector<MapTile*>::iterator it;
    for(it = mapTileMatrix->begin(); it != mapTileMatrix->end(); it++)
        if ((*it) != NULL)
        {
			delete (*it);
			(*it) = NULL;
        }
	delete mapTileMatrix;
	mapTileMatrix = NULL;
}

// Getters utiles:
int Overworld::getNumZones(){
	if (zonesInfo != NULL)
		return zonesInfo->size();
	return 0;
}

int Overworld::getNumDungeons(){
	if (dungeonsInfo != NULL)
		return dungeonsInfo->size();
	return 0;
}

int Overworld::getNumSafeZones(){
	if (safeZonesInfo != NULL)
		return safeZonesInfo->size();
	return 0;
}

vector<ZoneInfo>* Overworld::getZonesInfo(){
	return zonesInfo;
}


int Overworld::getWorldSizeH(){return worldSizeH;}

int Overworld::getWorldSizeW(){return worldSizeW;}

MapTile* Overworld::getMapTile(int x, int y)
{
    return mapTileMatrix->at(y*worldSizeW + x);
}
