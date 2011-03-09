#include "Sfx.h"

/***********************************************
 *			Funciones Públicas				   *
 ***********************************************/

/* Sfx::Sfx(string fname, SoundEngine* soundEngine) */
/*
Creamos un elemento de tipo Sound (que estamos encapsulando), y apuntamos al
SoundEngine que nos va a gestionar para pasarle las órdenes
*/
Sfx::Sfx(string fname, SoundEngine* soundEngine)
{
	this->soundEngine = soundEngine;
	//En la constructora de Sound se carga fname y se almacena en SoundManager
	sound = new Sound(fname,soundEngine);

	volume= -1;
	loop = false;
	playing  = false;
}
		
/* Sfx::~Sfx() */
/*
Eliminamos el elemento Sound que creamos, al soundEngine le ponemos el puntero
a NULL pero no le hacemos el delete pues otras clases podrían estar utilizándolo
*/
Sfx::~Sfx()
{
	delete sound;
	sound = NULL;

	soundEngine = NULL;
}

/* Sfx::play(float volume = -1) */
/*
Establecemos el valor de playing para indicar el estado de la música
y reproducimos la música usando SoundEngine.
*/
void Sfx::play(float volume)
{
	playing = true;
	soundEngine->playSound(sound, volume); //loop = false por defecto
}

/* void Sfx::stop() */
/*
Paramos la reproducción de este sonido utilizando la funcionalidad
de SoundEngine
*/
void Sfx::stop()
{
	playing = false;
	soundEngine->stopSound(sound);
}
		
void Sfx::fade (float volume, int time)
{
	soundEngine->fadeSound(sound,volume,time);
}

/* Sfx::isPlaying() */
/*
Comprobamos si no esta parado y si es el que se esta reproduciendo en este 
momento (preguntandoselo al SoundEngine) y si se cumplen todas esas condiciones devolvemos True.
*/
bool Sfx::isPlaying()
{
	return playing && soundEngine->isSoundPlaying(sound);
}

/* Sfx::setVolume(bool loop) */
/*
Establecemos el valor del volumen y luego si nuestro sonido se
esta reproduciendo cambiamos también dicho valor en el SoundEngine.
*/
void Sfx::setVolume (float volume)
{
	this->volume = volume;

	if (soundEngine->isSoundPlaying(sound))
		soundEngine->setSoundVolume(volume);
}