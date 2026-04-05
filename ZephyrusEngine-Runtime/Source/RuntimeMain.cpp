#include "GameApplication.h"
#include <memory>

int main(int argc, char** argv) {
	std::unique_ptr<Zephyrus::Application::GameApplication> pong = std::make_unique<Zephyrus::Application::GameApplication>("Game", "../Config/Game.config");
	return 0;
}
