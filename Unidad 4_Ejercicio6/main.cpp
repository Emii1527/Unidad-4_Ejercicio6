#include <iostream>
#include <vector>
#include <ctime>

#include "Juego.h"



int main()
{
	//inicializar el Random
	std::srand(static_cast<unsigned>(time(NULL)));

	Juego game;

	//Game loop
	while (game.running() && !game.getEndGame()) {


		//Upgrade
		game.update();

		//Render
		game.render();

	}
	return 0;
}
