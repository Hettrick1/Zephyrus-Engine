#define SDL_MAIN_HANDLED
#include "Engine/Editor/Game.h"
#include "Childs/LVLDoom.h"
#include "Childs/LVLDoomMainMenu.h"
#include "FactoryJSON/ComponentRegister.h"

int main() {
	ComponentRegister::RegisterAllComponents();
	Game* pong = new Game("Game", new LVLDoomMainMenu());
	return 0;
}