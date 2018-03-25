#ifndef TERRAIN_H
#define TERRAIN_H
#include "section.h"

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../cameraZ.h"

#include <vector>
class terrain {
private:
	int _baseVertsPerSection;
	float _distancePerWorldSpace;
	int _sectionX;
	int _sectionY;
	int _lodDistances[3];
	section* _loadedSections[29];
	unsigned int* indices[3];
	unsigned int EBOs[3];
	FractalNoise &_noise;
public:
	terrain(const int baseVertsPerSection, const float distancePerWorldSpace, const int lodDistances[],int sectionCoords[],FractalNoise &f);
	void update(const glm::vec3 position);
	int lod(float distance); 
	void display(Camera &camera, float maxViewDistance, float minViewDistance, float distancePerWorldSpace, int screenWidth, int screenHeight, Shader& shader);
	unsigned int* createIndices(int levelOfDetail);
	
};









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
