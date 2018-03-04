#ifndef SECTION_H
#def SECTION_H
#include <noise/FractalNoise.h>
#include <shader/shader_m.h>
#include <cameraZ.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class section {
private:
	int _lod; 
	int _sectionX;
	int _sectionY;
	float* _vertices;

public:
	section(int lod, int sectionX, int sectionY) {_lod = lod;_sectionX = sectionX;_sectionY = sectionY;}
	~section() { delete[] vertices; std::cout << "Deleted section\n"; }
	void setVertices(const int baseVertsPerSection,const float distancePerWorldSpace,const FractalNoise f);
	void setLod(const lod) { _lod = lod; }
	int getLod()const { return _lod; }
	int getOrigin()const { return { _sectionX, _sectionY }; }
	float* getVertices()const { return _vertices; }
};

float* section::setVertices(int baseVertsPerSection, float distancePerWorldSpace, FractalNoise &f) {
	float lodFactor = pow(2, 1 - lod);
	const int numVerts = int(pow(baseVertsPerSection*lodFactor, 2));
	float* vertices = new float[numVerts * 3];
	for (int i = 0; i < numVerts * 3; i += 3) {//assign coordinates to each vertex 
		float x = ((i / 3) % int(baseVertsPerSection*lodFactor)) - baseVertsPerSection*lodFactor / 2.0f;
		float y = i / (baseVertsPerSection*lodFactor * 3) - baseVertsPerSection*lodFactor / 2.0f;
		vertices[i] = float(x) / lodFactor;
		vertices[i + 1] = float(y) / lodFactor;
		float temp = f.noise((vertices[i]  + originX*baseVertsPerSection)*distancePerWorldSpace, (vertices[i+1] + originY*baseVertsPerSection)*distancePerWorldSpace, 0.0f);
		vertices[i + 2] = std::fmax(temp, 0.3*temp);
		//vertices[i + 2] = 0.5*temp*(0.3*exp(-8 * temp) + exp(8 * temp)) / (cosh(8 * temp));//Jungle
		//vertices[i + 2] = 0.5*(0.3*temp*exp(-2 * temp) + (0.3*temp+1)*exp(2 * temp)) / (cosh(2 * temp));//Mooreland
		//vertices[i + 2] = temp*tanh(temp);
	}
} 

void render(unsigned int& EBO, unsigned int& VAO, unsigned int& VBO, Shader &shader, Camera &camera,
	float maxViewDistance, float minViewDistance,float distancePerWorldSpace, int screenWidth, int screenHeight) {
	glBindVertexArray(VAO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices)*len(_vertives), _vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)*numIndices, indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);




	shader.use();
	//set transformations
	//view
	glm::mat4 view;
	view = camera.GetViewMatrix();
	int viewLoc = glGetUniformLocation(myShader.ID, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	//projection
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(camera.Zoom), float(screenWidth) / float(screenHeight), minViewDistance / distancePerWorldSpace, maxViewDistance / distancePerWorldSpace);
	int projectionLoc = glGetUniformLocation(myShader.ID, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	//model
	glm::mat4 model;
	model = glm::translate(model, { _sectionX,_sectionY,0 });
	int modelLoc = glGetUniformLocation(myShader.ID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//render terrain
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

	}

}




#endif
