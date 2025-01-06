#include <AL/al.h>
#include <AL/alc.h>
#include <sndfile.h>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <stdexcept>

class Sound 
{
public:
    Sound();
    ~Sound();

    void LoadSound(const std::string& name, const std::string& filePath);
    void PlaySound(const std::string& name);
    void StopSound(const std::string& name);
    void Cleanup();

private:
    struct SoundData 
    {
        ALuint Buffer;
        ALuint Source;
    };

    std::map<std::string, SoundData> soundMap;

    ALenum GetOpenALFormat(int channels, int samples);
    void CheckOpenALError(const std::string& message);

    ALCdevice* m_device;
    ALCcontext* m_context;
};