#pragma once

class Engine
{
	ALCdevice* m_alcDev;
	ALCcontext* m_alcContext;

	class Texture
	{
	public:
		string m_filename;
		GLuint m_glId;
		int m_id;

		unsigned int m_width;
		unsigned int m_height;
		bool m_hasAlpha;
		int m_framesX;
		int m_framesY;

		void* m_data;

		Texture(void* dib);

		Texture()
		{
		}

		Texture(const Texture& other)
		{
			m_filename = other.m_filename;
			m_glId = other.m_glId;
			m_id = other.m_id;
			m_hasAlpha = false;
			m_width = other.m_width;
			m_height = other.m_height;
			m_data = nullptr;
		}
	};

	int m_maxTextureId;
	std::map<int, Texture> m_textures;
	GLuint	m_uiVertexArrayObject;
	GLuint m_posBuff;
	GLuint m_coordBuff;
	GLuint m_indexBuff;
	GLuint m_uiSP;
	GLuint m_aPoint;
	GLuint m_aCoord;
	GLuint m_posbias;
	GLuint m_coordbias;
	GLuint m_map;

public:
	bool init();
	int loadTexture(const char* fileName, int framesX, int framesY);
	int loadSound(const char* fileName);
	bool renderRectangle(int textureId, int frame, float topX, float topY, float bottomX, float bottomY);
	bool playSound(int soundId, bool loop=false);
	void shutdown();
	static Engine& get();

	Engine();
};