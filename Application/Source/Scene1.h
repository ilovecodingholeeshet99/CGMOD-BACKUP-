#ifndef SCENE_1_H
#define SCENE_1_H
#include "MatrixStack.h"
#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"

class Scene1 : public Scene
{
public:
	enum GEOMETRY_TYPE
	{
		GEO_AXES, // x, z ,y
		GEO_QUAD, 
		GEO_WHITEQUAD,
		GEO_RANDQUADCOLOR,
		GEO_LANDQUAD, // World space
		GEO_CUBE,
		GEO_GYMCUBE,
		GEO_REDCUBE,
		GEO_GREENCUBE,
		GEO_BLACKCUBE, 
		GEO_SATANGRAYCUBE,
		GEO_ORANGECUBE, 
		GEO_FLOORCOLOR, 
		GEO_SPHERE,
		GEO_GREENSPHERE,
		GEO_BLUEGREENSPHERE,
		GEO_REDSPHERE,
		GEO_RANDSPHERECOLOR,
		GEO_ORANGESPHERE,
		GEO_BLUECUBE,
		GEO_CIRCLE,
		GEO_CONE,
		GEO_GREENCONE, 
		GEO_CYLINDER,
		GEO_BROWNCYLINDER, 
		GEO_BLACKCYLINDER,
		GEO_REDCYLINDER,
		GEO_BLUECYLINDER,
		NUM_GEOMETRY,
	};

	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_TOTAL,
	};

	Scene1();
	~Scene1();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	void HandleKeyPress();

	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];


	// defining the movement and position variables so can move along z axis
	Vector3 moveZ;
	Vector3 planePos;
	Camera camera;

	// defining the movement and position variables so can move along y axis
	float rotationY; // current rotation in degrees - scalar
	float rotationSpeed; // speed to rotate in degrees – scalar

	// define scale and scale size variables so that can increase size of model
	float scaleY;
	float scaleSize;
	
	float swayTree;
	float rotateswayTree;
	float swingingPunchBag;
	float topBallfallX;
	float topBallfallY;
	float greenBallX;
	float greenBallY;
	float bouncingBalls;
	float movingTreadmill;
	float treadmillSpeed;
	float aroundtheWorldleft;
	float aroundtheWorldright;
	bool isLeft;
	bool isRight;
	float floatingDumbbellsUP;
	float scaleDumbbell;
	float floorWeightsY;
	float movecarForward;
	float rotatecarRight;
	float movecarminusX;
	float rotatecarRight2nd;
	float movecar3rd;
	float rotatecarRight3rd;
	float movecar4th;
	float rotatecarRight4th;
	float movecar5th;
	float rotatecarRight5th;
	float movecar6th;
	float wheels360;
	float fireworks;
	bool sprout;
	float topcarRotate;
	float helicopterCar;

	/*float rotateWeightsX;
	float rotateWeightsX2;
	float translateWeightsZ;*/

	// define the matrix stacks required
	MS modelStack, viewStack, projectionStack;
	
	
};

#endif