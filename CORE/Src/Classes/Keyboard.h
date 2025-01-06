#pragma once
#include <SDL3/SDL.h>

#include <iostream>
#include <unordered_map>
#include <string>

class Keyboard 
{
public:

    Keyboard();
  

    void Update();
    
    bool KeyPressed(const std::string& keyName);
    

private:
    std::unordered_map<std::string, Uint8> m_keyStates;
};

