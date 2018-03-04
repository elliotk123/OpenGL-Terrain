#ifndef TERRAIN_H
#define TERRAIN_H
#include "section.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cameraZ.h>

#include <vector>
class terrrain {
private:
	int _baseVertsPerSection;
	float _distancePerWorldSpace;
	int _sectionX;
	int _sectionY;
	int[3] _lodDistances;
	section*[29] _loadedSections;
	unsigned int* indices[3];
	unsigend int EBOs[3]
	FractalNoise _noise;
public:
	terrain(int baseVertsPerSection, float distancePerWorldSpace, int lodDistances[3]);
	void update(const glm::vec3 position);
	int lod(float distance); 
	void display(Camera &camera, float maxViewDistance, float minViewDistance, float distancePerWorldSpace, int screenWidth, int screenHeight, Shader& shader);
	
};
terrain::terrain(int baseVertsPerSection, float distanceperWorldSpace, int lodDistances[3], int sectionCoords[2]) {
	_baseVertsPerSection = baseVertsPerSection;
	_distancePerWorldSpace = distancePerWorldSpace;
	_lodDistances = lodDistances;
	_sectionX = sectionCoords[0];
	_sectionY = sectionCoords[1];
	indices1 = createIndices(1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0])*len(indices[0]), indices[0], GL_STATIC_DRAW);
	indices2 = createIndices(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[1])*len(indices[1]), indices[1], GL_STATIC_DRAW);
	indices3 = createIndices(3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[2])*len(indices[2]), indices[2], GL_STATIC_DRAW);
	for (int i(0); i < 25; i++) {
		_loadedSections[i] = new section(0,i%5-2,i/5-2)
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
unsigned int* createIndices(int levelOfDetail) {
	float lodFactor = pow(2, 1 - levelOfDetail);
	int sectionLength = int(baseVertsPerSection*lodFactor);
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
	relSectionChangeX = int(position.x) / _baseVertsPerSection - _sectionX;
	relSectionChangeY = int(position.y) / _baseVertsPerSection - _sectionY;
	if (relSectionChangeX != 0 || relSectionChangeY != 0) {
		sectionX += relSectionChangeX;
		sectionY += relSectionChangeY;
		for (int i(0); i < len(_loadedSections); i++) {
			relSectionX = _loadedSections[i]->getOrigin()[0]) - _sectionX;
			relSectionY = _loadedSections[i]->getOrigin()[1]) - _sectionY;
			float relDistance = sqrt(pow(relSectionX, 2) + pow(relSectionY, 2));
			newLod = lod(relDistance);
			if (lod(relDistance) != _loadedSections[i]->getLod()) {
				if (newLod == 4) {
					delete _loadedSections[i];
					_loadedSections[i] = new section(3, _sectionX - relsectionX, _sectionY - relsectionY);
					_loadedSections[i]->setvertices(_basevertsPerSection, _distancePerWorldSpace, _noise);
					_loadedSections[i]->bindBuffers(EBO[2],indices[2] )
				}
				else {
					_loadedSections[i]->setLod(newLod);
					_loadedSections[i]->setVertices(_baseVertsPerSection, _distancePerWorldSpace, _noise);
					_loadedSections[i]->bindBuffers(EBO[newLod-1], indices[newLod-1])
				}
			}
		}
	}
}

