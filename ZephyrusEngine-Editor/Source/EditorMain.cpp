#include "EditorApplication/EditorApplication.h"
#include "ComponentRegister.h"

int main(int argc, char** argv) {
	Zephyrus::Factory::ComponentRegister::RegisterAllComponents();
	std::unique_ptr<EditorApplication> editor = std::make_unique<EditorApplication>("ZephyrusEditor", "../Content/Maps/LevelDoom.zpmap");
	return 0;
}