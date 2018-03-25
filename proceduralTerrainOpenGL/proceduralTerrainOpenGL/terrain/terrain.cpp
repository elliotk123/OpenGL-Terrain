#include "terrain.h"

terrain::terrain(const int baseVertsPerSection,const float distancePerWorldSpace, const int lodDistances[], int sectionCoords[], FractalNoise &f) {
	_baseVertsPerSection = baseVertsPerSection;
	_distancePerWorldSpace = distancePerWorldSpace;
	_noise = f;
	for (int i{ 0 }; i < 3; i++) { _lodDistances[i] = lodDistances[i]; }
	_sectionX = sectionCoords[0];
	_sectionY = sectionCoords[1];
	indices[0] = createIndices(1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0])*sizeof(*indices[0]), indices[0], GL_STATIC_DRAW);
	indices[1] = createIndices(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[1])*sizeof(*indices[1]), indices[1], GL_STATIC_DRAW);
	indices[2] = createIndices(3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[2])*sizeof(*indices[2]), indices[2], GL_STATIC_DRAW);
	for (int i(0); i < 25; i++) {
		_loadedSections[i] = new section(0, i % 5 - 2, i / 5 - 2)
	}
	_loadedSections[25] = new section(0, -3, 0);
	_loadedSections[26] = new section(0, 3, 0);
	_loadedSections[27] = new section(0, 0, 3);
	_loadedSections[28] = new section(0, 0, -3);
}


int terrain::lod(float distance) {
	if (distance >= _lodDistances[2]) { return 4; }
	else if (distance >= _lodDistances[1]) { return 3; }
	else if (distance >= _lodDistances[0]) { return 2; }
	else { return 1; }

}
unsigned int* terrain::createIndices(int levelOfDetail) {
	float lodFactor = pow(2, 1 - levelOfDetail);
	int sectionLength = int(_baseVertsPerSection*lodFactor);
	const int numIndices = 6 * pow(sectionLength - 1, 2);
	unsigned int* indices = new unsigned int[numIndices];
	int index = 0;
	for (int i = 0; i < numIndices; i += 6) {
		if (index%sectionLength == sectionLength - 1) { index++; }
		indices[i] = index;//first triangle
		indices[i + 1] = index + 1;
		indices[i + 2] = index + sectionLength;

		indices[i + 3] = index + 1;//second triangle
		indices[i + 4] = index + sectionLength;
		indices[i + 5] = index + sectionLength + 1;
		index++;

	}
	return indices;
}
void terrain::update(const glm::vec3 position) {
	int relSectionChangeX = int(position.x) / _baseVertsPerSection - _sectionX;
	int relSectionChangeY = int(position.y) / _baseVertsPerSection - _sectionY;
	if (relSectionChangeX != 0 || relSectionChangeY != 0) {
		_sectionX += relSectionChangeX;
		_sectionY += relSectionChangeY;
		for (int i(0); i < len(_loadedSections); i++) {
			int relSectionX = _loadedSections[i]->getOrigin()[0]) - _sectionX;
			int relSectionY = _loadedSections[i]->getOrigin()[1]) - _sectionY;
			float relDistance = sqrt(pow(relSectionX, 2) + pow(relSectionY, 2));
			int newLod = lod(relDistance);
			if (lod(relDistance) != _loadedSections[i]->getLod()) {
				if (newLod == 4) {
					delete _loadedSections[i];
					_loadedSections[i] = new section(3, _sectionX - relsectionX, _sectionY - relsectionY);
					_loadedSections[i]->setVertices(_baseVertsPerSection, _distancePerWorldSpace, _noise);
					_loadedSections[i]->bindBuffers(EBO[2], indices[2])
				}
				else {
					_loadedSections[i]->setLod(newLod);
					_loadedSections[i]->setVertices(_baseVertsPerSection, _distancePerWorldSpace, _noise);
					_loadedSections[i]->bindBuffers(EBO[newLod - 1], indices[newLod - 1])
				}
			}
		}
	}
}

void terrain::display(Camera &camera, float maxViewDistance, float minViewDistance, float distancePerWorldSpace, int screenWidth, int screenHeight, Shader& shader)) {
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
int projectionLoc = glGetUniformLocation(shader.ID, "projection");
glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
//model
glm::mat4 model;
for (int i = 0; i < 29, i++) {
	_loadedSection->render(shader)

}
}
