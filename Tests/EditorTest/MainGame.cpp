#include "MainGame.hpp"

#include <Files/Files.hpp>
#include <Files/FileSystem.hpp>
#include <Devices/Mouse.hpp>
#include <Graphics/Graphics.hpp>
#include <Scenes/Scenes.hpp>
#include <Uis/Uis.hpp>
#include "MainRenderer.hpp"
#include "Scenes/PlayerFps.hpp"
#include "Scenes/Scene1.hpp"
#include "Resources/Resources.hpp"

#if defined(ACID_RELOAD)

#include <Engine/cr.h>

CR_EXPORT int cr_main(struct cr_plugin *ctx, enum cr_op operation)
{
	switch (operation)
	{
	case CR_LOAD:
		Log::Out("[Guest] Operation load: %i\n", ctx->version);
		Engine::Get()->SetGame(new test::MainGame());
		return 0;
	case CR_UNLOAD:
		Log::Out("[Guest] Operation unload: %i\n", ctx->version);
		Engine::Get()->SetGame(nullptr);
		return 0;
	default:
		return 0;
	}
}
#else
int main(int argc, char **argv)
{
	using namespace test;

	// Creates the engine.
	auto engine = std::make_unique<Engine>(argv[0]);
	engine->SetGame(new MainGame());

	// Runs the game loop.
	int32_t exitCode = engine->Run();

	// Pauses the console.
	std::cout << "Press enter to continue...";
	std::cin.get();
	return exitCode;
}
#endif

namespace test
{
MainGame::MainGame() :
	m_buttonFullscreen(Key::F11),
	m_buttonScreenshot(Key::F9),
	m_buttonExit(Key::Delete)
{
	Log::Out("[Game] Constructor\n");

	// Registers file search paths.
	Files::Get()->AddSearchPath("Resources/Engine");
	Log::Out("Working Directory: %s\n", FileSystem::GetWorkingDirectory().c_str());

	m_buttonFullscreen.OnButton().Add([this](InputAction action, BitMask<InputMod> mods)
	{
		if (action == InputAction::Press)
		{
			Window::Get()->SetFullscreen(!Window::Get()->IsFullscreen());
		}
	});
	m_buttonScreenshot.OnButton().Add([this](InputAction action, BitMask<InputMod> mods)
	{
		if (action == InputAction::Press)
		{
			Resources::Get()->GetThreadPool().Enqueue([]()
			{
				Graphics::Get()->CaptureScreenshot("Screenshots/" + Engine::GetDateTime() + ".png");
			});
		}
	});
	m_buttonExit.OnButton().Add([this](InputAction action, BitMask<InputMod> mods)
	{
		if (action == InputAction::Press)
		{
			Engine::Get()->RequestClose(false);
		}
	});

	// Registers modules.
	//Engine::Get()->RemoveModule<Shadows>();

	// Registers components.
	auto &componentRegister = Scenes::Get()->GetComponentRegister();
	componentRegister.Add<PlayerFps>("PlayerFps");

	// Sets values to modules.
	//Window::Get()->SetTitle("Test Physics");
	//Window::Get()->SetIcons({ "Icons/Icon-16.png", "Icons/Icon-24.png", "Icons/Icon-32.png", "Icons/Icon-48.png", "Icons/Icon-64.png",
	//	"Icons/Icon-96.png", "Icons/Icon-128.png", "Icons/Icon-192.png", "Icons/Icon-256.png" });
	//Mouse::Get()->SetCursor("Guis/Cursor.png", CursorHotspot::UpperLeft);
	//Renderer::Get()->SetManager(new MainRenderer());
	Scenes::Get()->SetScene(new Scene1());
}

MainGame::~MainGame()
{
	Log::Out("[Game] Destructor\n");
	//Files::Get()->ClearSearchPath();

	// Deregisters components.
	auto &componentRegister = Scenes::Get()->GetComponentRegister();
	componentRegister.Remove("PlayerFps");

	//Renderer::Get()->SetManager(nullptr);
	Scenes::Get()->SetScene(nullptr);
	Uis::Get()->GetContainer().ClearChildren();
}

void MainGame::Update()
{
}
}
