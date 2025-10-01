#include "Game.h"
#include "ComponentRegister.h"
#include <memory>

int main(int argc, char** argv) {
	ComponentRegister::RegisterAllComponents();
	std::unique_ptr<Game> pong = std::make_unique<Game>("Game", "../Content/Maps/DoomMainMenu.zpmap");
	return 0;
}