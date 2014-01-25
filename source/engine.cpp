#include "ra_pch.h"
#include "engine.h"


static Engine* g_engine = nullptr;

bool Engine::init()
{
	m_alcDev = alcOpenDevice(NULL); // select the "preferred device"
	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		cout << "OpenAL: Unable to create Device." << endl;
		return false;
	}

	m_alcContext = alcCreateContext(m_alcDev, NULL);
	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		cout << "OpenAL: Unable to create Context." << endl;
		return false;
	}

	if (m_alcContext)
	{
		cout << "OpenAL: " << alcGetString(m_alcDev, ALC_DEVICE_SPECIFIER) << " Device" << endl;
		alcMakeContextCurrent(m_alcContext);
		error = alGetError();
		if (error != AL_NO_ERROR)
		{
			cout << "OpenAL: Unable to set Context." << endl;
			return false;
		}
	}
	else
	{
		alcCloseDevice(m_alcDev);
		m_alcDev = NULL;
		//return PSSG_SET_LAST_ERROR(false, ("OpenAL: Unable to create Context."));
		cout << "OpenAL: Unable to create Context." << endl;
	}


	//setDistanceModel(getDistanceModel());

	return true;
}

int Engine::loadTexture(const char* fileName)
{
	return -1;
}

int Engine::loadSound(const char* fileName)
{
	return -1;
}

bool Engine::renderRectangle(int textureId, float topX, float topY, float bottomX, float bottomY)
{
	return false;
}

bool Engine::playSound(int soundId, bool loop)
{
	return false;
}

void Engine::shutdown()
{

}

Engine& Engine::get()
{
	if (!g_engine)
	{
		g_engine = new Engine;
	}
	return *g_engine;
}