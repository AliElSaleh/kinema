#include "Input.h"

#include "SDL2/SDL.h"

const uint8_t* Input::Keyboard = nullptr;

bool Input::GetKey(SDL_Scancode scancode)
{
	return Keyboard[scancode];
}

float Input::GetMouseDeltaX()
{
	return -1.0f;
}
float Input::GetMouseDeltaY()
{
	return -1.0f;
}

void Input::Update()
{
	Keyboard = SDL_GetKeyboardState(nullptr);
}

