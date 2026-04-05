#define IMGUI_DEFINE_MATH_OPERATORS
#include "EditorApplication/EditorApplication.h"

int main(int argc, char** argv) {
	 std::unique_ptr<EditorApplication> editor = std::make_unique<EditorApplication>("ZephyrusEditor", "../Config/Editor.config");
	return 0;
}