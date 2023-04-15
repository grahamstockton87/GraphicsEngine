#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>


#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Objects.h"
#include "Light.h"
#include "Material.h"

#include <iostream>
#include <string>



const float toRadians = 3.14159265f / 180.0f;
float rotate = 0;

//Inilize objects

Window mainWindow;


std::vector<Shader> shaderList;


Camera camera;
Objects object;

Texture woodTex;
Texture dirt;

Light mainLight;

Material shinyMaterial;
Material dullMaterial;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void up() {
	if (rotate < 360)
		rotate += 0.1;
	else
		rotate = 0;
}


int main()
{
	mainWindow = Window(2000, 1000);
	mainWindow.Initialise();

	object.createObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -80.0f, 0.0f, 20.0f, 0.1f);

	woodTex = Texture("Textures(PNG)/brick.png");
	woodTex.loadTexture();
	dirt = Texture("Textures(PNG)/metal.png");
	dirt.loadTexture();

	shinyMaterial = Material(5.0f, 25);
	dullMaterial = Material(1.0f, 10);

	mainLight = Light(0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, -1.0f, 1.0f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		unifromAmbientIntensity = 0, uniformAmbientColor = 0, uniformDirection = 0, uniformDiffuseIntensity, 
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f * toRadians, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	float y = 0.0f;
	rotate = 1;

	double previousTime = glfwGetTime();
	float frameCount = 0.0f;

	

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		// Measure speed
		double currentTime = glfwGetTime();
		frameCount++;
		// If a second has passed.
		if (currentTime - previousTime >= 1.0)
		{
			// Display the frame count here any way you want.
			//std::cout << frameCount << std::endl;

			frameCount = 0.0f;
			previousTime = currentTime;
		}

		y += 0.0005f;
		

		if (Camera::fov < 180) {
			projection = glm::perspective(Camera::fov * toRadians, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
		}
		else {
			Camera::fov = 179;
			projection = glm::perspective(Camera::fov * toRadians, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
		}

		GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
		deltaTime = now - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
		lastTime = now;


		// Get + Handle User Input
		glfwPollEvents();

		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformAmbientColor = shaderList[0].GetAmbientColorLocation();
		unifromAmbientIntensity = shaderList[0].GetAmbientIntensityLocation();
		uniformDirection = shaderList[0].GetDirectionLocation();
		uniformDiffuseIntensity = shaderList[0].GetDiffuseIntensityLocation();
		uniformEyePosition = shaderList[0].getEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].getSpecularIntensityLocation();
		uniformShininess = shaderList[0].getShininessLocation();

		mainLight.useLight(unifromAmbientIntensity, uniformAmbientColor, uniformDiffuseIntensity, uniformDirection);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		up();
		
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::rotate(model, rotate * toRadians, glm::vec3(0.0f, 0.1f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		woodTex.useTexture();
		dullMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
		object.meshList[0]->RenderMesh();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirt.useTexture();
		shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
		object.meshList[0]->RenderMesh();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(1.0f, 1.0f, -2.5f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 1.0f));		
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirt.useTexture();
		shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
		object.meshList[1]->RenderMesh();
		
		//Floor

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		woodTex.useTexture();
		shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
		object.meshList[1]->RenderMesh();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}