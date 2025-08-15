#define SDL_MAIN_HANDLED
#include "Engine/Editor/Game.h"
#include "Childs/BowlingOpenGl.h"
#include "Childs/LVLDoom.h"
#include "Childs/LVLAdvancedOpenGl.h"
#include "Childs/LVLDoomMainMenu.h"
#include "rapidjson/document.h"

int main() {
	Game* pong = new Game("Game", new LVLDoomMainMenu());
	return 0;
}