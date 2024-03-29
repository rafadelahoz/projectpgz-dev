#pragma once

#ifndef __TILESET_H__
#define __TILESET_H__

#include "GfxEngine.h"
#include "Image.h"
#include "Graphic.h"

using namespace std;

//!Encapsula un elemento gr�fico que se divide en tiles de tama�o configurable
/*!
	Proporciona m�todos para acceder a tiles de forma sencilla. Pertenece a la clase Graphic
	por ser un elemento gr�fico con elementos del mapa a pintar.
*/

class TileSet 
{
	private:
		//! Atributos de la clase tileSet.
		/*!
			\param gfxEngine contiene la informaci�n de la gesti�n de los gr�ficos.
			\param image contiene la imagen asociada al tileSet.
			\param path contiene la ruta en la que se encuentra el tileSet.
			\param tileW contiene el ancho de un tile.
			\param tileH contiene el alto de un tile.
			\param tileSetW contiene el ancho del tileSet.
			\param tileSetH contiene el alto del tileSet.
			\param colNumber contiene el numero de tiles de ancho del tileSet.
			\param rowNumber contiene el n�mero de tiles de alto del tileSet.
		*/
		GfxEngine* gfxEngine;
		Image* image;
		string path;
		int tileW;
		int tileH;
		int tileSetW;
		int tileSetH;
		int colNumber;
		int rowNumber;
	
	public:
		//! Construye un tileSet a partir de la ruta el alto y ancho de un tile el motor grafico y su transparencia
		/*!
			\param path Contiene la ruta en la que se encuentra el tileSet a cargar
			\param tileW Contiene el ancho de un tile
			\param tileH Contiene el alto de un tile
			\param gfxEngine Contiene la informaci�n de la gesti�n de los gr�ficos.
			\param transparent Contiene la informaci�n de si debe llevar o no transparencia el tileSet (por defecto a true)
		*/
		TileSet(string path, int tileW, int tileH, GfxEngine* gfxEngine, bool transparent = true);
		
		//! Elimina el tileSet
		~TileSet();
		
		//! Obtiene de la imagen.
		/*!
			\return la imagen del tileSet
		*/
		Image* getImg();
		
		//! Obtiene del numero de columnas.
		/*!
			\return el n�mero de columnas
		*/
		int getColumns();
		
		//! Obtiene del numero de filas.
		/*!
			\return el n�mero de filas
		*/
		int getRows();

		//! Obtiene el ancho de un tile
		int getTileW();

		//! Obtiene el alto de un tile
		int getTileH();
};

#endif