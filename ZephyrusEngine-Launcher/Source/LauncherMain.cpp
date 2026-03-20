#include <memory>

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#include <filesystem>
#endif

int main(int argc, char** argv)
{
	
#ifdef _WIN32
	std::filesystem::path path = "ZephyrusEngine-Editor/ZephyrusEngine-Editor.exe";
	ShellExecuteA(nullptr, "open", path.make_preferred().string().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
#endif
	
	return 0;
}
