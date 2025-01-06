#include "Keyboard.h"

Keyboard::Keyboard()
{
    // Initialize the key map
    int numKeys;
    const bool* state = SDL_GetKeyboardState(&numKeys);
    for (int i = 0; i < numKeys; ++i) 
    {
        m_keyStates[SDL_GetScancodeName(static_cast<SDL_Scancode>(i))] = state[i];
    }
}

void Keyboard::Update()
{
    // Update the current key states
    const bool* state = SDL_GetKeyboardState(nullptr);
    for (auto& keyState : m_keyStates)
    {
        SDL_Scancode scancode = SDL_GetScancodeFromName(keyState.first.c_str());

        if (scancode != SDL_SCANCODE_UNKNOWN) 
        {
            keyState.second = state[scancode];
        }
    }
}

bool Keyboard::KeyPressed(const std::string& keyName)
{
    SDL_Scancode scancode = SDL_GetScancodeFromName(keyName.c_str());
    if (scancode == SDL_SCANCODE_UNKNOWN) 
    {
        std::cerr << "Invalid key name: " << keyName << std::endl;
        return false;
    }
    return m_keyStates[keyName] == 1;
}
