#include "ra_pch.h"
#include "engine.h"


#define MAX_INFO_LOG_SIZE 512
static Engine* g_engine = nullptr;

Engine::Texture::Texture(void* dib):
m_data(dib)
{
	FIBITMAP* fiData = (FIBITMAP*) dib;
	m_width = FreeImage_GetWidth(fiData);
	m_height = FreeImage_GetHeight(fiData);
}

bool Engine::init()
{
	//open al
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

	//shader
	GLint success;
	const GLchar *vsStringPtr[1];
	const GLchar *fsStringPtr[1];

	GLchar vsString[] =
		"#version 150\n"
		"in vec2 aPoint;\n"
		"in vec2 aCoord;\n"
		"uniform vec4 posbias;\n"
		"uniform vec4 coordbias;\n"
		"out vec2 oCoords;\n"
		"void main(void) {\n"
		"	gl_Position = vec4(aPoint.xy*posbias.xy + posbias.zw, 0.0, 1.0);\n"
		"	oCoords = aCoord.xy*coordbias.xy + coordbias.zw;\n"
		"}\n";

	GLchar fsString[] =
		"#version 150\n"
		"uniform sampler2D map;\n"
		"in vec2 oCoords;\n"
		"out vec4 vFragColor;\n"
		"void main(void) {\n"
		"	vFragColor = texture(map, oCoords);\n"
		"}\n";

	GLuint uiVS = glCreateShader(GL_VERTEX_SHADER);
	vsStringPtr[0] = vsString;
	glShaderSource(uiVS, 1, vsStringPtr, NULL);
	glCompileShader(uiVS);
	glGetShaderiv(uiVS, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[MAX_INFO_LOG_SIZE] = { 0 };
		glGetShaderInfoLog(uiVS, MAX_INFO_LOG_SIZE, NULL, infoLog);

		assert(0);
		return false;
	}

	GLuint uiFS = glCreateShader(GL_FRAGMENT_SHADER);
	fsStringPtr[0] = fsString;
	glShaderSource(uiFS, 1, fsStringPtr, NULL);
	glCompileShader(uiFS);
	glGetShaderiv(uiFS, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[MAX_INFO_LOG_SIZE] = { 0 };
		glGetShaderInfoLog(uiFS, MAX_INFO_LOG_SIZE, NULL, infoLog);

		assert(0);
		return false;
	}

	m_uiSP = glCreateProgram();
	glAttachShader(m_uiSP, uiVS);
	glAttachShader(m_uiSP, uiFS);
	glLinkProgram(m_uiSP);
	glGetProgramiv(m_uiSP, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[MAX_INFO_LOG_SIZE] = { 0 };
		glGetProgramInfoLog(m_uiSP, MAX_INFO_LOG_SIZE, NULL, infoLog);

		assert(0);
		return false;
	}

	assert(glIsProgram(m_uiSP));

	GLchar infoLog[MAX_INFO_LOG_SIZE];
	glGetProgramInfoLog(m_uiSP, MAX_INFO_LOG_SIZE, NULL, infoLog);

	m_aPoint = glGetAttribLocation(m_uiSP, "aPoint");
	assert(m_aPoint != -1);

	m_aCoord = glGetAttribLocation(m_uiSP, "aCoord");
	assert(m_aCoord != -1);

	m_posbias = glGetUniformLocation(m_uiSP, "posbias");
	m_coordbias = glGetUniformLocation(m_uiSP, "coordbias");
	//--------------------------------------------


	//init buffers
	glGenBuffers(1, &m_posBuff);
	glGenBuffers(1, &m_coordBuff);
	glGenBuffers(1, &m_indexBuff);

	glGenVertexArrays(1, &m_uiVertexArrayObject);
	glBindVertexArray(m_uiVertexArrayObject);

	float points[] = {
		-1, 1,
		1, 1,
		1, -1,
		-1, -1,
	};

	glEnableVertexAttribArray(m_aPoint);
	glBindBuffer(GL_ARRAY_BUFFER, m_posBuff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, points, GL_STATIC_DRAW);
	glVertexAttribPointer(m_aPoint, 2, GL_FLOAT, GL_FALSE, 0, 0);

	float coords[] = {
		0, 1,
		1, 1,
		1, 0,
		0, 0,
	};

	glEnableVertexAttribArray(m_aCoord);
	glBindBuffer(GL_ARRAY_BUFFER, m_coordBuff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, coords, GL_STATIC_DRAW);
	glVertexAttribPointer(m_aCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//int indices [] = { 0, 1, 2, 0, 2, 3 };
	int indices[] = { 0, 1, 2, 3 };

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuff);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 4, indices, GL_STATIC_DRAW);

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glUseProgram(m_uiSP);

	return true;
}

int Engine::loadTexture(const char* fileName, int framesX, int framesY)
{
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName);
	if (format == FIF_UNKNOWN)
	{
		return false;
	}

	FIBITMAP* originalData = FreeImage_Load(format, fileName);
	if (originalData == nullptr)
	{
		return false;
	}

	bool hasAlpha = false;
	FREE_IMAGE_COLOR_TYPE clrType = FreeImage_GetColorType(originalData);
	if (clrType == FIC_RGBALPHA)
	{
		hasAlpha = true;
	}

	FIBITMAP* data = FreeImage_ConvertTo32Bits(originalData);
	FreeImage_Unload(originalData);
	if (data == nullptr)
	{
		return false;
	}

	Texture texture(data);
	glGenTextures(1, &texture.m_glId);

	texture.m_framesX = framesX;
	texture.m_framesY = framesY;
	
	glBindTexture(GL_TEXTURE_2D, texture.m_glId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture.m_width, texture.m_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, FreeImage_GetBits(data));
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	FreeImage_Unload((FIBITMAP*) data);
	texture.m_data = nullptr;

	texture.m_id = m_maxTextureId++;
	texture.m_hasAlpha = hasAlpha;
	m_textures[texture.m_id] = texture;

	cout << "Loaded texture: " << fileName << texture.m_width << "x" << texture.m_height << endl;

	return texture.m_id;
}

int Engine::loadSound(const char* fileName)
{
	return -1;
}

bool Engine::renderRectangle(int textureId, int frame, float topX, float topY, float bottomX, float bottomY)
{
	auto it = m_textures.find(textureId);
	if (it == m_textures.end())
	{
		return false;
	}

	Texture& texture = it->second;

	float posbias [] = {
		(bottomX - topX) / 2.0f,
		(topY - bottomY) / 2.0f,
		topX + (bottomX - topX) / 2.0f,
		bottomY + (topY - bottomY) / 2.0f,
	};

	float coordbias [] = {
		1,
		1,
		0,
		0,
	};

	glUniform4fv(m_posbias, 1, posbias);
	glUniform4fv(m_coordbias, 1, coordbias);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.m_glId);
	glUniform1i(m_map, 0);

	if (texture.m_hasAlpha)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	if (texture.m_hasAlpha)
	{
		glDisable(GL_BLEND);
	}

	return true;
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


Engine::Engine():
m_maxTextureId(1)
{
	
}