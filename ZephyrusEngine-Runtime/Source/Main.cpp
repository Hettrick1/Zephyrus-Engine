#define SDL_MAIN_HANDLED
#include "Game.h"
#include "Temp/ChildScenes/LVLDoom.h"
#include "Temp/ChildScenes/LVLDoomMainMenu.h"
#include "ComponentRegister.h"

int main() {
	ComponentRegister::RegisterAllComponents();
	Game* pong = new Game("Game", new LVLDoomMainMenu());
	return 0;
}