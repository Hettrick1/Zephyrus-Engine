#include "Game.h"
#include <memory>

int main(int argc, char** argv) {
	std::unique_ptr<Zephyrus::Application::Game> pong = std::make_unique<Zephyrus::Application::Game>("Game", "../Content/Maps/LevelDoom.zpmap");
	return 0;
}
