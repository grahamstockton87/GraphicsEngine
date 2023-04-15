#include "Objects.h"
#include "Mesh.h"
#include <iostream>

Objects::Objects() {
	
}


void Objects::calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount,
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

void Objects::createObjects()
{
	unsigned int indices[] = {
	0, 3, 1,
	1, 3, 2,
	2, 3, 0,
	0, 1, 2
	};

	GLfloat vertices[] = {
		//x      y     z,        u     v         nx   y         z
		-1.0f, -1.0f, 0.0f,     0.0f,  0.0f,   0.0f,  0.0f,  0.0f,
		0.0f, -1.0f, 1.0f,     0.5f,  0.0f,    0.0f,  0.0f,  0.0f,
		1.0f, -1.0f, 0.0f,     1.0f,  0.0f,    0.0f,  0.0f,  0.0f,
		0.0f, 1.0f, 0.0f,     0.5f,  1.0f,     0.0f,  0.0f,  0.0f
	};

	unsigned int CubeIndices[] = {

		1, 0, 2,//back
		1, 2, 3,

		3, 2, 6, //bottom
		3, 6, 7,

		5, 4, 6, //front
		5, 6, 7,

		1, 5, 7, //right
		1, 7, 3,

		1, 5, 0, //top
		0, 4, 5,

		0, 4, 6, //left
		0, 6, 2
	};



	GLfloat cubeVert[] = {

		-1.0f, 1.0f, 0.0f,		1.0f,  1.0f,	 0.0f,  0.0f,  0.0f,  //0 ,
		1.0f, 1.0f, 0.0f,		0.5f,  0.0f,	 0.0f,  0.0f,  0.0f,  //1
		-1.0f, -1.0f, 0.0f,		1.0f,  0.0f,	 0.0f,  0.0f,  0.0f,  //2 y
		1.0f, -1.0f, 0.0f,		1.0f,  1.0f,	 0.0f,  0.0f,  0.0f,  //3

		-1.0f, 1.0f, 1.0f,		0.0f,  0.0f,	 0.0f,  0.0f,  0.0f,  //4
		1.0f, 1.0f, 1.0f,		1.0f,  1.0f,	 0.0f,  0.0f,  0.0f,  //5
		-1.0f, -1.0f, 1.0f,		0.5f,  0.0f,	 0.0f,  0.0f,  0.0f,   //6
		1.0f, -1.0f, 1.0f,		0.5f,  1.0f,	 0.0f,  0.0f,  0.0f,   //7

	};

	unsigned int planeIndices[]{

		2, 1, 3,
		0, 1, 2
		

	};
	GLfloat planeVert[]{
		-1.0f, 0.0f, -1.0f,		0.0f,  1.0f,	 0.0f,  0.0f,  0.0f,  //0 ,
		-1.0f, 0.0f, 1.0f,		0.0f,  0.0f,	 0.0f,  0.0f,  0.0f,  //1
		1.0f, 0.0f, -1.0f,		1.0f,  1.0f,	 0.0f,  0.0f,  0.0f,  //2 y
		1.0f, 0.0f, 1.0f,		1.0f,  0.0f,	 0.0f,  0.0f,  0.0f,  //3
	};

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);
	calcAverageNormals(CubeIndices, 36, cubeVert, 64, 8, 5);
	calcAverageNormals(planeIndices, 6, planeVert, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	//Mesh *obj2 = new Mesh();
	//obj2->CreateMesh(cubeVert, CubeIndices, 64, 36);
	//obj2->CreateMesh(cubeVert, CubeIndices, 64, 36);
	//meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	//obj2->CreateMesh(cubeVert, CubeIndices, 64, 36);
	obj3->CreateMesh(planeVert, planeIndices, 32, 6);
	meshList.push_back(obj3);
}


Objects::~Objects()
{
}


