#include "Game.h"


int main() {
	Game game("JANELINHA", 640, 480, 4, 4, true);

	//MAIN LOOP
	while (!game.getWindowShouldClose()) {
		//UPDATE INPUT
		game.update();
		game.render();
	}

	return 0;
}
