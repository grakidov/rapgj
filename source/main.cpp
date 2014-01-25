#include "ra_pch.h"
#include "settings.h"

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

void DisplayFunc(void)
{
	
}

void CloseFunc()
{

}

int main(int argc, char* argv[])
{	

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

	std::cout << "OpenGL version = " << glGetString(GL_VERSION) << std::endl;

	//g_cameraController.OnScreenReshape(g_Settings.GetWindowWidth(), g_Settings.GetWindowHeight());

	/*if(!LoadRC())
	{
		ClearRC();
		return -1;
	}*/

	cout << "entering main loop..." << endl;
	glutMainLoop();
	return 0;
}