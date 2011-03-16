#pragma once
#ifndef __ONEGAME_H__
#define __ONEGAME_H__

#include "Game.h"
#include "OneLevel.h"

class OneGame : public Game
{
	private:
		bool on;


	public:

		OneGame() : Game(320*4, 144*4, 32, 320, 144, 3, 30)
		{
			srand(time(NULL));

			on = false;
			gameState = new OneLevel(this);
			gfxEngine->setScreenBackgroundColor(Color::Red);
		}

		void onStep()
		{
			if (getInput()->keyPressed(Input::kESC))
				setFinished(true);
		};
};

#endif