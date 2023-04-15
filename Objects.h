#pragma once


#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Mesh.h"

#include <iostream>
#include <string>


class Objects
{
public:
	Objects();


	void initilizeMeshList();
	void createObjects();
	

	std::vector<Mesh*> meshList;

	~Objects();

private:

	void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount,
		unsigned int vLength, unsigned int normalOffset);

};

