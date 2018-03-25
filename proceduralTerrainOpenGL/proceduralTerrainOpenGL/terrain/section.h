#ifndef SECTION_H
#define SECTION_H
#include "../noise/FractalNoise.h"
#include "../shader/shader_m.h"

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
class section {
private:
	int _lod; 
	int _sectionX;
	int _sectionY;
	float* _vertices;
	unsigned int VBO;
	unsigned int VAO;

public:
	section(int lod, int sectionX, int sectionY) {_lod = lod;_sectionX = sectionX;_sectionY = sectionY; glGenVertexArrays(1, &VAO); glGenBuffers(1, &VBO);
	}
	~section() { delete[] _vertices; glDeleteVertexArrays(1, &VAO);glDeleteBuffers(1, &VBO); std::cout << "Deleted section\n"; }
	void setVertices(const int baseVertsPerSection,const float distancePerWorldSpace,const FractalNoise &f);
	void setLod(int lod) { _lod = lod; }
	int getLod()const { return _lod; }
	int getOrigin()const { return { _sectionX, _sectionY }; }
	float* getVertices()const { return _vertices; }
	void bindBuffers(unsigned int &EBO, float indices);
};

void section::setVertices(const int baseVertsPerSection, const float distancePerWorldSpace, const FractalNoise &f) {
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
void section::bindBuffers(unsigned int &EBO,float indices) {
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices)*len(_vertives), _vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)*len(Indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void section::render(Shader &shader) {
	model = glm::translate(model, { _sectionX,_sectionY,0 });
	int modelLoc = glGetUniformLocation(shader.ID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//render terrain
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

	}

}




#endif
