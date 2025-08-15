#define SDL_MAIN_HANDLED
#include "Engine/Editor/Game.h"
#include "Childs/BowlingOpenGl.h"
#include "Childs/LVLDoom.h"
#include "Childs/LVLAdvancedOpenGl.h"
#include "Childs/LVLDoomMainMenu.h"
#include "rapidjson/document.h"

int main() {
    const char* json = R"({
        "name": "TestActor"
    })";

    rapidjson::Document doc;
    doc.Parse(json);

    if (doc.HasParseError()) {
        std::cout << "Erreur de parsing JSON\n";
        return 1;
    }

    std::cout << "Actor name: " << doc["name"].GetString() << "\n";
	Game* pong = new Game("Game", new LVLDoomMainMenu());
	return 0;
}