#ifndef SOUND_H
#define SOUND_H

#include <list>
#include <vector>
#include <map>
#include <string>
#include <glm/glm.hpp>
#include "opengl.h"


class Sound {
public:
	typedef std::list<Sound*> List;
	typedef std::vector<Sound*> Vector;

	// Initialize ALUT library
	static void initialize();
	// Shutdown the ALUT library
	static void shutdown();

	// Load a sound file (if not loaded already)
	// and attach to this sound source
	void load(const std::string& fname);

	void setPosition(const glm::vec3& p);
	void setVelocity(const glm::vec3& p);
	void setPitch(float p);
	void setGain(float g);
	void enableLoop(bool en = true);

	// The distance that the source will be the
	// loudest (if the listener is closer, it
	// won't be any louder than if they were at
	// this distance)
	void setRefDistance(float dist);

	// The distance that the source will be the
	// quietest (if the listener is farther, it
	// won't be any quieter than if they were at
	// this distance)
	void setMaxDistance(float dist);


	void play();
	void stop();
	void pause();
	void rewind();

	// Constructor
	Sound();
	// Destructor
	~Sound();

private:
	// Filename <-> buffer id map
	typedef std::map<std::string, ALuint> BufMap;

	ALuint id;
	static BufMap buffers;
};

#endif // SOUND_H

