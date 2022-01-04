#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "graphics/Window.h"
#include "graphics/ResourceManager.h"
#include "state/Controller.h"
#include "scripts/Script.h"
#include "scripts/ScriptInitializer.h"

// TODO: application profiling

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
const int PLAYABLE_WIDTH = 760;
const int PLAYABLE_HEIGHT = 600;
const int SCREEN_OFFSET_X = 20;
const int SCREEN_OFFSET_Y = 20;

const std::string TEXTURE_BASE_PATH = "./graphics/textures/";
const std::string TEXTURE_INFO_FILE = "textures.info";
const std::string UI_FONT_NAME = "arial";
const int DEFAULT_FONT_RENDER_SIZE = 48;

int main()
{
	ResourceManager manager = ResourceManager();
	Engine engine = Engine();
	Window mainWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game", manager);

	manager.initialize(TEXTURE_INFO_FILE, TEXTURE_BASE_PATH);

	Game theGame = Game(PLAYABLE_WIDTH, PLAYABLE_HEIGHT, SCREEN_OFFSET_X, SCREEN_OFFSET_Y, manager);

	ScriptExecutor executor;
	ScriptCollection scripts = ScriptInitializer::createTestScripts(theGame);
	std::cout << "Script loaded: " << scripts.getCollectionName() << std::endl;

	Controller keyControl = Controller(theGame, InputType::INPUT_KEYBOARD, scripts, executor, engine, SCREEN_WIDTH, SCREEN_HEIGHT);

	mainWindow.initGame(PLAYABLE_WIDTH, PLAYABLE_HEIGHT, UI_FONT_NAME, DEFAULT_FONT_RENDER_SIZE);

	mainWindow.beginRendering(keyControl, PLAYABLE_WIDTH, PLAYABLE_HEIGHT, SCREEN_OFFSET_X, SCREEN_OFFSET_Y);
}

