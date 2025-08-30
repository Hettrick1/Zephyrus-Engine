#define SDL_MAIN_HANDLED
#include "EditorApplication.h"
#include "Temp/ChildScenes/LVLDoom.h"
#include "Temp/ChildScenes/LVLDoomMainMenu.h"
#include "ComponentRegister.h"

int main() {
	ComponentRegister::RegisterAllComponents();
	EditorApplication* editor = new EditorApplication("ZephyrusEditor", new LVLDoom());
	return 0;
}