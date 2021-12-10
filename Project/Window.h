#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	GLfloat getrota() { return rota; }
	GLfloat getmov_helices() { return mov_helices; }
	GLfloat getmovy() { return movy; }
	GLfloat getmovx() { return movx; }
	GLfloat getmovz() { return movz; }
	GLfloat getrot() { return rot; }
	GLfloat getposLightx() { return posLightx; }
	GLfloat getposLighty() { return posLighty; }
	GLfloat getdirLightx() { return dirLightx; }
	GLfloat getdirLighty() { return dirLighty; }
	GLfloat getposLightFx() { return posLightFx; }
	GLfloat getposLightFz() { return posLightFz; }
	GLfloat getdirLightFx() { return dirLightFx; }
	GLfloat getdirLightFz() { return dirLightFz; }
	GLfloat getcircuitoTanq() { return circuitoTanq; }
	GLfloat getRegaloAnim() { return RegaloAnim; }
	GLfloat getmagia() { return magia; }
	GLfloat getProyectil() { return Proyectil; }
	GLfloat getvistaCuarto() { return vistaCuarto; }
	GLfloat getvistaAerea() { return vistaAerea; }
	GLfloat getvistaSala() { return vistaSala; }
	GLfloat getVista() { return Vista; }
	GLfloat getespectaculoLuces() { return espectaculoLuces; }
	GLfloat getmovPuertaF() { return movPuertaF; }
	GLfloat getrecorrido() { return recorrido; }
	GLfloat getrotx() { return rotx; }
	GLfloat getroty() { return roty; }
	GLfloat getrotz() { return rotz; }
	GLfloat getSaludo() { return saludo; }

	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);
	}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }

	~Window();
private:
	GLFWwindow* mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	GLfloat rota;
	GLfloat mov_helices;
	GLfloat movy;
	GLfloat movx;
	GLfloat movz;
	GLfloat rot;
	GLfloat posLightx;
	GLfloat posLighty;
	GLfloat dirLightx;
	GLfloat dirLighty;
	GLfloat posLightFx;
	GLfloat posLightFz;
	GLfloat dirLightFx;
	GLfloat dirLightFz;
	GLfloat circuitoTanq;
	GLfloat RegaloAnim;
	GLfloat magia;
	GLfloat Proyectil;
	GLfloat vistaCuarto;
	GLfloat vistaAerea;
	GLfloat vistaSala;
	GLfloat Vista;
	GLfloat espectaculoLuces;
	GLfloat espectaculoLucesOff;
	GLfloat movPuertaF;
	GLfloat recorrido;
	GLfloat rotx;
	GLfloat roty;
	GLfloat rotz;
	GLfloat saludo;

	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};