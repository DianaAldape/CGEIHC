/*
Semestre 2022-1
Práctica: Animación Sencilla y animación compleja
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <windows.h>
#include <mmsystem.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"

#include"Model.h"
#include "Skybox.h" 
#include"SpotLight.h"
#include"Sphere.h"

#include <irrKlang.h>

// include console I/O methods (conio.h for windows, our wrapper in linux)
#if defined(WIN32)
#include <conio.h>
#else
#include "common/conio.h"
#endif
using namespace irrklang;



#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

void inputKeyframes(bool* keys);

const float toRadians = 3.14159265f / 180.0f;
const float PI = 3.14159265f;
float movTanqX = 0.0f;
float movTanqZ = 0.0f;
float rotTorreta = 0.5f;
float rotLlantas = 0.0f;
float rotTapa = 0.1f;
float movArbol = -0.0f;
float rotArbol = 0.0f;
float crecer = 0.05f;
float movy = 0.0f;
float movx = 0.0f;
//float aux = 0.0f;
float movProyX = 0.0f;
float movProyY = 0.0f;
float dirLightx = 0.0f;
float dirLightFx = 0.0f;
float dirLightFz = 0.0f;
float rotTimmy = 0.0f;
float movPiernas = 0.0f;
float movBrazoDer = 0.0f;
float movBrazoIzq = 0.0f;
float rotPuerta = 0.0f;
float aux = 0.0f;
float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;
int aux1 = 1;
bool animacion = false;
bool keys[1024];
bool skyDia = true;
bool skynoche = false;
bool rotTorretaIzq = true;
bool rotTorretaDer = false;
bool rotTorretaRegreso = false;
bool rotTorretaRegreso2 = false;
bool Regalo = true;
bool magia = false;
bool EspectaculoLuces = false;
bool luces = true;
bool luces2 = false;
bool recorrido = true;
bool recorrido1 = true;
bool recorrido2 = false;
bool recorrido3 = false;
bool recorrido4 = false;
bool auxMovTimmy = true;
//bool personaje = true;
bool movPuerta = false;
bool abrirPuerta = true;
bool cerrarPuerta = false;
//Audio
int i = 0;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;
Camera cameraVistaAerea;
Camera cameraVistaCuarto;
Camera cameraVistaSala;
Camera cameraInicial;

Texture brickTexture;
Texture dirtTexture;
Texture pisoTexture;
Texture Tagave;
Texture MunecoNieve;
Texture zanahoria;
Texture boton;
Texture sombrero;
Texture verde;
Texture brazoMuneco;

//materiales
Material Material_brillante;
Material Material_opaco;
Material Timmy;
//luz direccional
DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
Model Camino_M;
Model casa;
Model sillon;
Model cama;
Model buro;
Model tv;
Model escritorio;
Model compu;
Model silla;
Model puerta;
Model marcoPuerta;
Model arbol;
Model regalo;
Model regaloTapa;
Model regaloCaja;
Model arbolRegalo;
Model pelota;
//Robot
Model Cuerpo;
Model PiernaDer;
Model PiernaIzq;
Model BrazoDer;
Model BrazoIzq;
//Dino
Model Dino;
Model bocaDino;
//Tanque
Model cuerpoTank;
Model torreta;
Model ruedas;
Model llantaGrande;
Model llantaChica;
//Personaje
Model timmy_cabeza;
Model timmy_tronco;
Model pierna_izq;
Model pierna_izq_abajo;
Model pierna_der;
Model pierna_der_abajo;
Model brazo_izq;
Model brazo_der;
Model mano_der;
Model mano_izq;

Sphere sp = Sphere(2.0, 20, 20);



Skybox skyboxDia;
Skybox skyboxNoche;

glm::vec3 luzNoche(0.4f, 0.4f, 0.8f);
glm::vec3 luzDia(0.9f, 0.9f, 1.0f);


//glm::vec3 PosIni(1.0f, -3.2f, 1.5f);
glm::vec3 PosIni(0.0f, 0.0f, 0.0f);
float posX = PosIni.x, posY = PosIni.y, posZ = PosIni.z, rotRodIzq = 0, rotRodDer = 0, rotBraDer = 0, rotBraIzq = 0, rotRodIzqS = 0, rotRodDerS = 0, rotBraDerS = 0, rotBraIzqS = 0;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";
//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount, 
						unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);
		
		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects() 
{
	unsigned int indices[] = {		
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
	//	x      y      z			u	  v			nx	  ny    nz
		-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};


	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

	Mesh *obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

}

void CrearCubo()
						{
							unsigned int cubo_indices[] = {
								// front
								0, 1, 2,
								2, 3, 0,
								// right
								4, 5, 6,
								6, 7, 4,
								// back
								8, 9, 10,
								10, 11, 8,

								// left
								12, 13, 14,
								14, 15, 12,
								// bottom
								16, 17, 18,
								18, 19, 16,
								// top
								20, 21, 22,
								22, 23, 20,
							};
						// average normals
							GLfloat cubo_vertices[] = {
								// front
								//x		y		z		S		T			NX		NY		NZ
								-0.5f, -0.5f,  0.5f,	0.27f,  0.35f,		0.0f,	0.0f,	-1.0f,	//0
								0.5f, -0.5f,  0.5f,		0.48f,	0.35f,		0.0f,	0.0f,	-1.0f,	//1
								0.5f,  0.5f,  0.5f,		0.48f,	0.64f,		0.0f,	0.0f,	-1.0f,	//2
								-0.5f,  0.5f,  0.5f,	0.27f,	0.64f,		0.0f,	0.0f,	-1.0f,	//3
								// right
								//x		y		z		S		T
								0.5f, -0.5f,  0.5f,	    0.52f,  0.35f,		-1.0f,	0.0f,	0.0f,
								0.5f, -0.5f,  -0.5f,	0.73f,	0.35f,		-1.0f,	0.0f,	0.0f,
								0.5f,  0.5f,  -0.5f,	0.73f,	0.64f,		-1.0f,	0.0f,	0.0f,
								0.5f,  0.5f,  0.5f,	    0.52f,	0.64f,		-1.0f,	0.0f,	0.0f,
								// back
								-0.5f, -0.5f, -0.5f,	0.77f,	0.35f,		0.0f,	0.0f,	1.0f,
								0.5f, -0.5f, -0.5f,		0.98f,	0.35f,		0.0f,	0.0f,	1.0f,
								0.5f,  0.5f, -0.5f,		0.98f,	0.64f,		0.0f,	0.0f,	1.0f,
								-0.5f,  0.5f, -0.5f,	0.77f,	0.64f,		0.0f,	0.0f,	1.0f,

								// left
								//x		y		z		S		T
								-0.5f, -0.5f,  -0.5f,	0.0f,	0.35f,		1.0f,	0.0f,	0.0f,
								-0.5f, -0.5f,  0.5f,	0.23f,  0.35f,		1.0f,	0.0f,	0.0f,
								-0.5f,  0.5f,  0.5f,	0.23f,	0.64f,		1.0f,	0.0f,	0.0f,
								-0.5f,  0.5f,  -0.5f,	0.0f,	0.64f,		1.0f,	0.0f,	0.0f,

								// bottom
								//x		y		z		S		T
								-0.5f, -0.5f,  0.5f,	0.27f,	0.02f,		0.0f,	1.0f,	0.0f,
								0.5f,  -0.5f,  0.5f,	0.48f,  0.02f,		0.0f,	1.0f,	0.0f,
								 0.5f,  -0.5f,  -0.5f,	0.48f,	0.31f,		0.0f,	1.0f,	0.0f,
								-0.5f, -0.5f,  -0.5f,	0.27f,	0.31f,		0.0f,	1.0f,	0.0f,

								//UP
								 //x		y		z		S		T
								 -0.5f, 0.5f,  0.5f,	0.27f,	0.68f,		0.0f,	-1.0f,	0.0f,
								 0.5f,  0.5f,  0.5f,	0.48f,  0.68f,		0.0f,	-1.0f,	0.0f,
								  0.5f, 0.5f,  -0.5f,	0.48f,	0.98f,		0.0f,	-1.0f,	0.0f,
								 -0.5f, 0.5f,  -0.5f,	0.27f,	0.98f,		0.0f,	-1.0f,	0.0f,

							};

							Mesh* cubo = new Mesh();
							cubo->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
							meshList.push_back(cubo);
						}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


#define MAX_FRAMES 30
int i_max_steps = 90;
int i_curr_steps = 5;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float incX;		//Variable para IncrementoX
	float incY;		//Variable para IncrementoY
	float incZ;		//Variable para IncrementoZ
	float rotRodIzq;
	float rotRodIzqS;
	float rotRodDer;
	float rotRodDerS;
	float rotBraDer;
	float rotBraDerS;
	float rotBraIzq;
	float rotBraIzqS;
	float rotInc;
	float rotInc2;
	float rotInc3;
	float rotInc4;
	float rotIncS;
	float rotInc2S;
	float rotInc3S;
	float rotInc4S;


}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 5;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);


	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].rotRodDer = rotRodDer;
	KeyFrame[FrameIndex].rotBraDer = rotBraDer;
	KeyFrame[FrameIndex].rotBraIzq = rotBraIzq;
	KeyFrame[FrameIndex].rotRodIzqS = rotRodIzqS;
	KeyFrame[FrameIndex].rotRodDerS = rotRodDerS;
	KeyFrame[FrameIndex].rotBraDerS = rotBraDerS;
	KeyFrame[FrameIndex].rotBraIzqS = rotBraIzqS;

	FrameIndex++;
}

void resetElements(void)
{

	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	rotRodDer = KeyFrame[0].rotRodDer;
	rotBraDer = KeyFrame[0].rotBraDer;
	rotBraIzq = KeyFrame[0].rotBraIzq;
	rotRodIzqS = KeyFrame[0].rotRodIzqS;
	rotRodDerS = KeyFrame[0].rotRodDerS;
	rotBraDerS = KeyFrame[0].rotBraDerS;
	rotBraIzqS = KeyFrame[0].rotBraIzqS;
}

void interpolation(void)
{
	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	KeyFrame[playIndex].rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	KeyFrame[playIndex].rotInc2 = (KeyFrame[playIndex + 1].rotRodDer - KeyFrame[playIndex].rotRodDer) / i_max_steps;
	KeyFrame[playIndex].rotInc3 = (KeyFrame[playIndex + 1].rotBraDer - KeyFrame[playIndex].rotBraDer) / i_max_steps;
	KeyFrame[playIndex].rotInc4 = (KeyFrame[playIndex + 1].rotBraIzq - KeyFrame[playIndex].rotBraIzq) / i_max_steps;
	KeyFrame[playIndex].rotIncS = (KeyFrame[playIndex + 1].rotRodIzqS - KeyFrame[playIndex].rotRodIzqS) / i_max_steps;
	KeyFrame[playIndex].rotInc2S = (KeyFrame[playIndex + 1].rotRodDerS - KeyFrame[playIndex].rotRodDerS) / i_max_steps;
	KeyFrame[playIndex].rotInc3S = (KeyFrame[playIndex + 1].rotBraDerS - KeyFrame[playIndex].rotBraDerS) / i_max_steps;
	KeyFrame[playIndex].rotInc4S = (KeyFrame[playIndex + 1].rotBraIzqS - KeyFrame[playIndex].rotBraIzqS) / i_max_steps;
}

void animate(void)
{
	//Movimiento del objeto
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("Frame index= %d\n", FrameIndex);
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				//printf("entro aquí\n");
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//printf("se quedó aqui\n");
			//printf("max steps: %f", i_max_steps);
			//Draw animation
			posX += KeyFrame[playIndex].incX;
			posY += KeyFrame[playIndex].incY;
			posZ += KeyFrame[playIndex].incZ;

			rotRodIzq += KeyFrame[playIndex].rotInc;
			rotRodIzqS += KeyFrame[playIndex].rotIncS;
			rotRodDer += KeyFrame[playIndex].rotInc2;
			rotRodDerS += KeyFrame[playIndex].rotInc2S;
			rotBraDer += KeyFrame[playIndex].rotInc3;
			rotBraDerS += KeyFrame[playIndex].rotInc3S;
			rotBraIzq += KeyFrame[playIndex].rotInc4;
			rotBraIzqS += KeyFrame[playIndex].rotInc4S;
			i_curr_steps++;
		}
	}
}


int main(int argc, const char** argv)
{
	ISoundEngine* engine = irrklang::createIrrKlangDevice();

	//mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow = Window(800, 600); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearCubo();
	CreateShaders();

	camera = Camera(glm::vec3(-2.0f, 1.0f, 20.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 10.0f, 0.5f);
	cameraVistaCuarto = Camera(glm::vec3(7.0f, 5.5f, -13.5f), glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, 0.0f, 5.0f, 0.5f);
	cameraVistaAerea = Camera(glm::vec3(0.0f, 55.0f, -15.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -90.0f, 5.0f, 0.5f);
	cameraVistaSala = Camera(glm::vec3(4.0f, -0.6f, -9.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);
	//cameraInicial = Camera(posTimmy, glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 10.0f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso2.tga");
	pisoTexture.LoadTextureA();
	Tagave = Texture("Textures/Agave.tga");
	Tagave.LoadTextureA();
	MunecoNieve = Texture("Textures/munecoNieve.png");
	MunecoNieve.LoadTexture();
	zanahoria = Texture("Textures/zanahoria.png");
	zanahoria.LoadTexture();
	boton = Texture("Textures/boton.png");
	boton.LoadTexture();
	sombrero = Texture("Textures/sombrero.png");
	sombrero.LoadTexture();
	verde = Texture("Textures/verde.png");
	verde.LoadTexture();
	brazoMuneco = Texture("Textures/arbolAnimCentro.png");
	brazoMuneco.LoadTexture();
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);
	Timmy = Material(0.2, 1);
	//Casa
	casa = Model();
	casa.LoadModel("Models/casaCompleta.obj");
	//Sala
	sillon = Model();
	sillon.LoadModel("Models/sillon.obj");
	tv = Model();
	tv.LoadModel("Models/tv.obj");
	marcoPuerta = Model();
	marcoPuerta.LoadModel("Models/marco_puerta.obj");
	puerta = Model();
	puerta.LoadModel("Models/puerta.obj");
	arbol = Model();
	arbol.LoadModel("Models/arbol.obj");
	regalo = Model();
	regalo.LoadModel("Models/regalo.obj");
	pelota = Model();
	pelota.LoadModel("Models/pelota.obj");
	regaloTapa = Model();
	regaloTapa.LoadModel("Models/regaloTapa.obj");
	regaloCaja = Model();
	regaloCaja.LoadModel("Models/regaloCaja.obj");
	arbolRegalo = Model();
	arbolRegalo.LoadModel("Models/arbolAnim.obj");
	//Cuarto
	cama = Model();
	cama.LoadModel("Models/camacompleta2.obj");
	buro = Model();
	buro.LoadModel("Models/buro.obj");
	escritorio = Model();
	escritorio.LoadModel("Models/desk.obj");
	compu = Model();
	compu.LoadModel("Models/compu.obj");
	//Personaje
	timmy_tronco = Model();
	timmy_tronco.LoadModel("Models/Timmy/timmy.obj");
	pierna_der = Model();
	pierna_der.LoadModel("Models/Timmy/pierna_der_arriba.obj");
	pierna_der_abajo = Model();
	pierna_der_abajo.LoadModel("Models/Timmy/pierna_der_abajo.obj");
	pierna_izq = Model();
	pierna_izq.LoadModel("Models/Timmy/pierna_izq_arriba.obj");
	pierna_izq_abajo = Model();
	pierna_izq_abajo.LoadModel("Models/Timmy/pierna_izq_abajo.obj");
	brazo_der = Model();
	brazo_der.LoadModel("Models/Timmy/brazo_sup_der.obj");
	brazo_izq = Model();
	brazo_izq.LoadModel("Models/Timmy/brazo_sup_izq.obj");
	mano_der = Model();
	mano_der.LoadModel("Models/Timmy/muneca_der.obj");
	mano_izq = Model();
	mano_izq.LoadModel("Models/Timmy/muneca_izq.obj");
	//Dino
	Dino = Model();
	Dino.LoadModel("Models/Dinosaurio/dinosaurio.obj");
	bocaDino = Model();
	bocaDino.LoadModel("Models/Dinosaurio/bocadinosaurio.obj");
	//Tanque
	cuerpoTank = Model();
	cuerpoTank.LoadModel("Models/Tank/estructura2.obj");
	ruedas = Model();
	ruedas.LoadModel("Models/Tank/ruedas.obj");
	llantaGrande = Model();
	llantaGrande.LoadModel("Models/Tank/llantaGrande.obj");
	llantaChica = Model();
	llantaChica.LoadModel("Models/Tank/llantaChica.obj");
	torreta = Model();
	torreta.LoadModel("Models/Tank/torreta.obj");



	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].rotRodDer = 0;
		KeyFrame[i].rotBraDer = 0;
		KeyFrame[i].rotBraIzq = 0;
		KeyFrame[i].rotRodIzqS = 0;
		KeyFrame[i].rotRodDerS = 0;
		KeyFrame[i].rotBraDerS = 0;
		KeyFrame[i].rotBraIzqS = 0;
		KeyFrame[i].rotIncS = 0;
		KeyFrame[i].rotInc2S = 0;
		KeyFrame[i].rotInc3S = 0;
		KeyFrame[i].rotInc4S = 0;
		KeyFrame[i].rotInc = 0;
		KeyFrame[i].rotInc2 = 0;
		KeyFrame[i].rotInc3 = 0;
		KeyFrame[i].rotInc4 = 0;

	}

	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(0.9f, 0.95f, 1.0f, //Noche 0.8f, 0.8f, 1.0f
		1.0f, 1.0f,
		0.0f, 1.0f, 0.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		0.0f, 1.0f,
		5.0f, 2.0f, -15.0f,
		1.0f, 1.0f, 1.0f);
	pointLightCount++;
	pointLights[1] = PointLight(1.0f, 0.0f, 1.0f,
		0.0f, 1.0f,
		2.0f, 7.8f, -13.0f,
		1.0f, 1.0f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(0.0f, 1.0f, 0.0f,
		0.5f, 2.0f,
		2.09f, 1.51f, -16.0f,
		-0.09f, -0.6f, -0.76f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	spotLights[1] = SpotLight(1.0f, 0.0f, 0.0f,
		1.0f, 2.0f,
		//1.2f, 0.9f, -20.0f,
		4.09f, 1.51f, -15.0f,
		0.45f, -0.7f, -56.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	spotLights[2] = SpotLight(1.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		3.09f, 1.51f, -18.0f,
		-0.54f, -0.36f, -0.75f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	std::vector<std::string> skyboxFaces;

	//Dia
	skyboxFaces.push_back("Textures/Skybox3/right.tga");
	skyboxFaces.push_back("Textures/Skybox3/left.tga");
	skyboxFaces.push_back("Textures/Skybox3/bottom.tga");
	skyboxFaces.push_back("Textures/Skybox3/top.tga");
	skyboxFaces.push_back("Textures/Skybox3/back.tga");
	skyboxFaces.push_back("Textures/Skybox3/front.tga");
	std::vector<std::string> skyboxFacesNoche;
	//noche
	skyboxFacesNoche.push_back("Textures/SkyNight/right.tga");
	skyboxFacesNoche.push_back("Textures/SkyNight/left.tga");
	skyboxFacesNoche.push_back("Textures/SkyNight/bottom.tga");
	skyboxFacesNoche.push_back("Textures/SkyNight/top.tga");
	skyboxFacesNoche.push_back("Textures/SkyNight/back.tga");
	skyboxFacesNoche.push_back("Textures/SkyNight/front.tga");

	skyboxDia = Skybox(skyboxFaces);
	skyboxNoche = Skybox(skyboxFacesNoche);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);

	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader
	
	//bool musica = true;
	//PlaySoundW(TEXT("media\Cancion.wav"), NULL, SND_ASYNC);
	
	//	//i++;
	//	
	//	//printf("i = %i", i);
	//	break;
	//}
	engine->play2D("media/Cancion.wav", true, false, true);

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		/*if (mainWindow.getMusica())
		{*/
			//PlaySound(TEXT("media\\Cancion.wav"), NULL, SND_ASYNC);
			//engine->play2D("media/Cancion.wav", true,false, true );
		/*}u
		else
		{
			Sleep(1000); 
		}*/
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;
		

		//Tanque
		if (mainWindow.getcircuitoTanq())
		{
			if (movTanqX < 20.0f)
			{
				//printf("Movimiento x = %f", movTanqX);
				movTanqX += 3.0 * deltaTime;
				rotLlantas += 10.0f * deltaTime;
			}
			if (rotTorretaIzq)
			{
				rotTorreta += 5.0f * deltaTime;
				if (rotTorreta >= 90)
				{
					rotTorretaIzq = false;
					rotTorretaRegreso = true;
				}
			}
			if (rotTorretaRegreso)
			{
				rotTorreta -= 5.0f * deltaTime;
				if (rotTorreta < 0)
				{
					rotTorretaRegreso = false;
					rotTorretaDer = true;

				}
			}
			if (rotTorretaDer)
			{
				rotTorreta -= 5.0f * deltaTime;
				if (rotTorreta < -90)
				{
					rotTorretaDer = false;
					rotTorretaRegreso2 = true;
				}
			}
			if (rotTorretaRegreso2)
			{
				rotTorreta += 5.0f * deltaTime;
				if (rotTorreta > 0)
				{
					rotTorretaRegreso2 = false;
					rotTorretaIzq = true;
				}
			}
		}
		//Regalo Cajita
		if (mainWindow.getRegaloAnim() )
		{	
			if (Regalo)
			{				
				rotTapa += 6.5f * deltaTime;
				movArbol += 0.1f * deltaTime;
				while (i < 4) {
					engine->play2D("media/caja_musica.wav", false, false, true);
					printf("i = %i", i);
					i++;
					break;
				}
				if (movArbol > 1.15)
				{
					Regalo = false;
				}
			}
			rotArbol += 4.0f * deltaTime;
		}
		//Magia
		if (mainWindow.getmagia())
		{
			if (crecer < 0.09)
			{
				crecer += 0.02f * deltaTime;
				movy  += 0.15f * deltaTime;
				movx -= 0.5f * deltaTime;
				while (i < 2)  {
					i ++;
					engine->play2D("media/bell.wav", false, false, true);
					printf("i = %i", i);
					break;
				}	
			}
			else
			{
				magia = false;
			}
			
		}
		//Espectaculo Luces
		if ((EspectaculoLuces) || (mainWindow.getespectaculoLuces()))
		{
			if (luces)
			{
				if (dirLightx < 0.5)
				{
					dirLightx += 0.01f;
					dirLightFx -= 0.01f;
					dirLightFz -= 0.01f;
				}
				else
				{
					luces = false;
					luces2 = true;
				}
			}
			if (luces2)
			{
				if (dirLightx > -0.5)
				{
					dirLightx -= 0.01f;
					dirLightFx += 0.01f;
					dirLightFz += 0.01f;
				}
				else
				{
					luces2 = false;
					luces = true;
				}
			}
		}
		//Movimiento de puerta
		if (mainWindow.getmovPuertaF())
		{
			if (abrirPuerta)
			{
				while (i < 2) {
					i++;
					engine->play2D("media/puertaAbre.wav", false, false, true);
					printf("i = %i", i);
					break;
				}

				rotPuerta -= 20.0f * deltaTime;
				if (rotPuerta < -90)
				{
					abrirPuerta = false;
					cerrarPuerta = true;
					engine->play2D("media/puertaCierra.wav", false, false, true);
				}
			}
			if (cerrarPuerta)
			{
				
				rotPuerta += 20.0f * deltaTime;
				if (rotPuerta > 0)
				{
					cerrarPuerta = false;
					recorrido = false;
				}
			}
		}
		//Recorrido Inicial
		if (mainWindow.getrecorrido())
		{
			if (recorrido1)
			{
				//posX += 0.5f;
				rotTimmy = 90.0f;
				posZ -= 0.009;
				if (auxMovTimmy)
				{
					rotRodDer = 0.0f;
					rotRodDerS += 0.4f;
					rotRodIzqS -= 0.4f;
					rotRodIzq -= 0.5f;
					rotBraDerS -= 0.4f;
					rotBraIzqS += 0.4f;
					if (rotRodDerS > 30)
					{
						auxMovTimmy = false;

					}
				}
				if (!auxMovTimmy)
				{	
					rotRodDerS -= 0.4f;
					rotRodIzqS += 0.4f;
					rotRodIzq = 0.0f;
					rotRodDer -= 0.5f;
					rotBraDerS += 0.4f;
					rotBraIzqS -= 0.4f;
					if (rotRodDerS < -30)
					{
						auxMovTimmy = true;
					}
				}
				if (posZ < -7.7f)
				{
					rotRodDerS = 0.0f;
					rotRodIzqS = 0.0f;
					rotRodIzq = 0.0f;
					rotRodDer = 0.0f;
					rotBraDerS = 0.0f;
					rotBraIzqS = 0.0f;
					recorrido1 = false;
					engine->play2D("media/puertaAbre.wav",
						false, false, true);
					movPuerta = true;
				}
			}
			if (movPuerta)
			{
				
				if (abrirPuerta)
				{
					rotPuerta -= 20.0f * deltaTime;
					if (rotPuerta < -90)
					{
						abrirPuerta = false;
						recorrido2 = true;
					}
				}
			}
			if (recorrido2)
			{
				rotRodDerS = 60.0f;
				rotRodDer = -30.0f;
				posY += 0.008f;
				posZ -= 0.008f;
				printf("Pos y = %f", posY);
				if (posY > 0.3f)
				{
					rotRodDer = 0.0f;
					rotRodDerS = 0.0f;
					rotRodIzqS = 60.0f;
					rotRodIzq = -30.0f;
					posZ -= 0.008f;
					posY += 0.008f;
					printf("pos y = %f", posY);
					if (posY > 0.75)
					{
						rotRodIzqS = 0.0f;
						rotRodIzq = 0.0f;
						posZ -= 0.008f;
						printf("pos z = %f", posZ);
						if (posZ < -0.5f)
						{
							recorrido2 = false;
							recorrido3 = true;
						}
					}
				}
			}
			if (recorrido3)
			{
				posZ -= 0.009;
				if (auxMovTimmy)
				{
					rotRodDerS += 0.4f;
					rotRodIzqS -= 0.4f;
					rotBraDerS -= 0.4f;
					rotBraIzqS += 0.4f;
					if (rotRodDerS > 20)
					{
						auxMovTimmy = false;

					}
				}
				if (!auxMovTimmy)
				{
					//movTimmyZ -+ 0.006f;
					rotRodDerS -= 0.4f;
					rotRodIzqS += 0.4f;
					rotBraDerS += 0.4f;
					rotBraIzqS -= 0.4f;
					if (rotRodDerS < -20)
					{
						auxMovTimmy = true;
					}
				}
				if (posZ < -11.7f)
				{
					rotRodDerS = 0.0f;
					rotRodIzqS = 0.0f;
					rotBraDerS = 0.0f;
					rotBraIzqS = 0.0f;
					posY = 0.3f;
					recorrido3 = false;
					EspectaculoLuces = true;
					//cerrarPuerta = true;
					recorrido4 = true;
				}
			}
			if(recorrido4)
			{
				posZ -= 0.009;
				if (auxMovTimmy)
				{
					rotRodDerS += 0.4f;
					rotRodIzqS -= 0.4f;
					rotBraDerS -= 0.4f;
					rotBraIzqS += 0.4f;
					if (rotRodDerS > 20)
					{
						auxMovTimmy = false;

					}
				}
				if (!auxMovTimmy)
				{
					rotRodDerS -= 0.4f;
					rotRodIzqS += 0.4f;
					rotBraDerS += 0.4f;
					rotBraIzqS -= 0.4f;
					if (rotRodDerS < -20)
					{
						auxMovTimmy = true;
					}
				}
				if (posZ < -17.7f)
				{
					rotRodDerS = 0.0f;
					rotRodIzqS = 0.0f;
					rotBraDerS = 0.0f;
					rotBraIzqS = 0.0f;
					recorrido4 = false;
					engine->play2D("media/puertaCierra.wav", 
						false, false, true);
					cerrarPuerta = true;
				}
			}
			if (cerrarPuerta)
			{
				rotPuerta += 20.0f * deltaTime;
				if (rotPuerta > 0)
				{
					cerrarPuerta = false;
					recorrido = false;
				}
			}
		}		


		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		inputKeyframes(mainWindow.getsKeys());

		animate();

		cameraVistaCuarto.keyControl(mainWindow.getsKeys(), deltaTime);
		cameraVistaCuarto.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		cameraVistaAerea.keyControl(mainWindow.getsKeys(), deltaTime);
		cameraVistaSala.keyControl(mainWindow.getsKeys(), deltaTime);
		cameraVistaSala.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (skyDia)
		{
			skyboxDia.DrawSkybox(camera.calculateViewMatrix(), projection);
			mainLight.SetDiaNoche(luzDia);
			ciclo += 1.0f;
			if (ciclo > 1000)
			{
				skyDia = false;
				skynoche = true;
			}
			//spotLightCount = -1;

		}
		if (skynoche)
		{
			skyboxNoche.DrawSkybox(camera.calculateViewMatrix(), projection);
			ciclo -= 1.0f;
			mainLight.SetDiaNoche(luzNoche);
			if (ciclo < 0)
			{
				skynoche = false;
				skyDia = true;
			}
		}

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glm::vec3 dirLight(dirLightx, -1.0f, -0.4);
		spotLights[0].SetDir(dirLight);
		glm::vec3 dirLight1(dirLightFx, -1.0f, -0.4f);
		spotLights[1].SetDir(dirLight1);
		glm::vec3 dirLight2(-0.0, -1.0f, dirLightFz);
		spotLights[2].SetDir(dirLight2);

		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		//Vistas
		if (mainWindow.getVista() )
		{
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
			glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
		}
		if (mainWindow.getvistaAerea())
		{
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(cameraVistaAerea.calculateViewMatrix()));
			glUniform3f(uniformEyePosition, cameraVistaAerea.getCameraPosition().x, cameraVistaAerea.getCameraPosition().y, cameraVistaAerea.getCameraPosition().z);
		}
		if (mainWindow.getvistaCuarto())
		{
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(cameraVistaCuarto.calculateViewMatrix()));
			glUniform3f(uniformEyePosition, cameraVistaCuarto.getCameraPosition().x, cameraVistaCuarto.getCameraPosition().y, cameraVistaCuarto.getCameraPosition().z);
			glm::vec3 posTimmy(7.5f, 5.5f, -13.5f);
		}
		if (mainWindow.getvistaSala())
		{
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(cameraVistaSala.calculateViewMatrix()));
			glUniform3f(uniformEyePosition, cameraVistaSala.getCameraPosition().x, cameraVistaSala.getCameraPosition().y, cameraVistaSala.getCameraPosition().z);
		}

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 modelauxTimmy(1.0);
		glm::mat4 modelauxTank(1.0);
		glm::mat4 modelauxRegalo(1.0);
		glm::mat4 modelauxBrazo(1.0);
		glm::mat4 modelauxMuneco(1.0);
		glm::mat4 modelauxMunecoBotones(1.0);
		glm::mat4 modelauxMunecoBrazos(1.0);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -6.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		pisoTexture.UseTexture();
		meshList[2]->RenderMesh();
		//Casa
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, -15.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		casa.RenderModel();
		modelaux = model;
		model = glm::translate(model, glm::vec3(5.2f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 3.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sillon.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.8f, 0.0f, -1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tv.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.2f, 6.8f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cama.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.0f, 6.8f, 3.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		buro.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.9f, 5.0f, 4.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		compu.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(4.5f, 7.0f, 3.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		escritorio.RenderModel();
		//Regalo Anim
		model = modelaux;
		model = glm::translate(model, glm::vec3(5.5f, 8.3f, 3.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		regaloCaja.RenderModel();
		modelauxRegalo = model;
		model = glm::translate(model, glm::vec3(-0.93f, 0.8f, 0.0f));
		model = glm::rotate(model, rotTapa * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::translate(model, glm::vec3(0.0f + 0.0f, 0.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		regaloTapa.RenderModel();
		model = modelauxRegalo;
		model = glm::translate(model, glm::vec3(0.0f, -0.6f + movArbol, 0.0f));
		model = glm::rotate(model, rotArbol * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbolRegalo.RenderModel();
		//Magia
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.5f, 7.5f + movy, 1.0f));
		model = glm::rotate(model, -15 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(crecer, crecer, crecer));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dino.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 14.2f, 15.5f));
		model = glm::rotate(model, mainWindow.getrotx() * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bocaDino.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(8.0f, -3.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();
		model = glm::translate(model, glm::vec3(4.0f, 2.5f, -2.5f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		regalo.RenderModel();
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, -2.4f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		regalo.RenderModel();
		//Puerta
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.7f, -1.0f, -8.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		marcoPuerta.RenderModel();
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotPuerta * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, -0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		puerta.RenderModel();
		//Tanque
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f + movTanqX, -3.0f, 15.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuerpoTank.RenderModel();
		modelauxTank = model;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ruedas.RenderModel();
		model = modelauxTank;
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 1.16));
		model = glm::rotate(model, rotLlantas * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		llantaGrande.RenderModel();
		model = modelauxTank;
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, -1.195));
		model = glm::rotate(model, rotLlantas * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		llantaGrande.RenderModel();
		model = modelauxTank;
		model = glm::translate(model, glm::vec3(-0.95f, 0.0f, 1.18));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotLlantas * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		llantaGrande.RenderModel();
		model = modelauxTank;
		model = glm::translate(model, glm::vec3(-0.95f, 0.0f, -1.195));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotLlantas * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		llantaGrande.RenderModel();
		model = modelauxTank;
		model = glm::translate(model, glm::vec3(-0.95f, 0.17f, -0.5));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotLlantas * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		llantaChica.RenderModel();
		model = modelauxTank;
		model = glm::translate(model, glm::vec3(-0.95f, -0.15f, -0.5));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotLlantas * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		llantaChica.RenderModel();
		model = modelauxTank;
		model = glm::translate(model, glm::vec3(-0.95f, 0.17f, -0.1));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotLlantas * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		llantaChica.RenderModel();
		model = modelauxTank;
		model = glm::translate(model, glm::vec3(-0.95f, -0.15f, -0.1));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotLlantas * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		llantaChica.RenderModel();
		model = modelauxTank;
		model = glm::translate(model, glm::vec3(-0.95f, 0.17f, 0.3));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotLlantas * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		llantaChica.RenderModel();
		model = modelauxTank;
		model = glm::translate(model, glm::vec3(-0.95f, -0.15f, 0.3));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotLlantas * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		llantaChica.RenderModel();
		model = modelauxTank;
		model = glm::translate(model, glm::vec3(1.0f, 0.17f, -0.5));
		model = glm::rotate(model, rotLlantas * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		llantaChica.RenderModel();
		model = modelauxTank;
		model = glm::translate(model, glm::vec3(1.0f, -0.15f, -0.5));
		model = glm::rotate(model, rotLlantas * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		llantaChica.RenderModel();
		model = modelauxTank;
		model = glm::translate(model, glm::vec3(1.0f, 0.17f, -0.1));
		model = glm::rotate(model, rotLlantas * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		llantaChica.RenderModel();
		model = modelauxTank;
		model = glm::translate(model, glm::vec3(1.0f, -0.15f, -0.1));
		model = glm::rotate(model, rotLlantas * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		llantaChica.RenderModel();
		model = modelauxTank;
		model = glm::translate(model, glm::vec3(1.0f, 0.17f, 0.3));
		model = glm::rotate(model, rotLlantas * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		llantaChica.RenderModel();
		model = modelauxTank;
		model = glm::translate(model, glm::vec3(1.0f, -0.15f, 0.3));
		model = glm::rotate(model, rotLlantas * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		llantaChica.RenderModel();
		model = modelauxTank;
		model = glm::translate(model, glm::vec3(0.0f, 0.45f, -0.35f));
		model = glm::rotate(model, rotTorreta * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		torreta.RenderModel();


		//glm::vec3 PosIni(1.0f, -3.2f, 1.5f);
		//Personaje
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(posX+1.8f, -2.6f+posY, posZ+1.9f));
		model = glm::rotate(model, rotTimmy * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Timmy.UseMaterial(uniformSpecularIntensity, uniformShininess);
		timmy_tronco.RenderModel();
		modelauxTimmy = model;
		model = glm::translate(model, glm::vec3(0.0, -0.29f, 0.11));
		model = glm::rotate(model, rotRodDerS *toRadians, glm::vec3(0.0f, 0.0f,1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pierna_der.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, -0.3f, 0.0));
		model = glm::rotate(model, rotRodDer* toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pierna_der_abajo.RenderModel();
		model = modelauxTimmy;
		model = glm::translate(model, glm::vec3(0.0, -0.29f, -0.13));
		model = glm::rotate(model, rotRodIzqS * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pierna_izq.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, -0.3f, 0.0));
		model = glm::rotate(model, rotRodIzq * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pierna_izq_abajo.RenderModel();
		model = modelauxTimmy;
		model = glm::translate(model, glm::vec3(0.0f, 0.25f, 0.15f));
		model = glm::rotate(model, rotBraDerS * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brazo_der.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, -0.251f, 0.0707f));
		model = glm::rotate(model, rotBraDer * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mano_der.RenderModel();
		model = modelauxTimmy;
		model = glm::translate(model, glm::vec3(0.0f, 0.25f,-0.15f));
		model = glm::rotate(model, rotBraIzqS * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brazo_izq.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, -0.242f, -0.09f));
		model = glm::rotate(model, rotBraIzq * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mano_izq.RenderModel();
		glDisable(GL_BLEND);
		//Muñeco de nieve
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, -3.0f, 3.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		MunecoNieve.UseTexture();
		sp.render(); //Renderiza esfera
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		modelauxMuneco = model;
		model = glm::translate(model, glm::vec3(0.0f, 0.3f, 2.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 1.2f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		zanahoria.UseTexture();
		meshList[1]->RenderMesh();
		model = modelauxMuneco;
		model = glm::translate(model, glm::vec3(0.7f, 0.8f, 1.7f));
		model = glm::scale(model, glm::vec3(0.13f, 0.13f, 0.07f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		boton.UseTexture();
		sp.render();
		model = modelauxMuneco;
		model = glm::translate(model, glm::vec3(-0.7f, 0.8f, 1.7f));
		model = glm::scale(model, glm::vec3(0.13f, 0.13f, 0.07f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		model = modelauxMuneco;
		model = glm::translate(model, glm::vec3(-0.7f, 0.0f, 1.85f));
		model = glm::scale(model, glm::vec3(0.04f, 0.04f, 0.04f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		model = modelauxMuneco;
		model = glm::translate(model, glm::vec3(-0.4f, -0.1f, 1.925f));
		model = glm::scale(model, glm::vec3(0.04f, 0.04f, 0.04f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		model = modelauxMuneco;
		model = glm::translate(model, glm::vec3(-0.1f, -0.2f, 1.95f));
		model = glm::scale(model, glm::vec3(0.04f, 0.04f, 0.04f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		model = modelauxMuneco;
		model = glm::translate(model, glm::vec3(0.2f, -0.2f, 1.95f));
		model = glm::scale(model, glm::vec3(0.04f, 0.04f, 0.04f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		model = modelauxMuneco;
		model = glm::translate(model, glm::vec3(0.5f, -0.1f, 1.925f));
		model = glm::scale(model, glm::vec3(0.04f, 0.04f, 0.04f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		model = modelauxMuneco;
		model = glm::translate(model, glm::vec3(0.8f, 0.0f, 1.85f));
		model = glm::scale(model, glm::vec3(0.04f, 0.04f, 0.04f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		model = modelauxMuneco;
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 2.7f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		model = modelauxMuneco;
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 2.35f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		model = modelauxMuneco;
		model = glm::translate(model, glm::vec3(0.0f, -4.0f, 2.7f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, 2.6f, 3.8f));
		model = glm::scale(model, glm::vec3(1.2f, 1.0f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		sombrero.UseTexture();
		meshList[4]->RenderMesh();
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.0f));
		model = glm::scale(model, glm::vec3(1.3f, 0.25f, 1.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshList[4]->RenderMesh();
		model = glm::translate(model, glm::vec3(0.0f, 0.9f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.7f, 0.80f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		verde.UseTexture();
		meshList[4]->RenderMesh();
		model = modelauxMuneco;
		model = glm::translate(model, glm::vec3(0.0f, -1.1f, 1.6f));
		model = glm::scale(model, glm::vec3(3.5f, 0.5f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		verde.UseTexture();
		meshList[4]->RenderMesh();
		model = modelauxMuneco;
		model = glm::translate(model, glm::vec3(1.6f, -1.1f, 0.1f));
		model = glm::scale(model, glm::vec3(0.2f, 0.5f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		verde.UseTexture();
		meshList[4]->RenderMesh();
		model = modelauxMuneco;
		model = glm::translate(model, glm::vec3(-1.6f, -1.1f, 0.1f));
		model = glm::scale(model, glm::vec3(0.2f, 0.5f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		verde.UseTexture();
		meshList[4]->RenderMesh();
		model = modelauxMuneco;
		model = glm::translate(model, glm::vec3(1.4f, -3.0f, 1.7f));
		model = glm::scale(model, glm::vec3(0.5f, 3.0f, 1.2f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		verde.UseTexture();
		meshList[4]->RenderMesh();
		model = modelauxMuneco;
		model = glm::translate(model, glm::vec3(0.0f, -1.1f, -1.5f));
		model = glm::scale(model, glm::vec3(3.5f, 0.5f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		verde.UseTexture();
		meshList[4]->RenderMesh();
		model = modelauxMuneco;
		model = glm::translate(model, glm::vec3(-4.0f, -2.0f, 0.5f));
		model = glm::rotate(model, -20 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelauxMunecoBrazos = model;
		model = glm::scale(model, glm::vec3(3.0f, 0.20f, 0.20f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brazoMuneco.UseTexture();
		meshList[4]->RenderMesh();
		model = modelauxMunecoBrazos;
		model = glm::rotate(model, -40 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f, 0.08f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[4]->RenderMesh();
		model = modelauxMuneco;
		model = glm::translate(model, glm::vec3(4.0f, -2.0f, 0.5f));
		model = glm::rotate(model, 20 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelauxMunecoBrazos = model;
		model = glm::scale(model, glm::vec3(3.0f, 0.20f, 0.20f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brazoMuneco.UseTexture();
		meshList[4]->RenderMesh();
		model = modelauxMunecoBrazos;
		model = glm::rotate(model, 40 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(1.0f, -0.25f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[4]->RenderMesh();
		model = modelauxMuneco;
		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	engine->drop(); // delete engine
	return 0;
}

void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (play == false && (FrameIndex > 1))
		{
			resetElements();
			//First Interpolation				
			interpolation();
			play = true;
			playIndex = 0;
			i_curr_steps = 0;
			//reproduciranimacion++;
			printf("\n presiona 0 para habilitar reproducir de nuevo la animación'\n");
			//habilitaranimacion = 0;
		}
		else
		{
			play = false;
		}
	}
	if (keys[GLFW_KEY_0])
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
	if (keys[GLFW_KEY_UP])
	{
		rotTimmy = 90.0f;
		posZ -= 0.5 * deltaTime;
		if (!auxMovTimmy)
		{
			rotRodIzq = 0.0f;
			rotRodIzqS += 1.0f;
			rotRodDerS -= 1.0f;
			rotRodDer -= 0.5f;
			rotBraDerS += 1.0f;
			rotBraIzqS -= 1.0f;
			printf("Rotacion rod Izq: %f", rotRodIzq);
			if (rotRodIzqS > 40.0f)
			{
				auxMovTimmy=true;
			}
		}		
		if (auxMovTimmy)
		{
			rotRodDer = 0.0f;
			rotRodIzqS -= 1.0f;
			rotRodDerS += 1.0f;
			rotRodIzq -= 0.5f;
			rotBraDerS -= 1.0f;
			rotBraIzqS += 1.0f;
			printf("Rotacion rod Der: %f", rotRodDer);
			if (rotRodDerS > 40.0f)
			{
				auxMovTimmy = false;
				
			}
		}
	}
	if (keys[GLFW_KEY_DOWN])
	{
		rotTimmy = -90.0f;
		posZ += 0.5 * deltaTime;
		if (!auxMovTimmy)
		{
			rotRodIzq = 0.0f;
			rotRodIzqS += 1.0f;
			rotRodDerS -= 1.0f;
			rotRodDer -= 0.5f;
			rotBraDerS += 1.0f;
			rotBraIzqS -= 1.0f;
			printf("Rotacion rod Izq: %f", rotRodIzq);
			if (rotRodIzqS > 40.0f)
			{
				auxMovTimmy = true;
			}
		}
		if (auxMovTimmy)
		{
			rotRodDer = 0.0f;
			rotRodIzqS -= 1.0f;
			rotRodDerS += 1.0f;
			rotRodIzq -= 0.5f;
			rotBraDerS -= 1.0f;
			rotBraIzqS += 1.0f;
			printf("Rotacion rod Der: %f", rotRodDer);
			if (rotRodDerS > 40.0f)
			{
				auxMovTimmy = false;
			}
		}
	}
	if (keys[GLFW_KEY_RIGHT])
	{
		rotTimmy = 0.0f;
		posX += 0.5 * deltaTime;
		if (!auxMovTimmy)
		{
			rotRodIzq = 0.0f;
			rotRodIzqS += 1.0f;
			rotRodDerS -= 1.0f;
			rotRodDer -= 0.5f;
			rotBraDerS += 1.0f;
			rotBraIzqS -= 1.0f;
			printf("Rotacion rod Izq: %f", rotRodIzq);
			if (rotRodIzqS > 40.0f)
			{
				auxMovTimmy = true;
			}
		}
		if (auxMovTimmy)
		{
			rotRodDer = 0.0f;
			rotRodIzqS -= 1.0f;
			rotRodDerS += 1.0f;
			rotRodIzq -= 0.5f;
			rotBraDerS -= 1.0f;
			rotBraIzqS += 1.0f;
			printf("Rotacion rod Der: %f", rotRodDer);
			if (rotRodDerS > 40.0f)
			{
				auxMovTimmy = false;
			}
		}
	}
	if (keys[GLFW_KEY_LEFT])
	{
		rotTimmy = 180.0f;
		posX -= 0.5 * deltaTime;
		if (!auxMovTimmy)
		{
			rotRodIzq = 0.0f;
			rotRodIzqS += 1.0f;
			rotRodDerS -= 1.0f;
			rotRodDer -= 0.5f;
			rotBraDerS += 1.0f;
			rotBraIzqS -= 1.0f;
			printf("Rotacion rod Izq: %f", rotRodIzq);
			if (rotRodIzqS > 40.0f)
			{
				auxMovTimmy = true;
			}
		}
		if (auxMovTimmy)
		{
			rotRodDer = 0.0f;
			rotRodIzqS -= 1.0f;
			rotRodDerS += 1.0f;
			rotRodIzq -= 0.5f;
			rotBraDerS -= 1.0f;
			rotBraIzqS += 1.0f;
			printf("Rotacion rod Der: %f", rotRodDer);
			if (rotRodDerS > 40.0f)
			{
				auxMovTimmy = false;
			}
		}
	}

}

