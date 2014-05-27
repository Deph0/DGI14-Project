#include "sound.h"
#include "exception.h"

Sound::BufMap Sound::buffers;


Sound::Sound()
{
	alGenSources(1, &id);
	// Set default values
	setPitch(1.f);
	setGain(1.f);
}


Sound::~Sound()
{
	alDeleteSources(1, &id);
}


void Sound::initialize()
{
	if (!alutInit(0, NULL))
		throw Exception(alutGetErrorString(alutGetError()));
}


void Sound::shutdown()
{
	BufMap::iterator i = buffers.begin();
	for (; i != buffers.end(); ++i) {
		alDeleteBuffers(1, &i->second);
	}
	buffers.clear();
	alutExit();
}


void Sound::load(const std::string& fname)
{
	ALuint bufId;

	// See if file is already loaded
	BufMap::iterator i = buffers.find(fname);
	if (i != buffers.end()) {
		// Already loaded just attach the source
		bufId = i->second;
	}
	else {
		// Not loaded yet, do it now
		printf("loading %s\n", fname.c_str());
		bufId = alutCreateBufferFromFile(fname.c_str());
		if (bufId == AL_NONE)
			throw Exception(alutGetErrorString(alutGetError()));
		buffers[fname] = bufId;
	}
	// Attach
	alSourcei(id, AL_BUFFER, bufId);
}


void Sound::setPosition(const glm::vec3& p)
{
	alSourcefv(id, AL_POSITION, &p.x);
}


void Sound::setVelocity(const glm::vec3& v)
{
	alSourcefv(id, AL_VELOCITY, &v.x);
}


void Sound::setPitch(float p)
{
	alSourcef(id, AL_PITCH, p);
}


void Sound::setGain(float g)
{
	alSourcef(id, AL_GAIN, g);
}


void Sound::enableLoop(bool en)
{
	alSourcei(id, AL_LOOPING, en ? AL_TRUE : AL_FALSE);
}

void Sound::setRefDistance(float dist)
{
	alSourcef(id, AL_REFERENCE_DISTANCE, dist);
}


void Sound::setMaxDistance(float dist)
{
	alSourcef(id, AL_MAX_DISTANCE, dist);
}


void Sound::play()
{
	alSourcePlay(id);
}


void Sound::stop()
{
	alSourceStop(id);
}


void Sound::pause()
{
	alSourcePause(id);
}


void Sound::rewind()
{
	alSourceRewind(id);
}

