#include "ra_pch.h"

#include "settings.h"
#include "engine.h"

#include "board.h"

Settings g_Settings;

void SpecialFunc(int key, int x, int y)
{
	// Refresh the Window
	glutPostRedisplay();
}


void ReshapeFunc(int w, int h)
{
	
}


void MotionFunc(int x, int y)
{
	
}

void PassiveMotionFunc(int x, int y)
{

}

void KeyboardFunc(unsigned char key, int x, int y)
{

}

void MouseFunc(int b, int s, int x, int y)
{
	
}

//int testTextId = -1;
void DisplayFunc(void)
{
	glClearColor(1.0f, .0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//Engine::get().renderRectangle(testTextId, 0, 0, 1, 1, 0);
	Board::get().render();
	glutSwapBuffers();
}

void CloseFunc()
{
	Engine::get().shutdown();
}

int main(int argc, char* argv[])
{	
	char exePath[512] = { 0 };
	GetModuleFileNameA(NULL, exePath, 512);

	char dev[512] = { 0 };
	char dir[512] = { 0 };
	char file[512] = { 0 };
	char ext[512] = { 0 };
	_splitpath_s(exePath, dev, dir, file, ext);

	char exeDir[512] = { 0 };
	strcpy_s(exeDir, dev);
	strcat_s(exeDir, dir);
	SetCurrentDirectoryA(exeDir);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
#ifdef _DEBUG
    	glutInitContextFlags(GLUT_DEBUG);
#endif
	glutInit(&argc, argv);

	if(g_Settings.GetMultisample() > 1)
	{
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	}
	else
	{
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	}

	glutInitWindowSize(g_Settings.GetWindowWidth(), g_Settings.GetWindowHeight());
	glutCreateWindow("RA");

	glutReshapeFunc(ReshapeFunc);
	glutSpecialFunc(SpecialFunc);
	glutDisplayFunc(DisplayFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutMouseFunc(MouseFunc);
	glutMotionFunc(MotionFunc);
	glutPassiveMotionFunc(PassiveMotionFunc);
	glutCloseFunc(CloseFunc);

	if(gl3wInit())
	{
		std::cout << "ERROR: Failed to initializa OpenGL" << std::endl;
		return -1;
	}

	if(!gl3wIsSupported(3, 3))
	{
		std::cout << "ERROR: OpenGL 3.3 not supported" << std::endl;
		return -1;
	}

	//GLint numVtxUniformBlocks = 0;
	//GLint numFragUniformBlocks = 0;
	//GLint maxUniformBlockSize = 0;
	//glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &numVtxUniformBlocks);
	//glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &numFragUniformBlocks);
	//glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBlockSize);

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	if (!Engine::get().init())
	{
		return 0;
	}


	//testTextId = Engine::get().loadTexture("../content/Background.tga", 1, 1);
	Board::get().init();

	//g_cameraController.OnScreenReshape(g_Settings.GetWindowWidth(), g_Settings.GetWindowHeight());
	/*if(!LoadRC())
	{
		ClearRC();
		return -1;
	}*/

	cout << "entering main loop..." << endl;

    srand((unsigned)time(NULL));

	glutMainLoop();
	return 0;
}