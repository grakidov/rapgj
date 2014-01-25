#pragma once

class Engine
{
	ALCdevice* m_alcDev;
	ALCcontext* m_alcContext;
public:
	bool init();
	int loadTexture(const char* fileName);
	int loadSound(const char* fileName);
	bool renderRectangle(int textureId, float topX, float topY, float bottomX, float bottomY);
	bool playSound(int soundId, bool loop=false);
	void shutdown();
	static Engine& get();
};