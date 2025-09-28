#include "EditorApplication/EditorApplication.h"
#include "ComponentRegister.h"

int main(int argc, char** argv) {
	ComponentRegister::RegisterAllComponents();
	EditorApplication* editor = new EditorApplication("ZephyrusEditor", "../Content/Maps/LevelDoom.zpmap");
	delete editor;
	return 0;
}