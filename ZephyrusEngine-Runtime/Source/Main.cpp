#include "SDL.h"
#include "Engine/Editor/Game.h"
#include "Childs/BowlingOpenGl.h"
#include "Childs/LVLDoom.h"
#include "Childs/LVLAdvancedOpenGL.h"
#include "Childs/LVLDoomMainMenu.h"

int main(int argc, char** argv) {
	Game* pong = new Game("Game", new LVLDoomMainMenu());
	return 0;
}