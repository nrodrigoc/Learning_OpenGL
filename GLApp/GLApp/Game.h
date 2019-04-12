#pragma once

#include "libs.h"

//ENUMERATIONS
enum shader_enums{ SHADER_CORE_PROGRAM = 0 };
enum texture_enum{ TEX_CATINHO = 0, TEX_CATINHO_SPECULAR, TEX_CONTAINER, TEX_CONTAINER_SPECULAR};
enum material_enum{ MAT_1 = 0};
enum mesh_enum{ MESH_QUAD = 0 };


class Game
{
private:
	//Variables
	//window
	GLFWwindow* window;
	const int SCR_WIDTH;
	const int SCR_HEIGHT;
	int framebufferWidth;
	int framebufferHeight;

	//OpenGL Context
	const int GL_VERSION_MAJOR;
	const int GL_VERSION_MINOR;


	//Matrices
	glm::mat4 ViewMatrix;
	glm::vec3 cameraPos;
	glm::vec3 worldUp;
	glm::vec3 camFront;

	glm::mat4 ProjectionMatrix;
	float fov;
	float nearPlane;
	float farPlane;

	//Shaders
	std::vector<Shader*> shaders;

	//Textures
	std::vector<Texture*> textures;

	//Materials
	std::vector<Material*> materials;

	//Meshes
	std::vector<Mesh*> meshes;

	//Lights
	std::vector<glm::vec3*> lights;

	//Private Functions
	void initGLFW();
	void initWindow(const char *title, bool resizable);
	void initGLEW();
	void initOpenGLOptions();
	void initMatrices();
	void initShaders();
	void initTextures();
	void initMaterials();
	void initMashes();
	void initLights();
	void initUniforms();

	void updateInput(GLFWwindow* window, Mesh &mesh);
	void updateUniforms();

//Static Variables

public:

	//Construtctor / Distructor
	Game(
		const char *title,
		const int SCR_WIDTH, const int SCR_HEIGHT,
		const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR,
		bool resizable
	);
	virtual ~Game();


	//Accessors	
	int getWindowShouldClose();

	//Modifiers
	void setWindowShouldClose();

	//Functions
	//void updateInput(GLFWwindow* window, Mesh &mesh);
	void update();
	void render();

	//Static Functions
	static void framebuffer_resize_callback(GLFWwindow* window, int width, int height);
};

