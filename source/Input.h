#pragma once

#include <vector>

#include "SDL2/SDL_keycode.h"
#include "SDL2/SDL_events.h"

enum class ButtonState : uint8_t
{
	Up,
	Pressed,
	Held,
	Released,
};

// TODO: ?action system for binds
class Input
{
public:
	static bool GetKey(SDL_Scancode scancode);

	static float GetMouseDeltaX();
	static float GetMouseDeltaY();

private:
	friend class Engine;

	static void Update();
	static const uint8_t* Keyboard;
};