#include "Game.h"


// Private Functions
void Game::initGLFW()
{
	//INIT GLFW
	glfwInit();
	if (glfwInit() == GLFW_FALSE) {
		std::cout << "ERROR::GLFW_INIT_FAILED" << "\n";
		glfwTerminate();
	}

}

void Game::initWindow(
	const char* title,
	bool resizable
)
{
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GL_VERSION_MINOR);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); MAC OS

	this->window = glfwCreateWindow(this->SCR_WIDTH, this->SCR_HEIGHT, title, NULL, NULL);

	if (this->window == nullptr)
	{
		std::cout << "ERROR::GLFW_WINDOW_INIT_FAILED" << "\n";
		glfwTerminate();
	}

	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);
	glfwSetFramebufferSizeCallback(window, Game::framebuffer_resize_callback);
	//IMPORTANT WHITH PERSPECTIVE MATRIX!!!

	//glViewport(0, 0, framebufferWidth, framebufferHeight);

	glfwMakeContextCurrent(this->window); //IMPORTANT!!
}

void Game::initGLEW()
{
	//INIT GLEW
	glewExperimental = GL_TRUE;

	//Error

	if (glewInit() != GLEW_OK) {
		std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
		glfwTerminate();
	}
}

void Game::initOpenGLOptions()
{
	//OPENGL OPTIONS
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Game::initMatrices()
{
	this->ViewMatrix = glm::mat4(1.f);
	this->ViewMatrix = glm::lookAt(this->cameraPos, this->cameraPos + this->camFront, this->worldUp);

	this->ProjectionMatrix = glm::mat4(1.f);
	this->ProjectionMatrix = glm::perspective(
		glm::radians(this->fov),
		static_cast<float>(this->framebufferWidth) / this->framebufferHeight,
		this->nearPlane,
		this->farPlane
	);
}

void Game::initShaders()
{
	this->shaders.push_back(new Shader(this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR, 
		"vertex_core.glsl", "fragment_core.glsl"));
}

void Game::initTextures()
{
	//INCIANDO TEXTURAS
	//NAO SE PODE LIGAR 2 TEXTURAS A 1 TEXTURE UNIT
	this->textures.push_back(new Texture("Images/catinho.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Images/catinho_specular.png", GL_TEXTURE_2D));

	this->textures.push_back(new Texture("Images/container.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Images/container_specular.png", GL_TEXTURE_2D));
}

void Game::initMaterials()
{
	this->materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), 0, 1));
}

void Game::initMashes()
{
	this->meshes.push_back(
		new Mesh(
			&Quad(),
			glm::vec3(0.f),
			glm::vec3(0.f),
			glm::vec3(1.f)
		)
	);
}

void Game::initLights()
{
	this->lights.push_back(new glm::vec3(0.f, 0.f, 1.f));
}

void Game::initUniforms()
{
	//INIT UNIFORMS
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ViewMatrix, "ViewMatrix");
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ProjectionMatrix, "ProjectionMatrix");

	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(*this->lights[0], "lightPos0");
	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(this->cameraPos, "cameraPos");
}

void Game::updateUniforms()
{
	//Update uniforms
	this->materials[MAT_1]->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);

	//Update framebuffer size and projection matrix
	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);

	this->ProjectionMatrix = glm::perspective(
		glm::radians(this->fov),
		static_cast<float>(this->framebufferWidth) / this->framebufferHeight,
		this->nearPlane,
		this->farPlane
	);

	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");
}

//Construtctors / Distructors
Game::Game(
	const char *title,
	const int SCR_WIDTH, const int SCR_HEIGHT,
	const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR,
	bool resizable
)
	: SCR_WIDTH(SCR_WIDTH),
	SCR_HEIGHT(SCR_HEIGHT),
	GL_VERSION_MAJOR(GL_VERSION_MAJOR), 
	GL_VERSION_MINOR(GL_VERSION_MINOR)
{
	//Init Variables
	this->window = nullptr;
	this->framebufferWidth = this->SCR_WIDTH;
	this->framebufferHeight = this->SCR_HEIGHT;

	this->cameraPos = glm::vec3(0.f, 0.f, 1.f);
	this->worldUp = glm::vec3(0.f, 1.f, 0.f);
	this->camFront = glm::vec3(0.f, 0.f, -1.f);


	//Matriz de projecao
	this->fov = 90.f;
	this->nearPlane = 0.1f;
	this->farPlane = 1000.f; //Distancia que o desenho sera renderizado

	this->initGLFW();
	this->initWindow(title, resizable);
	this->initGLEW();
	this->initOpenGLOptions();
	this->initMatrices();
	this->initShaders();
	this->initTextures();
	this->initMaterials();
	this->initMashes();
	this->initLights();
	this->initUniforms();

	this->updateUniforms();
}


Game::~Game()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();

	for (size_t i = 0; i < this->shaders.size(); i++){
		delete this->shaders[i];
	}
	for (size_t i = 0; i < this->textures.size(); i++){
		delete this->textures[i];
	}
	for (size_t i = 0; i < this->materials.size(); i++){
		delete this->materials[i];
	}
	for (size_t i = 0; i < this->meshes.size(); i++){
		delete this->meshes[i];
	}
	for (size_t i = 0; i < this->lights.size(); i++) {
		delete this->lights[i];
	}
}


//Accessors
int Game::getWindowShouldClose()
{
	return glfwWindowShouldClose(this->window);
}

//Modifiers
void Game::setWindowShouldClose()
{
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}


//Functions 
void Game::update()
{
	//Update Inputs
	//this->updateInput();
	glfwPollEvents();
}

/*void Game::updateInput(GLFWwindow* window, Mesh &mesh)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		mesh.move(glm::vec3(0.f, 0.f, -0.005f));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		mesh.move(glm::vec3(0.f, 0.f, 0.005f));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		mesh.move(glm::vec3(-0.005f, 0.f, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		mesh.move(glm::vec3(0.005f, 0.f, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		mesh.rotate(glm::vec3(0.f, -0.08, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		mesh.rotate(glm::vec3(0.f, 0.08, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		mesh.scaleUp(glm::vec3(0.01f));
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		mesh.scaleUp(glm::vec3(-0.01f));
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}*/

void Game::render()
{
	//updateInput(window);

	//DRAW
	//Clear
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//Update uniforms (dados enviados da CPU pra GPU)
	this->updateUniforms();

	//Use a program
	this->shaders[SHADER_CORE_PROGRAM]->use();

	//Activate Texture
	this->textures[TEX_CATINHO]->bind(0);
	//this->textures[TEX_CATINHO_SPECULAR]->bind(1);
	//this->textures[TEX_CONTAINER]->bind();
	//this->textures[TEX_CONTAINER]->bind(0);

	//Draw
	this->meshes[MESH_QUAD]->render(this->shaders[SHADER_CORE_PROGRAM]);
	//test.rotate(glm::vec3(0.03f, 0.f, 0.f));

	//End Draw
	glfwSwapBuffers(window);
	glFlush();

	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

/**Metodo utilizado para adequar a resolucao do framebuffer*/
//Static functions
void Game::framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH)
{
	glViewport(0, 0, fbW, fbH);
};
