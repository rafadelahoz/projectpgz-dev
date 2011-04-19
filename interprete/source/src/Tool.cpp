#include "Tool.h"

// Inicializaci�n por defecto
Tool::Tool(int x, int y, Game* game, GameState* world) : GameEntity(x, y, game, world)
{
	passive = true;
	player = NULL;
}

Tool::~Tool()
{
}

void Tool::init(bool passive, Player* p)
{
	this->passive = passive;
	player = p;
}

bool Tool::isPassive()
{
	return passive;
}

std::string Tool::getConfigurationFileName(string fname)
{
	// El fname debe tener extensi�n
	if (fname.length() < 5)
		return "";

	std::string cfgname = fname;
	// Se quita la extensi�n del fname (.png)
	for (int i = 0; i < 3; i++)
		cfgname.pop_back();
	cfgname.append("cfg");

	return cfgname;
}

bool Tool::loadAnimation(Direction dir, std::string name, FILE* from)
{
	// Cargar animaci�n indicada de from
	if (from == NULL || name == "")
		return false;

	int numFrames, speed;
	int* frameList;
	ToolAnimData animData;
	FrameData frameData;

	// Leemos datos de la animaci�n

	// 0.Speed
	if (fscanf(from, "%d", &speed) < 1)
		return false;

	// 1.N� frames
	if (fscanf(from, "%d", &numFrames) < 1)
		return false;

	animData.animSpeed = speed;
	animData.numFrames = numFrames;
	animData.dir = dir;

	// Se instancia el contenedor de frames
	frameList = new int[numFrames];

	// Se carga cada frame
	for (int i = 0; i < numFrames; i++)
	{
		frameData = loadAnimationFrame(from);
		frameList[i] = frameData.frameId;
		animData.frameData.push_back(frameData);
	}

	// Se a�ade la animaci�n al graphic con el nombre indicado
	SpriteMap* gfx = ((SpriteMap*) graphic);
	gfx->addAnim(name, frameList, numFrames, speed, true);

	// Se mappea la anim con la direcci�n
	// Y se agrega su info
	animList.insert(make_pair(name, animData));
	delete frameList;

	return true;
}

Tool::FrameData Tool::loadAnimationFrame(FILE* from)
{
	FrameData fd;

	// De momento rellenamos con valores vac�os el resto de campos del framdata
	fd.hotspotX = 0;
	fd.hotspotY = 0;
	fd.offsetX = 0;
	fd.offsetY = 0;
	fd.height = 0;
	fd.width = 0;

	// El archivo debe ser v�lido
	if (from == NULL)
		return fd;

	// Se lee el frameId
	if (fscanf(from, "%d", &fd.frameId) < 1)
		return fd;

	// Se lee el hotspot
	if (fscanf(from, "%d %d", &fd.hotspotX, &fd.hotspotY) < 1)
		return fd;

	// Se lee la m�scara
	if (fscanf(from, "%d %d %d %d", &fd.offsetX, &fd.offsetY, &fd.width, &fd.height) < 1)
		return fd;

	// Y por ahora ya
	return fd;
}

bool Tool::playAnim(std::string name)
{
	// Si la animaci�n no tiene datos, algo va mal
	ToolAnimData data = animList.at(name);
	if (data.numFrames < 0)
		return false;

	// Establecer nueva animaci�n
	if(graphic != NULL) ((SpriteMap*) graphic)->playAnim(name, data.animSpeed, false, false);

	return true;
}

bool Tool::animFinished()
{
	if(graphic != NULL)
		return ((SpriteMap*) graphic)->animFinished();
	else
		return true;
}

void Tool::placeTool()
{
	// Comprobamos qu� animaci�n es la que se est� ejecutando
	std::string name;
	if(graphic != NULL)
		name = ((SpriteMap*) graphic)->getCurrentAnim();
	else
		return; // si no existe gr�fico no se puede colocar

	// Actualizamos la posici�n de la entidad si hay alguna animaci�n en curso
	if (name != "none")
	{
		// hotspot actual del player
		pair<int, int> hotPlayer;
		hotPlayer = player->getCurrentHotSpot();

		ToolAnimData animData = animList.at(name); // cogemos la informaci�n de la animaci�n actual
		int frame = ((SpriteMap*) graphic)->getCurrentFrame(); // cogemos el frame actual
		FrameData fd = animData.frameData[frame];

		// Actualizamos la posici�n en funci�n del hotspot del player y del hotspot del frame actual de la espada
		x = player->x + hotPlayer.first - fd.hotspotX;
		y = player->y + hotPlayer.second - fd.hotspotY;

		// Actualizamos la m�scara
		if (mask != NULL) delete mask; // borramos la antigua
		mask = new MaskBox(x, y, fd.width, fd.height, "tool", fd.offsetX, fd.offsetY); // creamos la nueva en la posici�n actual

		// Actualizamos la profundidad del gr�fico
		depth = y;
	}
}