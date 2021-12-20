#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "graphics/Window.h"
#include "graphics/ResourceManager.h"
#include "state/Controller.h"
#include "game/Unit.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
const int COLLISION_MAP_LEVELS = 4;

int main()
{
	ResourceManager manager = ResourceManager();
	Engine engine = Engine();
	Window mainWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game", manager);

	Game theGame = Game(SCREEN_WIDTH, SCREEN_HEIGHT, manager);
	Controller keyControl = Controller(theGame, InputType::INPUT_KEYBOARD, manager, engine, SCREEN_WIDTH, SCREEN_HEIGHT);

	mainWindow.beginRendering(keyControl);
}

