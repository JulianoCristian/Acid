#include <iostream>
#include <Inputs/Mouse.hpp>
#include <Renderer/Renderer.hpp>
#include <Scenes/Scenes.hpp>
#include "Scenes/FpsPlayer.hpp"
#include "Scenes/Scene1.hpp"
#include "MainRenderer.hpp"

using namespace test;
using namespace acid;

int main(int argc, char **argv)
{
	// Registers file search paths.
	Files::AddSearchPath("Resources/Engine");

	// Creates the engine.
	auto engine = std::make_unique<Engine>();

	// Registers modules.

	// Registers components.
	Scenes::Get()->RegisterComponent<FpsPlayer>("FpsPlayer");

	// Initializes modules.
	Display::Get()->SetTitle("Test PBR");
	Display::Get()->SetIcon("Logos/Flask.png");
	Mouse::Get()->SetCustomMouse("Guis/Cursor.png");
	Renderer::Get()->SetManager(new MainRenderer());
	Scenes::Get()->SetScene(new Scene1());

	// Runs the game loop.
	int32_t exitCode = engine->Run();

	// Pauses the console.
	std::cout << "Press enter to continue...";
	std::cin.get();
	return exitCode;
}