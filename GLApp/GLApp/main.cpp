#include "libs.h"

/**Metodo utilizado para adequar a resolucao do framebuffer*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void updateInput(GLFWwindow* window, Mesh &mesh)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		mesh.move(glm::vec3(0.f, 0.f, 0.005f));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		mesh.move(glm::vec3(0.f, 0.f, -0.005f));
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
}

int main() {

	//INIT GLFW

	glfwInit();

	//CREATE WINDOW
	const int SCR_WIDTH = 640;
	const int SCR_HEIGHT = 480;
	int framebufferWidth = 0;
	int framebufferHeight = 0;

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//versao do opengl
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "JANELINHA", NULL, NULL);

	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight); 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//glViewport(0, 0, framebufferWidth, framebufferHeight);

	glfwMakeContextCurrent(window);

	//INIT GLEW

	glewExperimental = GL_TRUE;

	//Error

	if (glewInit() != GLEW_OK) {
		std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
		glfwTerminate();
	}
	
	//OPENGL OPTIONS
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	//SHADER INIT
	Shader core_program("vertex_core.glsl", "fragment_core.glsl");

	//MODEL MESH
	Quad tempQuad = Quad();
	Mesh test(&tempQuad,
		glm::vec3(0.f),
		glm::vec3(0.f),
		glm::vec3(1.f)
	);




	//INCIANDO TEXTURAS
	//NAO SE PODE LIGAR 2 TEXTURAS A 1 TEXTURE UNIT
	Texture texture0("Images/catinho.png", GL_TEXTURE_2D, 0);
	Texture texture1("Images/container.png", GL_TEXTURE_2D, 1);

	//MATERIAL 0
	Material material0(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), texture0.getTextureUnit(), texture1.getTextureUnit());


	//Criando Matriz View
	glm::vec3 cameraPos(0.f, 0.f, 1.f);
	glm::vec3 worldUp(0.f, 1.f, 0.f);
	glm::vec3 camFront(0.f, 0.f, -1.f);
	glm::mat4 ViewMatrix(1.f);
	ViewMatrix = glm::lookAt(cameraPos, cameraPos + camFront, worldUp);

	//Matriz de projecao
	float fov = 90.f;
	float nearPlane = 0.1f;
	float farPlane = 1000.f; //Distancia que o desenho sera renderizado
	glm::mat4 ProjectionMatrix(1.f);

	ProjectionMatrix = glm::perspective(
			glm::radians(fov), 
			static_cast<float>(framebufferWidth) / framebufferHeight, 
			nearPlane,
		    farPlane
	);

	//LIGHTS
	glm::vec3 lightPos0(0.f, 0.f, 1.f);

	//INIT UNIFORMS
	core_program.setMat4fv(ViewMatrix, "ViewMatrix");
	core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");

	core_program.setVec3f(lightPos0, "lightPos0");
	core_program.setVec3f(cameraPos, "cameraPos");

	core_program.unuse();
	//END OF INIT MATRICES

	//MAIN LOOP
	while (!glfwWindowShouldClose(window)) {

		//UPDATE INPUT
		glfwPollEvents();
		updateInput(window, test);

		//UPDATE

		//DRAW
		//Clear
		glClearColor(0.f, 0.f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//Update uniforms (dados enviados da CPU pra GPU)
		core_program.set1i(texture0.getTextureUnit(), "texture0");
		core_program.set1i(texture1.getTextureUnit(), "texture1");
		material0.sendToShader(core_program);


		
		//Update Frame Buffer Size and projection matrix
		glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
		
		//Perspectiva da camera
		ProjectionMatrix = glm::perspective(
			glm::radians(fov),
			static_cast<float>(framebufferWidth) / framebufferHeight,
			nearPlane,
			farPlane
		);

		core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");

		//Use a program
		core_program.use();
			
		//Activate Texture
		texture0.bind();
		texture1.bind();

		//Draw
		test.render(&core_program);

		//End Draw
		glfwSwapBuffers(window);
		glFlush();

		//Desativando a textura
		glBindVertexArray(0); 
		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);

	}



	//END PROGRAM
	glfwDestroyWindow(window);
	glfwTerminate();

	//Delete VAO and Buffers


	return 0;
}
