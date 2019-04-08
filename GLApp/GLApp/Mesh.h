#pragma once

#include <iostream>
#include <vector>
 
#include "Vertex.h"
#include "Primitives.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"

class Mesh
{
private:
	unsigned nrOfVertices;
	unsigned nrOfIndices;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 ModelMatrix;

	void initVAO(Primitive *primitive)
	{
		//Set variables
		this->nrOfVertices = primitive->getNrOfVertices();
		this->nrOfIndices = primitive->getNrOfIndices();

		//GENERATE VAO AND BIND
		//Criando espaço para conter parâmetros dos Vertex Shaders
		glGenVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		//GENERATE VBO AND BIND AND SEND DATA
		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO); //Criando espaço na memória da GPU
		glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(Vertex), primitive->getVertices(), GL_STATIC_DRAW);


		//GEN EBO AND BIND AND SEND DATA
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices * sizeof(GLuint), primitive->getIndices(), GL_STATIC_DRAW);

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
		//Normal
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);

		//BIND VAO 0
		glBindVertexArray(0);
	}


	/*VAO, VBO, EBO -->VBO: Cria um espaço de memoria na GPU para armazenar um grande numero de vertex data
	Dessa forma, o OpenGL nao precisa enviar um vertice por vez para a GPU, melhorando o desempenho
	VAO: Cria um espaço de memória onde serão armazenados os parametros para desenhar na tela como ponteiros para VBO*/
	void initVAO(Vertex *vertexArray, const unsigned &nrOfVertices, GLuint *indexArray, const unsigned &nrOfIndices)
	{
		//Set variables
		this->nrOfVertices = nrOfVertices;
		this->nrOfIndices = nrOfIndices;

		//GENERATE VAO AND BIND
		//Criando espaço para conter parâmetros dos Vertex Shaders
		glGenVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		//GENERATE VBO AND BIND AND SEND DATA
		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO); //Criando espaço na memória da GPU
		glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(Vertex), vertexArray, GL_STATIC_DRAW);


		//GEN EBO AND BIND AND SEND DATA
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices * sizeof(GLuint), indexArray, GL_STATIC_DRAW);

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
		//Normal
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);

		//BIND VAO 0
		glBindVertexArray(0);
	}

	void updateUniforms(Shader *shader)
	{
		shader->setMat4fv(this->ModelMatrix, "ModelMatrix");
	}

	void updateModelMatrix()
	{
		this->ModelMatrix = glm::mat4(1.f);
		this->ModelMatrix = glm::translate(this->ModelMatrix, this->position);
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
		this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);
	}

public:
	Mesh(Vertex *vertexArray, const unsigned &nrOfVertices, 
		GLuint *indexArray, const unsigned &nrOfIndices,
		glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f)
	){
		this->position = position; 
		this->rotation = rotation;
		this->scale = scale;
		this->initVAO(vertexArray, nrOfVertices, indexArray, nrOfIndices);
		this->updateModelMatrix();

	}

	Mesh(Primitive *primitive,
		glm::vec3 position = glm::vec3(0.f), 
		glm::vec3 rotation = glm::vec3(0.f), 
		glm::vec3 scale = glm::vec3(1.f)
	) {
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
		this->initVAO(primitive);
		this->updateModelMatrix();

	}

	~Mesh()
	{
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);
		glDeleteBuffers(1, &this->EBO);
	}

	//Accessors

	//Modefiers
	void setPosition(const glm::vec3 position) {
		this->position = position;
	}

	void setRotation(const glm::vec3 rotation) {
		this->rotation = rotation;
	}

	void setScale(const glm::vec3 scale) {
		this->rotation = scale;
	}

	//Functions

	void move(const glm::vec3 position) {
		this->position += position;
	}

	void rotate(const glm::vec3 rotation) {
		this->rotation += rotation;

		if (this->rotation.x == 360 || this->rotation.x == -360)
			this->rotation.x = 0;
	}

	void scaleUp(const glm::vec3 scale) {
		this->scale += scale;
	}


	void update() {


	}

	void render(Shader *shader) {
		//Update uniforms
		this->updateModelMatrix();
		this->updateUniforms(shader);

		shader->use();

		//Bind VAO
		glBindVertexArray(this->VAO);

		//Draw
		glDrawElements(GL_TRIANGLES, this->nrOfIndices, GL_UNSIGNED_INT, 0);
	}

};