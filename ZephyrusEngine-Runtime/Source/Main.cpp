#include "SDL.h"
#include "Engine/Editor/Game.h"
#include "Childs/BowlingOpenGl.h"
#include "Childs/LVLDoom.h"
#include "Childs/LVLAdvancedOpenGl.h"
#include "Childs/LVLDoomMainMenu.h"

int main(int argc, char** argv) {
	Game* pong = new Game("Game", new LVLAdvancedOpenGl());
	return 0;
}