#include "EditorApplication/EditorApplication.h"
#include "Temp/ChildScenes/LVLDoom.h"
#include "Temp/ChildScenes/LVLDoomMainMenu.h"
#include "ComponentRegister.h"

int main(int argc, char** argv) {
	ComponentRegister::RegisterAllComponents();
	EditorApplication* editor = new EditorApplication("ZephyrusEditor", new LVLDoom());
	return 0;
}