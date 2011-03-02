#include "BGM.h"

/***********************************************
 *			Funciones P�blicas				   *
 ***********************************************/

/* Bgm::Bgm(string fname, SoundEngine* sndEngine) */
/*
Creamos un elemento de tipo Music (que estamos encapsulando), y apuntamos al
SoundEngine que nos va a gestionar para pasarle las �rdenes
*/
Bgm::Bgm(string fname, SoundEngine* sndEngine)
{
	soundEngine = sndEngine;
	music = new Music(fname,sndEngine);

	volume= -1;
	loop = true;
	playing  = false;
	paused = false;
}

/* Bgm::~Bgm() */
/*
Eliminamos el elemento Music que creamos, al sndEngine le ponemos el puntero
a NULL pero no le hacemos el delete pues otras clases podr�an estar utiliz�ndolo
*/
Bgm::~Bgm()
{
	delete music;
	music = NULL;

	soundEngine = NULL;
}

/* Bgm::play(int volume = -1) */
/*
Establecemos los valores de playing y paused para indicar el estado de la m�sica
y reproducimos la m�sica usando SoundEngine.
*/
void Bgm::play(int volume = -1)
{
	playing = true;
	paused = false;
	soundEngine->playMusic(music,volume,loop);
}

/* void Bgm::stop() */
/*
Paramos la reproducci�n de este sonido utilizando la funcionalidad
de SoundEngine
*/
void Bgm::stop()
{
	paused = false;
	playing = false;
	soundEngine->stopMusic();
}

/* Bgm::pause() */
/*
Pausamos la reproducci�n de este sonido utilizando la funcionalidad
de SoundEngine
*/
void Bgm::pause()
{
	paused = true;
	soundEngine->pauseMusic();
}

/* Bgm::resume() */
/*
Reanudamos la reproducci�n de este sonido, si estaba previamente
pausado utilizando la funcionalidad de SoundEngine
*/
void Bgm::resume()
{
	if (paused)
	{
		soundEngine->resumeMusic();
		paused = false;
	}
}

/* Bgm::isPlaying() */
/*
Comprobamos si esta esta m�sica pausada, si no esta parada y por
�ltimo si es la que se esta reproduciendo en este momento (preguntandoselo
al SoundEngine) y si se cumplen todas esas condiciones devolvemos True.
*/
bool Bgm::isPlaying()
{
	return !paused && playing && soundEngine->isMusicPlaying(this);
}

/* Bgm::setLoop(bool loop) */
/*
Establecemos el valor del loop y luego si nuestra m�sica es la que se 
esta reproduciendo cambiamos tambi�n dicho valor en el SoundEngine.
*/
void Bgm::setLoop(bool loop)
{
	this->loop = loop;
	
	if (soundEngine->isMusicPlaying(this))
		soundEngine->setLoop(loop);
}

/* Bgm::setVolume(bool loop) */
/*
Establecemos el valor del volumen y luego si nuestra m�sica es la que se 
esta reproduciendo cambiamos tambi�n dicho valor en el SoundEngine.
*/
void Bgm::setVolume (int volume)
{
	this->volume = volume;

	if (soundEngine->isMusicPlaying(this))
		soundEngine->setMusicVolume(volume);
}

void Bgm::fade (int volume, int time)
{
	soundEngine->fadeMusic(volume, time);
}