#include "libs.h"

Vertex vertices[] =
{
	//Position						    //Color							//Texcoord
	glm::vec3(-0.5f, 0.5f, 0.f),		glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.f, 1.f), //indice 0
	glm::vec3(-0.5f, -0.5f, 0.f),		glm::vec3(0.f, 1.f, 0.f),		glm::vec2(0.f, 0.f), //indice 1
	glm::vec3(0.5f, -0.5f, 0.f),		glm::vec3(0.f, 0.f, 1.f),		glm::vec2(1.f, 0.f), //indice 2
	glm::vec3(0.5f, 0.5f, 0.f),	    	glm::vec3(1.f, 1.f, 0.f),		glm::vec2(1.f, 1.f), //indice 3
};
unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[] =
{
	0, 1, 2, // Triangulo 1
	0, 2, 3 // Triangulo 2
};
unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

/**Metodo utilizado para adequar a resolucao do framebuffer*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

bool loadShaders(GLuint &program) {

	bool loadSuccess = true;
	char infoLog[512];
	GLint success;

	std::string temp = "";
	std::string src = "";

	std::ifstream in_file;

	//Vertex
	in_file.open("vertex_core.glsl"); //Arquivo onde esta definido o Vertex Shader

	if (in_file.is_open()) {
		while (std::getline(in_file, temp))
			src += temp + "\n";

	}
	else
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_OPEN_VERTEX_FILE" << "\n";

	in_file.close();



	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);  //CRIA O VERTEX SHADER NO BACKGROUND
	const GLchar* vertSrc = src.c_str();
	glShaderSource(vertexShader, 1, &vertSrc, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_COMPILE_VERTEX_SHADER" << "\n";
		std::cout << infoLog << "\n";
		
	}


	temp = "";
	src = "";

	//Fragment 
	in_file.open("fragment_core.glsl");

	if (in_file.is_open()) {
		while (std::getline(in_file, temp))
			src += temp + "\n";
	}
	else {
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_OPEN_FRAGMENT_FILE" << "\n";
		loadSuccess = false;	
	}
	in_file.close();



	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);  //CRIA O VERTEX SHADER NO BACKGROUND
	const GLchar* fragSrc = src.c_str();
	glShaderSource(fragmentShader, 1, &fragSrc, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_COMPILE_FRAGMENT_SHADER" << "\n";
		std::cout << infoLog << "\n";
		loadSuccess = false;

	}

	//Program
	program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_LINK_PROGRAM" << "\n";
		std::cout << infoLog << "\n";
		loadSuccess = false;
	}




	//End
	glUseProgram(0);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	return loadSuccess;

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

	glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);


	//SHADER INIT
	GLuint core_program;
	if (!loadShaders(core_program)) {
		glfwTerminate();
	}
	
	//MODEL


	//VAO, VBO, EBO -->VBO: Cria um espaço de memoria na GPU para armazenar um grande numero de vertex data
	//Dessa forma, o OpenGL nao precisa enviar um vertice por vez para a GPU, melhorando o desempenho
	//VAO: Cria um espaço de memória onde serão armazenados os parametros para desenhar na tela como ponteiros para VBO


	//GENERATE VAO AND BIND
	//Criando espaço para conter parâmetros dos Vertex Shaders
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//GENERATE VBO AND BIND AND SEND DATA
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Criando espaço na memória da GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	//GEN EBO AND BIND AND SEND DATA
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//SET VERTEXATTRIBPOINTERS AND ENABLE (ASSEMBLY INPUT) 
	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);	

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);

	//Texcoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(2);

	//BIND VAO 0
	glBindVertexArray(0);

	//TEXTURE 0
	int image_width0 = 0;
	int image_height0 = 0;
	unsigned char* image0 = SOIL_load_image("Images/catinho.png", &image_width0, &image_height0, NULL, SOIL_LOAD_RGBA);

	GLuint texture0; //ID da textura
	glGenTextures(1, &texture0);
	glBindTexture(GL_TEXTURE_2D, texture0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	
	if (image0)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width0, image_height0, 0, GL_RGBA, GL_UNSIGNED_BYTE, image0);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR:TEXTURE_LOADING_FAILED" << "\n";
	}

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image0); //Apaga qualquer imagem carregada na memoria
	//FIM DA TEXTURA0


	//TEXTURE 1
	//NAO SE PODE LIGAR 2 TEXTURAS A 1 UNIDADE DE TEXTURA
	int image_width1 = 0;
	int image_height1 = 0;
	unsigned char* image1 = SOIL_load_image("Images/container.png", &image_width1, &image_height1, NULL, SOIL_LOAD_RGBA);

	GLuint texture1; //ID da textura
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	if (image1)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width1, image_height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR:TEXTURE_LOADING_FAILED" << "\n";
	}

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image1); //Apaga qualquer imagem carregada na memoria
	//FIM da textura1





	//INIT MATRIZES
	//Criando Model Matrix
	glm::mat4 ModelMatrix(1.f);

	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.f, 0.0f, 0.f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.f));

	//Criando Matriz View
	glm::vec3 camPos(0.f, 0.f, 1.f);
	glm::vec3 worldUp(0.f, 1.f, 0.f);
	glm::vec3 camFront(0.f, 0.f, -1.f);
	glm::mat4 ViewMatrix(1.f);
	ViewMatrix = glm::lookAt(camPos, camPos + camFront, worldUp);

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


	glUseProgram(core_program);
	
	glUniformMatrix4fv(glGetUniformLocation(core_program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(core_program, "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(core_program, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

	glUseProgram(0);
	//END OF INIT MATRICES

	//MAIN LOOP
	while (!glfwWindowShouldClose(window)) {

		//UPDATE INPUT
		glfwPollEvents();


		//UPDATE

		//DRAW
		//Clear
		glClearColor(0.f, 0.f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//Use a program
		glUseProgram(core_program);

		//Update uniforms (dados enviados da CPU pra GPU)
		glUniform1i(glGetUniformLocation(core_program, "texture0"), 0);
		glUniform1i(glGetUniformLocation(core_program, "texture1"), 1);


		//Move, Scale and Rotate, funciona em unidades por segundo (Matriz Model)
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.f, 0.f, 0.f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.01f), glm::vec3(0.f, 1.f, 0.f)); 
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.f));

		glUniformMatrix4fv(glGetUniformLocation(core_program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));

		glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
		
		//Perspectiva da camera

		ProjectionMatrix = glm::perspective(
			glm::radians(fov),
			static_cast<float>(framebufferWidth) / framebufferHeight,
			nearPlane,
			farPlane
		);


		glUniformMatrix4fv(glGetUniformLocation(core_program, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

		//Activate Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);



		//Bind Vertex ARRAY Object
		glBindVertexArray(VAO);

		//Draw
		//glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);
		glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);

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
	glfwTerminate();


	return 0;
}
