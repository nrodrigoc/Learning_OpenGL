#pragma once


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include "Vertex.h"


class Primitive
{
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;


public:
	Primitive() {

	}

	virtual ~Primitive() {

	}

	//Functions
	void set(const Vertex *vertices, const unsigned nrOfVertices,
		     const GLuint *indices, const unsigned nrOfIndices)
	{
		for (size_t i = 0; i < nrOfVertices; i++) {
			this->vertices.push_back(vertices[i]);
		}
	
		for (size_t i = 0; i < nrOfIndices; i++) {
			this->indices.push_back(indices[i]);
		}

	}


	inline Vertex *getVertices() {
		return this->vertices.data();
	}
	inline GLuint *getIndices() {
		return this->indices.data();
	}
	inline const unsigned getNrOfVertices() { return this->vertices.size(); }
	inline const unsigned getNrOfIndices() { return this->indices.size(); }


};

class Triangle : public Primitive
{
public:
	Triangle()
		: Primitive()
	{
		Vertex vertices[] =
		{
			//Position						    //Color							//Texcoord				//Normals
			glm::vec3(-0.5f, 0.5f, 0.f),		glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.f, 1.f),  	glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(-0.5f, -0.5f, 0.f),		glm::vec3(0.f, 1.f, 0.f),		glm::vec2(0.f, 0.f), 	glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f, -0.5f, 0.f),		glm::vec3(0.f, 0.f, 1.f),		glm::vec2(1.f, 0.f), 	glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f, 0.5f, 0.f),	    	glm::vec3(1.f, 1.f, 0.f),		glm::vec2(1.f, 1.f), 	glm::vec3(0.f, 0.f, 1.f),
		};
		unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] =
		{
			0, 1, 2 // Triangulo
		};
		unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, nrOfVertices, indices, nrOfIndices);

	}
};

//Quad esta herdando de Primitive
class Quad : public Primitive
{
public:
	Quad()
		: Primitive()
	{
		Vertex vertices[] =
		{
			//Position						    //Color							//Texcoord				//Normals
			glm::vec3(-0.5f, 0.5f, 0.f),		glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.f, 1.f),  	glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(-0.5f, -0.5f, 0.f),		glm::vec3(0.f, 1.f, 0.f),		glm::vec2(0.f, 0.f), 	glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f, -0.5f, 0.f),		glm::vec3(0.f, 0.f, 1.f),		glm::vec2(1.f, 0.f), 	glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f, 0.5f, 0.f),	    	glm::vec3(1.f, 1.f, 0.f),		glm::vec2(1.f, 1.f), 	glm::vec3(0.f, 0.f, 1.f)

		};
		unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] =
		{
			0, 1, 2, // Triangulo 1
			0, 2, 3, // Triangulo 2
		};
		unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, nrOfVertices, indices, nrOfIndices);

	}
};

/*
class Cube : public Primitive
{
public:
	Cube()
		: Primitive()
	{
		Vertex vertices[] =
		{
			//Position						    //Color							//Texcoord				//Normals
			glm::vec3(-0.5f, 0.5f, 0.f),		glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.f, 1.f),  	glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(-0.5f, -0.5f, 0.f),		glm::vec3(0.f, 1.f, 0.f),		glm::vec2(0.f, 0.f), 	glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f, -0.5f, 0.f),		glm::vec3(0.f, 0.f, 1.f),		glm::vec2(1.f, 0.f), 	glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f, 0.5f, 0.f),	    	glm::vec3(1.f, 1.f, 0.f),		glm::vec2(1.f, 1.f), 	glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(-0.5f, 0.5f, -1.f),		glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.f, 1.f),  	glm::vec3(0.f, 0.f, -1.f),
			glm::vec3(-0.5f, -0.5f, -1.f),		glm::vec3(0.f, 1.f, 0.f),		glm::vec2(0.f, 0.f), 	glm::vec3(0.f, 0.f, -1.f),
			glm::vec3(0.5f, -0.5f, -1.f),		glm::vec3(0.f, 0.f, 1.f),		glm::vec2(1.f, 0.f), 	glm::vec3(0.f, 0.f, -1.f),
			glm::vec3(0.5f, 0.5f, -1.f),		glm::vec3(1.f, 1.f, 0.f),		glm::vec2(1.f, 1.f), 	glm::vec3(0.f, 0.f, -1.f),


		};
		unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] =
		{
			0, 1, 2, 0, 2, 3, // Frente
			6, 5, 4, 7, 6, 4, // Costa
			4, 5, 1, 4, 1, 0, // Direita
			3, 2, 6, 3, 6, 7, // Esquerda
			4, 0, 3, 4, 3, 7, // Cima
			1, 5, 6, 1, 6, 2  // Baixo
		};
		unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, nrOfVertices, indices, nrOfIndices);

	}
};*/