#include "Sound.h"


Sound::Sound()
{
    // Initialize OpenAL
    m_device = alcOpenDevice(nullptr);
    if (!m_device) 
    {
        throw std::runtime_error("Failed to open OpenAL device");
    }

    m_context = alcCreateContext(m_device, nullptr);
    if (!m_context || !alcMakeContextCurrent(m_context)) 
    {
        throw std::runtime_error("Failed to create or set OpenAL context");
    }
}

Sound::~Sound()
{
    Cleanup();
    if (m_context)
    {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(m_context);
    }
    if (m_device)
    {
        alcCloseDevice(m_device);
    }
}

void Sound::LoadSound(const std::string& name, const std::string& filePath)
{
    // Open audio file with libsndfile
    SF_INFO sfInfo;
    SNDFILE* sndFile = sf_open(filePath.c_str(), SFM_READ, &sfInfo);
    if (!sndFile) 
    {
        throw std::runtime_error("Failed to load sound file: " + filePath);
    }

    // Determine OpenAL format
    ALenum format = GetOpenALFormat(sfInfo.channels, sfInfo.samplerate);
    if (format == AL_NONE)
    {
        sf_close(sndFile);
        throw std::runtime_error("Unsupported audio format in file: " + filePath);
    }

    // Read audio data
    std::vector<short> samples(sfInfo.frames * sfInfo.channels);
    sf_count_t numFrames = sf_readf_short(sndFile, samples.data(), sfInfo.frames);
    sf_close(sndFile);

    if (numFrames < 1)
    {
        throw std::runtime_error("Failed to read audio data from file: " + filePath);
    }

    // Create OpenAL buffer and source
    ALuint buffer;
    ALuint source;
    alGenBuffers(1, &buffer);
    CheckOpenALError("Failed to generate OpenAL buffer");
    alBufferData(buffer, format, samples.data(), samples.size() * sizeof(short), sfInfo.samplerate);
    CheckOpenALError("Failed to buffer audio data");

    alGenSources(1, &source);
    CheckOpenALError("Failed to generate OpenAL source");
    alSourcei(source, AL_BUFFER, buffer);

    // Store in map
    soundMap[name] = { buffer, source };
}

void Sound::PlaySound(const std::string& name)
{
    auto it = soundMap.find(name);
    if (it == soundMap.end()) 
    {
        throw std::runtime_error("Sound not found: " + name);
    }
    alSourcePlay(it->second.Source);
    CheckOpenALError("Failed to play sound");
}

void Sound::StopSound(const std::string& name)
{
    auto it = soundMap.find(name);
    if (it == soundMap.end())
    {
        throw std::runtime_error("Sound not found: " + name);
    }
    alSourceStop(it->second.Source);
    CheckOpenALError("Failed to stop sound");
}

void Sound::Cleanup()
{
    for (auto& pair : soundMap) 
    {
        alDeleteSources(1, &pair.second.Source);
        alDeleteBuffers(1, &pair.second.Buffer);
    }
    soundMap.clear();
}

ALenum Sound::GetOpenALFormat(int channels, int samples)
{
    if (channels == 1) 
    {
        return AL_FORMAT_MONO16;
    }
    else if (channels == 2) 
    {
        return AL_FORMAT_STEREO16;
    }
    return AL_NONE;
}

void Sound::CheckOpenALError(const std::string& message)
{
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) 
    {
        throw std::runtime_error(message + " (OpenAL error: " + std::to_string(error) + ")");
    }
}