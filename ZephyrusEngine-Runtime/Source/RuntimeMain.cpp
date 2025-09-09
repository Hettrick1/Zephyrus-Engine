#include "Game.h"
#include "Temp/ChildScenes/LVLDoom.h"
#include "Temp/ChildScenes/LVLDoomMainMenu.h"
#include "ComponentRegister.h"

int main(int argc, char** argv) {
	ComponentRegister::RegisterAllComponents();
	Game* pong = new Game("Game", "../Content/Maps/DoomMainMenu.ZPMap");
	return 0;
}