void terrain::display(Camera &camera,float maxViewDistance, float minViewDistance, float distancePerWorldSpace, int screenWidth, int screenHeight, Shader& shader)) {
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








	//	float sectionLength = baseVertsPerSection*_distancePerWorldSpace 
	//	newSectionX = _sectionLength*round((position.x - (_loadedSections[4]->getOrigin())[0] )/ sectionLength);
	//	newSectionY = _sectionLength*round((position.y - (_loadedSections[4]->getOrigin())[1]) / sectionLength);
	//	newSectionOrigin[2] = { newSectionX,newSectionY };
	//	switch(newSectionOrigin) {
	//		case {0, 0} {
	//			break;
	//		}
	//		case{-1, 1} {
	//			//delete the sections that are no longer required
	//			delete  _loadedSections[2], [5], [6], [7], [8];
	//			//move the sections that are kept
	//			_loadedSections[4] = _loadedSections[0];
	//			_loadedSections[5] = _loadedSections[1];
	//			_loadedSections[7] = _loadedSections[3];
	//			_loadedSections[8] = _loadedSections[4];
	//			//create the new sections
	//			_loadedSections[0] = new section(1, _loadedSections[4]->getOrigin())[0] - sectionLength, _loadedSections[4]->getOrigin())[1] + sectionLength)
	//			_loadedSections[1] = new section(1, _loadedSections[4]->getOrigin())[0], _loadedSections[4]->getOrigin())[1] + sectionLength)
	//			_loadedSections[2] = new section(1, _loadedSections[4]->getOrigin())[0] + sectionLength, _loadedSections[4]->getOrigin())[1] + sectionLength)
	//			_loadedSections[3] = new section(1, _loadedSections[4]->getOrigin())[0] - sectionLength, _loadedSections[4]->getOrigin())[1])
	//			_loadedSections[6] = new section(1, _loadedSections[4]->getOrigin())[0] - sectionLength, _loadedSections[4]->getOrigin())[1] - sectionLength)
	//			break;
	//		}
	//		case{1, 1} {
	//			//delete the sections that are no longer required
	//			delete  _loadedSections[0], [3], [6], [7], [8];
	//			//move the sections that are kept
	//			_loadedSections[3] = _loadedSections[1];
	//			_loadedSections[4] = _loadedSections[2];
	//			_loadedSections[6] = _loadedSections[4];
	//			_loadedSections[7] = _loadedSections[5];
	//			//create the new sections
	//			_loadedSections[0] = new section(1, _loadedSections[4]->getOrigin())[0] - sectionLength, _loadedSections[4]->getOrigin())[1] + sectionLength);
	//			_loadedSections[1] = new section(1, _loadedSections[4]->getOrigin())[0], _loadedSections[4]->getOrigin())[1] + sectionLength);
	//			_loadedSections[2] = new section(1, _loadedSections[4]->getOrigin())[0] + sectionLength, _loadedSections[4]->getOrigin())[1] + sectionLength);
	//			_loadedSections[5] = new section(1, _loadedSections[4]->getOrigin())[0] + sectionLength, _loadedSections[4]->getOrigin())[1]);
	//			_loadedSections[8] = new section(1, _loadedSections[4]->getOrigin())[0] + sectionLength, _loadedSections[4]->getOrigin())[1] - sectionLength);
	//			break;
	//		}
	//		case{1, -1} {
	//			//delete the sections that are no longer required
	//			delete  _loadedSections[0],[1],[2], [3], [6];
	//			//move the sections that are kept
	//			_loadedSections[0] = _loadedSections[4];
	//			_loadedSections[1] = _loadedSections[5];
	//			_loadedSections[3] = _loadedSections[7];
	//			_loadedSections[4] = _loadedSections[8];
	//			//create the new sections
	//			_loadedSections[2] = new section(1, _loadedSections[4]->getOrigin())[0] + sectionLength, _loadedSections[4]->getOrigin())[1] + sectionLength);
	//			_loadedSections[5] = new section(1, _loadedSections[4]->getOrigin())[0] + sectionLength, _loadedSections[4]->getOrigin())[1]);
	//			_loadedSections[6] = new section(1, _loadedSections[4]->getOrigin())[0] + sectionLength, _loadedSections[4]->getOrigin())[1] - sectionLength);
	//			_loadedSections[7] = new section(1, _loadedSections[4]->getOrigin())[0], _loadedSections[4]->getOrigin())[1] - sectionlength);
	//			_loadedSections[8] = new section(1, _loadedSections[4]->getOrigin())[0] - sectionLength, _loadedSections[4]->getOrigin())[1] - sectionLength);
	//			break;
	//		}
	//		case{-1, -1} {
	//			//delete the sections that are no longer required
	//			delete  _loadedSections[0], [1], [2], [5], [8];
	//			//move the sections that are kept
	//			_loadedSections[1] = _loadedSections[3];
	//			_loadedSections[2] = _loadedSections[4];
	//			_loadedSections[4] = _loadedSections[6];
	//			_loadedSections[5] = _loadedSections[7];
	//			//create the new sections
	//			_loadedSections[0] = new section(1, _loadedSections[4]->getOrigin())[0] - sectionLength, _loadedSections[4]->getOrigin())[1] + sectionLength);
	//			_loadedSections[3] = new section(1, _loadedSections[4]->getOrigin())[0] - sectionLength, _loadedSections[4]->getOrigin())[1]);
	//			_loadedSections[6] = new section(1, _loadedSections[4]->getOrigin())[0] - sectionLength, _loadedSections[4]->getOrigin())[1] - sectionLength);
	//			_loadedSections[7] = new section(1, _loadedSections[4]->getOrigin())[0], _loadedSections[4]->getOrigin())[1] - sectionlength);
	//			_loadedSections[8] = new section(1, _loadedSections[4]->getOrigin())[0] + sectionLength, _loadedSections[4]->getOrigin())[1] - sectionLength);
	//			break;
	//		}
	//		case{0, 1} {
	//			//delete the sections that are no longer required
	//			delete  _loadedSections [6], [7], [8];
	//			//move the sections that are kept
	//			_loadedSections[8] = _loadedSections[5];
	//			_loadedSections[7] = _loadedSections[4];
	//			_loadedSections[6] = _loadedSections[3];
	//			_loadedSections[5] = _loadedSections[2];
	//			_loadedSections[4] = _loadedSections[1];
	//			_loadedSections[3] = _loadedSections[0];
	//			//create the new sections
	//			_loadedSections[0] = new section(1, _loadedSections[4]->getOrigin())[0] - sectionLength, _loadedSections[4]->getOrigin())[1] + sectionLength);
	//			_loadedSections[1] = new section(1, _loadedSections[4]->getOrigin())[0], _loadedSections[4]->getOrigin())[1] + sectionLength);
	//			_loadedSections[2] = new section(1, _loadedSections[4]->getOrigin())[0] + sectionLength, _loadedSections[4]->getOrigin())[1] + sectionLength);
	//			break;
	//		}
	//		case{0, -1} {
	//			//delete the sections that are no longer required
	//			delete  _loadedSections[0], [1], [2];
	//			//move the sections that are kept
	//			_loadedSections[0] = _loadedSections[3];
	//			_loadedSections[1] = _loadedSections[4];
	//			_loadedSections[2] = _loadedSections[5];
	//			_loadedSections[3] = _loadedSections[6];
	//			_loadedSections[4] = _loadedSections[7];
	//			_loadedSections[5] = _loadedSections[8];
	//			//create the new sections
	//			_loadedSections[6] = new section(1, _loadedSections[4]->getOrigin())[0] - sectionLength, _loadedSections[4]->getOrigin())[1] - sectionLength);
	//			_loadedSections[7] = new section(1, _loadedSections[4]->getOrigin())[0], _loadedSections[4]->getOrigin())[1] - sectionLength);
	//			_loadedSections[8] = new section(1, _loadedSections[4]->getOrigin())[0] + sectionLength, _loadedSections[4]->getOrigin())[1] - sectionLength);
	//			break;
	//		}
	//		case{1, 0} {
	//			//delete the sections that are no longer required
	//			delete  _loadedSections[0], [3], [6];
	//			//move the sections that are kept
	//			_loadedSections[0] = _loadedSections[1];
	//			_loadedSections[3] = _loadedSections[4];
	//			_loadedSections[6] = _loadedSections[7];
	//			_loadedSections[1] = _loadedSections[2];
	//			_loadedSections[4] = _loadedSections[5];
	//			_loadedSections[7] = _loadedSections[8];
	//			//create the new sections
	//			_loadedSections[2] = new section(1, _loadedSections[4]->getOrigin())[0] + sectionLength, _loadedSections[4]->getOrigin())[1] + sectionLength);
	//			_loadedSections[5] = new section(1, _loadedSections[4]->getOrigin())[0] + sectiionLength, _loadedSections[4]->getOrigin())[1] );
	//			_loadedSections[8] = new section(1, _loadedSections[4]->getOrigin())[0] + sectionLength, _loadedSections[4]->getOrigin())[1] - sectionLength);
	//			break;
	//		}
	//		case{-1, 0} {
	//			//delete the sections that are no longer required
	//			delete  _loadedSections[2], [5], [8];
	//			//move the sections that are kept
	//			_loadedSections[2] = _loadedSections[1];
	//			_loadedSections[5] = _loadedSections[4];
	//			_loadedSections[8] = _loadedSections[7];
	//			_loadedSections[1] = _loadedSections[0];
	//			_loadedSections[4] = _loadedSections[3];
	//			_loadedSections[7] = _loadedSections[6];
	//			//create the new sections
	//			_loadedSections[0] = new section(1, _loadedSections[4]->getOrigin())[0] - sectionLength, _loadedSections[4]->getOrigin())[1] + sectionLength);
	//			_loadedSections[3] = new section(1, _loadedSections[4]->getOrigin())[0] - sectiionLength, _loadedSections[4]->getOrigin())[1] );
	//			_loadedSections[6] = new section(1, _loadedSections[4]->getOrigin())[0] - sectionLength, _loadedSections[4]->getOrigin())[1] - sectionLength);
	//			break;
	//		}
	//	}
	//} 
	

#endif
