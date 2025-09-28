#include "Game.h"
#include "ComponentRegister.h"

int main(int argc, char** argv) {
	ComponentRegister::RegisterAllComponents();
	Game* pong = new Game("Game", "../Content/Maps/DoomMainMenu.zpmap");
	delete pong;
	return 0;
}