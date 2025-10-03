#include "Game.h"
#include "ComponentRegister.h"
#include <memory>

int main(int argc, char** argv) {
	Zephyrus::Factory::ComponentRegister::RegisterAllComponents();
	std::unique_ptr<Zephyrus::Application::Game> pong = std::make_unique<Zephyrus::Application::Game>("Game", "../Content/Maps/DoomMainMenu.zpmap");
	return 0;
}