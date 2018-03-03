#ifndef SECTION_H
#def SECTION_H
#include <myheaders/noise/FractalNoise.h>
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
	void setLod(const lod) { lod = lod; }
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






#endif
