#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	movx = 2.0f;
	movy = 2.0f;
	movz = 2.0f;
	rot = 0.0f;
	posLightx = 0.0f;
	posLighty = 0.0f;
	dirLightx = 0.0f;
	dirLighty = 0.0f;
	posLightFx = 0.0f;
	posLightFz = 0.0f;
	dirLightFx = 0.0f;
	dirLightFz = 0.0f;
	circuitoTanq = false;
	RegaloAnim = false;
	magia = false;
	Proyectil = false;
	vistaAerea = false;
	vistaCuarto = false;
	vistaSala = false;
	Vista = true;
	espectaculoLuces = false;
	movPuertaF = false;
	recorrido = false;
	rotx = 0.0f;
	roty = 0.0f;
	rotz = 0.0f;
	saludo = false;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicializaci�n de GLFW
	if (!glfwInit())
	{
		printf("Fall� inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Primer ventana", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tama�o de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fall� inicializaci�n de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se est� usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}




void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_H)
	{
		theWindow-> circuitoTanq = true;
		theWindow->RegaloAnim = false;
		theWindow->magia = false;
	}
	if (key == GLFW_KEY_J)
	{
		theWindow->RegaloAnim = true;
		theWindow->circuitoTanq = false;
		theWindow->magia = false;
	}
	if (key == GLFW_KEY_K)
	{
		theWindow->magia = true;
		theWindow->circuitoTanq = false;
		theWindow->RegaloAnim = false;
	}

	if (key == GLFW_KEY_L)
	{
		theWindow->magia = true;
	}
	//Vistas
	if (key == GLFW_KEY_Y)
	{
		theWindow->Vista = true;
		theWindow->vistaCuarto = false;
		theWindow->vistaAerea = false;
		theWindow->vistaSala = false;
	}
	if (key == GLFW_KEY_U)
	{
		theWindow->vistaAerea = true;
		theWindow->vistaCuarto = false;
		theWindow->Vista = false;
		theWindow->vistaSala = false;
	}
	if (key == GLFW_KEY_I)
	{
		theWindow->vistaCuarto = true;
		theWindow->vistaAerea = false;
		theWindow->Vista = false;
		theWindow->vistaSala = false;
	}
	if (key == GLFW_KEY_O)
	{
		theWindow->vistaSala = true;
		theWindow->vistaCuarto = false;
		theWindow->vistaAerea = false;
		theWindow->Vista = false;
	}
	if (key == GLFW_KEY_N)
	{
		theWindow->espectaculoLuces = true;
	}
	if (key == GLFW_KEY_M)
	{
		theWindow->espectaculoLuces = false;
	}
	if (key == GLFW_KEY_Z)
	{
		theWindow->movPuertaF = true;
	}
	if (key == GLFW_KEY_X)
	{
		theWindow->recorrido = true;
	}
	if (key == GLFW_KEY_1)
	{
		theWindow->rotx += 10.0f;
	}
	if (key == GLFW_KEY_2)
	{
		theWindow->roty += 10.0f;
	}
	if (key == GLFW_KEY_3)
	{
		theWindow->rotz += 10.0f;
	}



	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
