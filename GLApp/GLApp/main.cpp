#include "libs.h"

/*Fecha a janela utilizando o ESC*/
void updateInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

/**Metodo utilizado para adequar a resolucao do framebuffer*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

/* Carregamento dos shaders na GPU*/
bool loadShaders(GLuint &program) { //GLuint >>> unsigned int comum

	bool loadSuccess = true;
	char infoLog[512]; 
	GLint success; //GLint >>> int comum

	std::string temp = "";
	std::string src = "";

	std::ifstream in_file;

	//Vertex
	in_file.open("vertex_core.glsl"); //abrindo arquivo de Vertex Shaders

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

	//Verifica o que est� acontecendo no VertexShader e retorna o resultado na var Success
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
	//Finalizando os Vertex Shader e Fragment Shader 
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
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "JANELINHA", NULL, NULL);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	//glViewport(0, 0, framebufferWidth, framebufferHeight);

	glfwMakeContextCurrent(window);

	//INIT GLEW

	glewExperimental = GL_TRUE;

	//Error

	if (glewInit() != GLEW_OK) {
		std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
		glfwTerminate();
	}

	//OPENGL OPTIONS (op��es necess�rias antes de renderizar alguma coisa
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);//Counter Clock Wise --> Diz ao OpenGL qual face dever� ser desenhada de acordo com a ordem dos vetores

	glEnable(GL_BLEND); // --> habilita a mistura de cores
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // ->> Desenhar poligonos com cores

	//SHADER INIT
	GLuint core_program;
	if (!loadShaders(core_program)) {
		glfwTerminate();
	}
	


	//MAIN LOOP
	while (!glfwWindowShouldClose(window)) {

		//UPDATE INPUT
		glfwPollEvents();



		//UPDATE
		updateInput(window);


		//DRAW
		//Clear
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


		//Draw

		//End Draw
		glfwSwapBuffers(window);
		glFlush();
	}





	//END PROGRAM
	glfwTerminate();


	return 0;
}
