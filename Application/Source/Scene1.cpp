#include "Scene1.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Application.h"
#include "Mtx44.h"
#include "MeshBuilder.h"

Scene1::Scene1()
{
}

Scene1::~Scene1()
{
}

void Scene1::Init()
{
	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);

	//Enable back face culling
	glDisable(GL_CULL_FACE);

	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Load the shader programs
	m_programID = LoadShaders("Shader//TransformVertexShader.vertexshader",
								"Shader//SimpleFragmentShader.fragmentshader");
	glUseProgram(m_programID);

	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	

	// Initialise the cameras
	camera.Init(Vector3(7, 7, 8), Vector3(1, 1, 1), Vector3(0, 1, 0));

	// initialising the moveZ and planePos to zero vectors at origin for Z AXIS
	moveZ.SetZero();
	planePos.SetZero();
	// initialising moveZ and planePos to zero vector at origin for Y AXIS
	rotationY = 0.0f; // current angle of rotation of the plane
	rotationSpeed = 0.0f; // doesn't rotate until key is pressed


	// init scale variable and scale size variable to scale object
	scaleY = 0.0f; // current scale size
	scaleSize = 10.0f; // doesnt scale unless key is pressed

	//define the projection matrix 
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);


	// Init VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = nullptr;
	}

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(0,0,0), 1);
	meshList[GEO_RANDQUADCOLOR] = MeshBuilder::GenerateQuad("quad", Color(0.502f, 0.712f, 1), 1);
	meshList[GEO_WHITEQUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1);
	meshList[GEO_LANDQUAD] = MeshBuilder::GenerateQuad("quad", Color(0, 0.5f, 0), 1);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 1,
		1), 1);
	meshList[GEO_BLUECUBE] = MeshBuilder::GenerateCube("cube", Color(0, 0,
		0.5f), 1);
	meshList[GEO_BLACKCUBE] = MeshBuilder::GenerateCube("cube", Color(0, 0,
		0), 1);
	meshList[GEO_GYMCUBE] = MeshBuilder::GenerateGymCube("gymcube", Color(0.91f, 0.76f,
		0.65f), 1);
	meshList[GEO_REDCUBE] = MeshBuilder::GenerateCube("gymcube", Color(1, 0,
		0), 1);
	meshList[GEO_GREENCUBE] = MeshBuilder::GenerateGymCube("gymcube", Color(0, 0.6f,
		0), 1);
	meshList[GEO_SATANGRAYCUBE] = MeshBuilder::GenerateGymCube("gymcube", Color(1, 0.502f,
		1), 1);
	meshList[GEO_ORANGECUBE] = MeshBuilder::GenerateCube("gymcube", Color(1, 0.5f,
		0), 1);
	meshList[GEO_FLOORCOLOR] = MeshBuilder::GenerateGymCube("gymcube", Color(0.416f, 0.353f,
		0.804f), 1);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 1,
		1),  128 ,128);
	meshList[GEO_GREENSPHERE] = MeshBuilder::GenerateSphere("sphere", Color(0, 0.8f,
		0), 128, 128);
	meshList[GEO_BLUEGREENSPHERE] = MeshBuilder::GenerateSphere("sphere", Color(0, 0.5f,
		0.5), 128, 128);
	meshList[GEO_REDSPHERE] = MeshBuilder::GenerateSphere("sphere", Color(0.5, 0,
		0.0), 128, 128);
	meshList[GEO_RANDSPHERECOLOR] = MeshBuilder::GenerateSphere("sphere", Color(0.502, 0.852,
		0.322), 128, 128);
	meshList[GEO_ORANGESPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0.5f,
		0), 128, 128);
	meshList[GEO_CIRCLE] = MeshBuilder::GenerateCircle("circle" , Color(1, 1,
		1), 16);
	meshList[GEO_CONE] = MeshBuilder::GenerateCone("cone", Color(1, 1,
		1), 16 , 2);
	meshList[GEO_GREENCONE] = MeshBuilder::GenerateCone("cone", Color(0, 0.8f,
		0), 16, 2);
	meshList[GEO_CYLINDER] = MeshBuilder::GenerateCylinder("cylinder", Color(1, 1,
		1), 16, 1);
	meshList[GEO_BROWNCYLINDER] = MeshBuilder::GenerateCylinder("cylinder", Color(0.5f ,0.35f,
		0.5f), 16, 1);
	meshList[GEO_BLACKCYLINDER] = MeshBuilder::GenerateCylinder("cylinder", Color(0, 0,
		0), 16, 1);
	meshList[GEO_REDCYLINDER] = MeshBuilder::GenerateCylinder("cylinder", Color(0.7f, 0,
		0), 16, 1);
	meshList[GEO_BLUECYLINDER] = MeshBuilder::GenerateCylinder("cylinder", Color(0, 0,
		0.5f), 16, 1);
}

void Scene1::Update(double dt)	
{
	// Check for key press, you can add more for interaction
	HandleKeyPress();

	// Update the camera object
	camera.Update(dt);
	// update the translation of plane based on time 
	planePos += moveZ * dt;

	// Reset moveZ so that it will only move whenever key is pressed
	moveZ.SetZero();

	//update the rotation of the plane based on time
	rotationY += rotationSpeed * dt;
	rotationSpeed = 0.0f; // Reset so only rotate when key is pressed

	// update the scale of the plane based on time 
	scaleY += scaleSize * dt; 
	// equals to a number because scaleSize is declared as a float	
	scaleSize = 0.0f; // Reset so only rotate when key is pressed
}

void Scene1::Render()
{
	// Define the matrices to handle transformation
	//Mtx44 view;
	//// Use the camera as the view matrix
	//view.SetToLookAt(
	//	camera.position.x, camera.position.y, camera.position.z,
	//	camera.target.x, camera.target.y, camera.target.z,
	//	camera.up.x, camera.up.y, camera.up.z
	//);

	//Implementing matrix stack
	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y,
		camera.position.z, camera.target.x, camera.target.y,
		camera.target.z, camera.up.x, camera.up.y, camera.up.z);

	modelStack.LoadIdentity();


	// Always set the model matrix to identity
	// i.e. placed at origin, scale 1 and rotation 0
	Mtx44 model;
	model.SetToIdentity();


	// FOV, Aspect Ratio, Near plane, Far plane
	Mtx44 projection;
	projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);


	// Clear color buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	// Render VBO here
	meshList[GEO_AXES]->Render();
	// meshList[GEO_CIRCLE]->Render();
	// meshList[GEO_CONE]->Render();
	/* meshList[GEO_CYLINDER]->Render();*/
	// meshList[GEO_SPHERE]->Render();

	modelStack.PushMatrix();
	{
		modelStack.Scale(1, 1, 1);
	// Push Matrix call
	// WORLD LAND
	modelStack.PushMatrix();
	{
		modelStack.Translate(0, 0, 0);
		/*modelStack.Translate(planePos.x, 0, planePos.z);*/
		// modelStack.Rotate(rotationY, 0.0f, 1.0f, 0.0f);
		// --------------------- WORLD SPACE --------------------
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(planePos.x, 0, planePos.z);
			// Scale the back rest to its position
			modelStack.Scale(100, 1, 100);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_LANDQUAD]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	{
		modelStack.Translate(0, 0, 0);
	/*	modelStack.Translate(planePos.x, 0, planePos.z);*/
		// modelStack.Rotate(rotationY, 0.0f, 1.0f, 0.0f);

		// ------------- GYM BUILDING --------------
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(0.5f, 1.8f, 1);
			/*modelStack.Rotate(rotationY, 0.0f, 1.0f, 0.0f);*/
			// Scale the back rest to its position
			modelStack.Scale(5, 5, 5);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GYMCUBE]->Render();
		}
		modelStack.PopMatrix();
		// ------------------------ Levels 3 ---------------------
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate( 0.501f,  3, 1.01);
			// modelStack.Rotate(rotationY, 0.0f, 1.0f, 0.0f); 
			// Scale the back rest to its position
			modelStack.Scale(5, 0.2f, 5);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_FLOORCOLOR]->Render();
		}
		modelStack.PopMatrix();

		// ----------------- Level 2 ------------------
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x + 0.5f, planePos.y + 1.5f, planePos.z - 1);*/// uncomment if want to move where the building
			modelStack.Translate(0.501f, 1.5f, 1.01);
			/*modelStack.Rotate(rotationY, 0.0f, 1.0f, 0.0f); */
			// Scale the back rest to its position
			modelStack.Scale(5, 0.1f, 5);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_FLOORCOLOR]->Render();
		}
		modelStack.PopMatrix();
		// ----------------------------------- Level 1 ------------------------------------------
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x + 0.5f, planePos.y + 1.5f, planePos.z - 1);*/// uncomment if want to move where the building
			modelStack.Translate(0.501f, 0.0f, 1.01f);
			/*modelStack.Rotate(rotationY, 0.0f, 1.0f, 0.0f); */
			// Scale the back rest to its position
			modelStack.Scale(5, 0.6f, 5);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_FLOORCOLOR]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();


	// ---------------- Chest Press ----------------
	modelStack.PushMatrix();
	{
		/*modelStack.Translate(planePos.x, 0, planePos.z);*/
		modelStack.Translate(0, 0.2f, -0.5f);
		// modelStack.Rotate(rotationY, 0.0f, 1.0f, 0.0f);
		modelStack.Scale(1, 1, 2);
		modelStack.PushMatrix();
		{
			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(- 0.5f, 3.5,  0.8f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
	/*		modelStack.Translate(planePos.x - 0.5f, 3.5f, planePos.z + 0.2f);*/
			modelStack.Translate(-0.5f, 3.5f, 0.2f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		// ------------------------BARBELL----------------------
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(-0.5f, 3.5f,  0.5f);
			/*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.0f, 0.4f, 0.03f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CYLINDER]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	{
		modelStack.Translate(0, 0, 0);
		/*modelStack.Translate(planePos.x, 0, planePos.z);*/
		// BENCH CHAIR
		modelStack.PushMatrix();
		{

			// --------------------- BENCH SEAT --------------------
			// Translate the back rest to its position
			modelStack.Translate(- 0.5f, 3.35f, 0.5f);
			modelStack.Rotate(-185, 0 , 1, 1);
			// Scale the back rest to its position
			modelStack.Scale(1, 0.4f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CUBE]->Render();
		}
		modelStack.PopMatrix();
		// BackRest
	modelStack.PushMatrix();
		{

			// --------------------- BENCH BACK LEG --------------------
			// Translate the back rest to its position
			modelStack.Translate( -0.95, 3.2f, 0.5f);
			modelStack.Rotate(90, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.3f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CUBE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- BENCH FRONT LEG --------------------
			// Translate the back rest to its position
			modelStack.Translate(- 0.02f, 3.2f,  0.5f);
			modelStack.Rotate(90, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.3f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CUBE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// --------------------- BAR FRONT LEFT --------------------
			// Translate the back rest to its positio
			modelStack.Translate( 0.3f, 3.5f, 1);
			modelStack.Rotate(90, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 1, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- BAR BACK LEFT--------------------
			// Translate the back rest to its position
			modelStack.Translate(- 1.2f, 3.5f,  1);
			modelStack.Rotate(90, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 1, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- BAR BACK RIGHT --------------------
			// Translate the back rest to its position
			modelStack.Translate( -1.2f, 3.5f,  - 0.8f);
			modelStack.Rotate(90, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 1, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- BAR FRONT RIGHT --------------------
			// Translate the back rest to its position
			modelStack.Translate( 0.3f, 3.5f,  - 0.8f);
			modelStack.Rotate(90, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 1, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- BAR FRONT--------------------
			// Translate the back rest to its position
			modelStack.Translate(0.3f, 3.95, 0.1f);
			modelStack.Rotate(180, 0, 1, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 1.8f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- BAR BACK --------------------
			// Translate the back rest to its position
			modelStack.Translate( - 1.2f, 3.95, 0.1f);
			modelStack.Rotate(180, 0, 1, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 1.8f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- BAR SIDE LEFT --------------------
			// Translate the back rest to its position
			modelStack.Translate( - 0.5f, 3.95f, 1);
			modelStack.Rotate(90, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 1.5f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- BAR SIDE RIGHT --------------------
			// Translate the back rest to its position
			modelStack.Translate(- 0.4f, 3.95f, - 0.8f);
			modelStack.Rotate(90, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 1.5f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- BARBELL HOLDER BACK--------------------
			// Translate the back rest to its position
			modelStack.Translate(- 0.45f, 3.5f, 1);
			modelStack.Rotate(90, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 1.4f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLUECUBE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- BARBELL HOLDER FRONT--------------------
			// Translate the back rest to its position
			modelStack.Translate(- 0.45f, 3.5f, - 0.8f);
			modelStack.Rotate(90, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 1.4f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLUECUBE]->Render();
		}
		modelStack.PopMatrix();

	}
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	{
		/*modelStack.Translate(0, 0, 0);*/
		//modelStack.Translate(planePos.x +1.8f, 0, planePos.z);
		modelStack.Translate( 1.85, 0, 0);
		modelStack.Rotate(90, 0 , 1, 0);
		// BENCH CHAIR
		//modelStack.PushMatrix();
		//{

		//	// --------------------- BENCH SEAT --------------------
		//	// Translate the back rest to its position
		//	modelStack.Translate(- 0.5f, 3.35f, 0.5f);
		//	modelStack.Rotate(-185, 0, 1, 1);
		//	// Scale the back rest to its position
		//	modelStack.Scale(1, 0.4f, 0.1f);
		//	// Compute the MVP using projection, view and model
		//	MVP = projectionStack.Top() * viewStack.Top() *
		//		modelStack.Top();
		//	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		//		&MVP.a[0]);
		//	// Render the chair part accordingly
		//	meshList[GEO_CUBE]->Render();
		//}
		//modelStack.PopMatrix();
		//// BackRest
		//modelStack.PushMatrix();
		//{

		//	// --------------------- BENCH BACK LEG --------------------
		//	// Translate the back rest to its position
		//	modelStack.Translate( - 0.95, 3.2f,  0.5f);
		//	modelStack.Rotate(90, 0, 1, 0);
		//	// Scale the back rest to its position
		//	modelStack.Scale(0.1f, 0.3f, 0.1f);
		//	// Compute the MVP using projection, view and model
		//	MVP = projectionStack.Top() * viewStack.Top() *
		//		modelStack.Top();
		//	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		//		&MVP.a[0]);
		//	// Render the chair part accordingly
		//	meshList[GEO_CUBE]->Render();
		//}
		//modelStack.PopMatrix();

		//modelStack.PushMatrix();
		//{

		//	// --------------------- BENCH FRONT LEG --------------------
		//	// Translate the back rest to its position
		//	modelStack.Translate(- 0.02f, 3.2f,  0.5f);
		//	modelStack.Rotate(90, 0, 1, 0);
		//	// Scale the back rest to its position
		//	modelStack.Scale(0.1f, 0.3f, 0.1f);
		//	// Compute the MVP using projection, view and model
		//	MVP = projectionStack.Top() * viewStack.Top() *
		//		modelStack.Top();
		//	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		//		&MVP.a[0]);
		//	// Render the chair part accordingly
		//	meshList[GEO_CUBE]->Render();
		//}
		//modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// --------------------- BAR FRONT LEFT --------------------
			// Translate the back rest to its position
			modelStack.Translate( 0.3f, 3.5f, 1);
			modelStack.Rotate(90, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 1, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- BAR BACK LEFT--------------------
			// Translate the back rest to its position
			modelStack.Translate( - 1.2f, 3.5f, 1);
			modelStack.Rotate(90, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 1, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- BAR BACK RIGHT --------------------
			// Translate the back rest to its position
			modelStack.Translate( - 1.2f, 3.5f, - 0.8f);
			modelStack.Rotate(90, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 1, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- BAR FRONT RIGHT --------------------
			// Translate the back rest to its position
			modelStack.Translate( 0.3f, 3.5f,  - 0.8f);
			modelStack.Rotate(90, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 1, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- BAR FRONT--------------------
			// Translate the back rest to its position
			modelStack.Translate( 0.3f, 3.95,  0.1f);
			modelStack.Rotate(180, 0, 1, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 1.8f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- BAR BACK --------------------
			// Translate the back rest to its position
			modelStack.Translate(- 1.2f, 3.95, 0.1f);
			modelStack.Rotate(180, 0, 1, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 1.8f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- BAR SIDE LEFT --------------------
			// Translate the back rest to its position
			modelStack.Translate(- 0.5f, 3.95f,  1);
			modelStack.Rotate(90, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 1.5f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- BAR SIDE RIGHT --------------------
			// Translate the back rest to its position
			modelStack.Translate(- 0.4f, 3.95f, - 0.8f);
			modelStack.Rotate(90, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 1.5f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- BARBELL HOLDER BACK--------------------
			// Translate the back rest to its position
			modelStack.Translate( - 0.45f, 3.5f,  1);
			modelStack.Rotate(90, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 1.4f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLUECUBE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- BARBELL HOLDER FRONT--------------------
			// Translate the back rest to its position
			modelStack.Translate(- 0.45f, 3.5f, -0.8f);
			modelStack.Rotate(90, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 1.4f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLUECUBE]->Render();
		}
		modelStack.PopMatrix();

	}
	modelStack.PopMatrix();


	// SECOND BENCH PRESS BAR
	modelStack.PushMatrix();
	{
		/*modelStack.Translate(planePos.x, 0, planePos.z);*/
		modelStack.Translate(0.3f, 0.2f, - 0.5f);
		modelStack.Rotate(90, 0.0f, 1.0f, 0.0f);
		modelStack.Scale(2, 1, 3.2f);
		modelStack.PushMatrix();
		{
			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.5, 0.8f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
	/*		modelStack.Translate(planePos.x - 0.5f, 3.5f, planePos.z + 0.2f);*/
			modelStack.Translate(-0.5f, 3.5f, 0.2f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		// ------------------------BARBELL----------------------
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(-0.5f, 3.5f, 0.5f);
			/*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.0f, 0.4f, 0.03f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CYLINDER]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();



	modelStack.PushMatrix();
	{
		modelStack.Translate(-1.5f, 3.5f, 2);
		modelStack.Rotate(180, 2, 1, 0);
		// BLACK TV
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(0, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(1, 1, 1);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_WHITEQUAD]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();



	modelStack.PushMatrix();
	{

		modelStack.Translate(-4, 5, 3);
		modelStack.Rotate(180, 2, 1, 0);
		modelStack.PushMatrix();
		{
			 // Translate the back rest to its position
			modelStack.Translate(1, 3, 1);
			// Scale the back rest to its position
			modelStack.Scale(1, 1, 2);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_QUAD]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();


	// DUMBELLS
	modelStack.PushMatrix();
	{
		/*modelStack.Translate(planePos.x, 0, planePos.z);*/
		modelStack.Translate(0.5f, 3.4f, - 0.8f);
		modelStack.Rotate(90, 1.0f, 0.0f, 0.0f);
		modelStack.Scale(1, 1, 0.5f);
		modelStack.PushMatrix();
		{
			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.5, 0.8f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
	/*		modelStack.Translate(planePos.x - 0.5f, 3.5f, planePos.z + 0.2f);*/
			modelStack.Translate(-0.5f, 3.5f, 0.2f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		// ------------------------BARBELL----------------------
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(-0.5f, 3.5f, 0.5f);
			/*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.0f, 0.4f, 0.03f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CYLINDER]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();



	modelStack.PushMatrix();
	{
		/*modelStack.Translate(planePos.x, 0, planePos.z);*/
		modelStack.Translate(0.5f, 0.9f, 0.4f);
		modelStack.Rotate(45, 1.0f, 0.0f, 0.0f);
		modelStack.Scale(1, 1, 0.5f);
		modelStack.PushMatrix();
		{
			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.5, 0.8f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
	/*		modelStack.Translate(planePos.x - 0.5f, 3.5f, planePos.z + 0.2f);*/
			modelStack.Translate(-0.5f, 3.5f, 0.2f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		// ------------------------BARBELL----------------------
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(-0.5f, 3.5f, 0.5f);
			/*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.0f, 0.4f, 0.03f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CYLINDER]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	{
		modelStack.Translate(-3.5f, 0 , 0);
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(2.5f, 3.3f, 3);
			/*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.35, 0.35f, 0.35f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENSPHERE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(2.5f, 3.3f, 2.3f);
			/*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.35, 0.35f, 0.35f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLUEGREENSPHERE]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();
	

	// DUMBELL RACK 
	modelStack.PushMatrix();
	{
		modelStack.Translate(1.9f , 1.1 , 2);
		modelStack.Scale(0.6f,0.6f, 0.6f);
		modelStack.PushMatrix();
		{
			// FAR LEFT SLANTED BAR
			// Translate the back rest to its position
	/*		modelStack.Translate(planePos.x + 1, 3.3f, planePos.z + 1);*/
			modelStack.Translate( 1, 3.3f, 1);
		/*	modelStack.Rotate(rotationY -120, 1, 2, 1);*/
			modelStack.Rotate(-120, 1, 2, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 1, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();
		}
		modelStack.PopMatrix();


		// FAR RIGHT SLANTED
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
	/*		modelStack.Translate(planePos.x + 1, 3.3f, planePos.z + 2);*/
			modelStack.Translate(1, 3.3f, 2);
			/*modelStack.Rotate(rotationY - 120, 1, 2, 1);*/
			modelStack.Rotate(-120, 1, 2, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 1, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();
		}
		modelStack.PopMatrix();

		// Horizontal Bar Top
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x + 1.45f, 3.55f, planePos.z + 1.55f);*/
			modelStack.Translate( 1.45f, 3.55f,1.55f);
			/*modelStack.Rotate(rotationY + 180, 0, 1, 1);*/
			modelStack.Rotate(180, 0, 1, 1);
			// Scale the back rest to its position
			// z makes it thicker , x makes it wider
			modelStack.Scale(0.1f, 1.2f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLUECUBE]->Render();

		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
		/*	modelStack.Translate(planePos.x + 1, 3.35f, planePos.z + 1.55f);*/
			modelStack.Translate(1, 3.35f, 1.55f);
			/*modelStack.Rotate(rotationY + 180, 0, 1, 1);*/
			modelStack.Rotate(180, 0, 1, 1);
			// Scale the back rest to its position
			// z makes it thicker , x makes it wider
			modelStack.Scale(0.1f, 1.2f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLUECUBE]->Render();

		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	{
		modelStack.Translate(0.3f, 0.4f, 0.5f);
		modelStack.Scale(0.8f, 0.8f, 0.8f);
	// ---------------------- TOP RACK DUMBBELLS ---------------------------
	modelStack.PushMatrix();
	{
		/*modelStack.Translate(planePos.x, 0, planePos.z);*/
		modelStack.Translate(2.9f, 0.2f, 2.1f);
		//modelStack.Rotate(rotationY + 90, 0, 1, 0);
		modelStack.Rotate( 90, 0, 1, 0);
		/*modelStack.Rotate(45, 1.0f, 0.0f, 0.0f);*/
		modelStack.Scale(1, 1, 0.5f);
		modelStack.PushMatrix();
		{
			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.5, 0.8f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
	/*		modelStack.Translate(planePos.x - 0.5f, 3.5f, planePos.z + 0.2f);*/
			modelStack.Translate(-0.5f, 3.5f, 0.2f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		// ------------------------BARBELL----------------------
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(-0.5f, 3.5f, 0.5f);
			/*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.0f, 0.4f, 0.03f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CYLINDER]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();



	modelStack.PushMatrix();
	{
		/*modelStack.Translate(planePos.x, 0, planePos.z);*/
		modelStack.Translate(2.9f, 0.2f, 2.3f);
		/*modelStack.Rotate(rotationY + 90, 0, 1, 0);*/
		modelStack.Rotate( 90, 0, 1, 0);
		/*modelStack.Rotate(45, 1.0f, 0.0f, 0.0f);*/
		modelStack.Scale(1, 1, 0.5f);
		modelStack.PushMatrix();
		{
			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.5, 0.8f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
	/*		modelStack.Translate(planePos.x - 0.5f, 3.5f, planePos.z + 0.2f);*/
			modelStack.Translate(-0.5f, 3.5f, 0.2f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		// ------------------------BARBELL----------------------
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(-0.5f, 3.5f, 0.5f);
			/*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.0f, 0.4f, 0.03f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CYLINDER]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	{
		/*modelStack.Translate(planePos.x, 0, planePos.z);*/
		modelStack.Translate(2.9f, 0.2f, 2.5f);
	/*	modelStack.Rotate(rotationY + 90, 0, 1, 0);*/
		modelStack.Rotate( 90, 0, 1, 0);
		/*modelStack.Rotate(45, 1.0f, 0.0f, 0.0f);*/
		modelStack.Scale(1, 1, 0.5f);
		modelStack.PushMatrix();
		{
			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.5, 0.8f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
	/*		modelStack.Translate(planePos.x - 0.5f, 3.5f, planePos.z + 0.2f);*/
			modelStack.Translate(-0.5f, 3.5f, 0.2f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		// ------------------------BARBELL----------------------
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(-0.5f, 3.5f, 0.5f);
			/*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.0f, 0.4f, 0.03f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CYLINDER]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();



	modelStack.PushMatrix();
	{
		/*modelStack.Translate(planePos.x, 0, planePos.z);*/
		modelStack.Translate(2.9f, 0.2f, 2.7f);/*
		modelStack.Rotate(rotationY + 90, 0, 1, 0);*/
		modelStack.Rotate( 90, 0, 1, 0);
		/*modelStack.Rotate(45, 1.0f, 0.0f, 0.0f);*/
		modelStack.Scale(1, 1, 0.5f);
		modelStack.PushMatrix();
		{
			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.5, 0.8f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
	/*		modelStack.Translate(planePos.x - 0.5f, 3.5f, planePos.z + 0.2f);*/
			modelStack.Translate(-0.5f, 3.5f, 0.2f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		// ------------------------BARBELL----------------------
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(-0.5f, 3.5f, 0.5f);
			/*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.0f, 0.4f, 0.03f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CYLINDER]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();



	// -----------------------------------BOTTOM RACK DUMBBELLS ----------------------------------
	modelStack.PushMatrix();
	{
		/*modelStack.Translate(planePos.x, 0, planePos.z);*/
		modelStack.Translate(2.45f, 0, 2.1f);
		/*modelStack.Rotate(rotationY + 90, 0, 1, 0);*/
		modelStack.Rotate( 90, 0, 1, 0);
		/*modelStack.Rotate(45, 1.0f, 0.0f, 0.0f);*/
		modelStack.Scale(1, 1, 0.5f);
		modelStack.PushMatrix();
		{
			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.5, 0.8f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
	/*		modelStack.Translate(planePos.x - 0.5f, 3.5f, planePos.z + 0.2f);*/
			modelStack.Translate(-0.5f, 3.5f, 0.2f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		// ------------------------BARBELL----------------------
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(-0.5f, 3.5f, 0.5f);
			/*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.0f, 0.4f, 0.03f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CYLINDER]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();



	modelStack.PushMatrix();
	{
		/*modelStack.Translate(planePos.x, 0, planePos.z);*/
		modelStack.Translate(2.45f, 0, 2.3f);
		/*modelStack.Rotate(rotationY + 90, 0, 1, 0);*/
		modelStack.Rotate(90, 0, 1, 0);
		/*modelStack.Rotate(45, 1.0f, 0.0f, 0.0f);*/
		modelStack.Scale(1, 1, 0.5f);
		modelStack.PushMatrix();
		{
			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.5, 0.8f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
	/*		modelStack.Translate(planePos.x - 0.5f, 3.5f, planePos.z + 0.2f);*/
			modelStack.Translate(-0.5f, 3.5f, 0.2f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		// ------------------------BARBELL----------------------
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(-0.5f, 3.5f, 0.5f);
			/*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.0f, 0.4f, 0.03f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CYLINDER]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	{
		/*modelStack.Translate(planePos.x, 0, planePos.z);*/
		modelStack.Translate(2.45f, 0, 2.5f);
		/*modelStack.Rotate(rotationY + 90, 0, 1, 0);*/
		modelStack.Rotate(90, 0, 1, 0);
		/*modelStack.Rotate(45, 1.0f, 0.0f, 0.0f);*/
		modelStack.Scale(1, 1, 0.5f);
		modelStack.PushMatrix();
		{
			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.5, 0.8f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
	/*		modelStack.Translate(planePos.x - 0.5f, 3.5f, planePos.z + 0.2f);*/
			modelStack.Translate(-0.5f, 3.5f, 0.2f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		// ------------------------BARBELL----------------------
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(-0.5f, 3.5f, 0.5f);
			/*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.0f, 0.4f, 0.03f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CYLINDER]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();



	modelStack.PushMatrix();
	{
		/*modelStack.Translate(planePos.x, 0, planePos.z);*/
		modelStack.Translate(2.45f, 0, 2.7f);
	/*	modelStack.Rotate(rotationY + 90, 0, 1, 0);*/
		modelStack.Rotate(90, 0, 1, 0);
		/*modelStack.Rotate(45, 1.0f, 0.0f, 0.0f);*/
		modelStack.Scale(1, 1, 0.5f);
		modelStack.PushMatrix();
		{
			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.5, 0.8f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
	/*		modelStack.Translate(planePos.x - 0.5f, 3.5f, planePos.z + 0.2f);*/
			modelStack.Translate(-0.5f, 3.5f, 0.2f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		// ------------------------BARBELL----------------------
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(-0.5f, 3.5f, 0.5f);
			/*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.0f, 0.4f, 0.03f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CYLINDER]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();
	}
	modelStack.PopMatrix();

	// --------------------------COOL GYM CHAIRS--------------------------
	modelStack.PushMatrix();
	{
		// Translate the entire chair
		modelStack.Translate(0.8f, 3.2f, 3);
		modelStack.Rotate(180 , 0, 1, 0);
		modelStack.Scale(0.5f, 0.5f, 0.5f);
		// modelStack.Rotate(rotationY, 0.0f, 1.0f, 0.0f); // uncomment this if want rotate whole chair
		// BackRest
		//modelStack.PushMatrix();
		//{
		//	// Translate the back rest to its position
		//	modelStack.Translate(planePos.x, planePos.y + 0.5f, planePos.z);
		//	// Scale the back rest to its position
		//	modelStack.Scale(1, 1, 0.2f);
		//	// Compute the MVP using projection, view and model
		//	MVP = projectionStack.Top() * viewStack.Top() *
		//		modelStack.Top();
		//	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		//		&MVP.a[0]);
		//	// Render the chair part accordingly
		//	meshList[GEO_CUBE]->Render();
		//}
		//modelStack.PopMatrix();
		// Seat
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x, planePos.y + 0.2f, planePos.z + 0.4f);*/
			modelStack.Translate(0,  0.2f, 0.4f);
			// Scale the back rest to its position
			modelStack.Scale(1, 0.2f, 1.5f);
			modelStack.Rotate(0, 0.0f, 1.0f, 0.0f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CUBE]->Render();
		}
		modelStack.PopMatrix();
		
		// SEAT BASE CONNECTOR
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(0, -0.2f, 1.6f);
			/*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
			modelStack.Rotate(-50, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.4f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CYLINDER]->Render();
		}
		modelStack.PopMatrix();



		// SEAT BASE FRONT
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x, planePos.y - 0.5f, planePos.z + 2);*/
			modelStack.Translate(0, - 0.5f,  2);
			// Scale the back rest to its position
			modelStack.Rotate(270, 0.0f, 1.0f, 0.0f);
			modelStack.Scale(0.5, 0.3f, 1.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CUBE]->Render();
		}
		modelStack.PopMatrix();


		// SEAT BACK CONNECTOR

		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x, planePos.y - 0.0f, planePos.z);*/
			modelStack.Translate(0, 0, 0);
			// Scale the back rest to its position
			modelStack.Rotate(-90, 1.0f, 0.0f, 0.0f);
			modelStack.Scale(0.3f, 0.3f, 0.4f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CUBE]->Render();
		}
		modelStack.PopMatrix();

		// BALL ROLLER FOR CHAIR

		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x + 0.05f, planePos.y - 0.4f, planePos.z);*/
			modelStack.Translate(0.05f, -0.4f, 0);
			///*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
			//modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2, 0.2f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLUEGREENSPHERE]->Render();
		}
		modelStack.PopMatrix();
	
		// BACKREST
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
		/*	modelStack.Translate(planePos.x, planePos.y + 0.5f, planePos.z  - 0.3f);*/
			modelStack.Translate(0, 0.5f, - 0.3f);
			modelStack.Rotate(-45, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(1 , 0.8f , 0.3f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CUBE]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();



	modelStack.PushMatrix();
	{
		// Translate the entire chair
		modelStack.Translate( 1.8f, 3.2f, 2);
		modelStack.Rotate(0, 0, 1, 0);
		modelStack.Scale(0.5f, 0.5f, 0.5f);
		// modelStack.Rotate(rotationY, 0.0f, 1.0f, 0.0f); // uncomment this if want rotate whole chair
		// BackRest
		//modelStack.PushMatrix();
		//{
		//	// Translate the back rest to its position
		//	modelStack.Translate(planePos.x, planePos.y + 0.5f, planePos.z);
		//	// Scale the back rest to its position
		//	modelStack.Scale(1, 1, 0.2f);
		//	// Compute the MVP using projection, view and model
		//	MVP = projectionStack.Top() * viewStack.Top() *
		//		modelStack.Top();
		//	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		//		&MVP.a[0]);
		//	// Render the chair part accordingly
		//	meshList[GEO_CUBE]->Render();
		//}
		//modelStack.PopMatrix();
		// Seat
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x, planePos.y + 0.2f, planePos.z + 0.4f);*/
			modelStack.Translate(0, 0.2f, 0.4f);
			// Scale the back rest to its position
			modelStack.Scale(1, 0.2f, 1.5f);
			modelStack.Rotate(0, 0.0f, 1.0f, 0.0f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CUBE]->Render();
		}
		modelStack.PopMatrix();

		// SEAT BASE CONNECTOR
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(0, -0.2f, 1.6f);
			/*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
			modelStack.Rotate(-50, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.4f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CYLINDER]->Render();
		}
		modelStack.PopMatrix();



		// SEAT BASE FRONT
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x, planePos.y - 0.5f, planePos.z + 2);*/
			modelStack.Translate(0, -0.5f, 2);
			// Scale the back rest to its position
			modelStack.Rotate(270, 0.0f, 1.0f, 0.0f);
			modelStack.Scale(0.5, 0.3f, 1.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CUBE]->Render();
		}
		modelStack.PopMatrix();


		// SEAT BACK CONNECTOR

		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x, planePos.y - 0.0f, planePos.z);*/
			modelStack.Translate(0, 0, 0);
			// Scale the back rest to its position
			modelStack.Rotate(-90, 1.0f, 0.0f, 0.0f);
			modelStack.Scale(0.3f, 0.3f, 0.4f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CUBE]->Render();
		}
		modelStack.PopMatrix();

		// BALL ROLLER FOR CHAIR

		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x + 0.05f, planePos.y - 0.4f, planePos.z);*/
			modelStack.Translate(0.05f, -0.4f, 0);
			///*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
			//modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2, 0.2f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLUEGREENSPHERE]->Render();
		}
		modelStack.PopMatrix();

		// BACKREST
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
		/*	modelStack.Translate(planePos.x, planePos.y + 0.5f, planePos.z  - 0.3f);*/
			modelStack.Translate(0, 0.5f, -0.3f);
			modelStack.Rotate(-45, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(1, 0.8f, 0.3f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CUBE]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();


	// ----------------------------- TREADMILL 2ND FLOOR ----------------------------------------
	modelStack.PushMatrix();
	{
		modelStack.Translate(1.2f, -1.1f, 0.9f);
		modelStack.Rotate(270, 0, 1, 0);
		modelStack.Scale(1.2f, 0.9f, 0.9f);
		modelStack.PushMatrix();
		{
			// FAR LEFT SLANTED BAR
			// Translate the back rest to its position
	/*		modelStack.Translate(planePos.x + 1, 3.3f, planePos.z + 1);*/
			modelStack.Translate(1, 3.3f, 1);
			/*	modelStack.Rotate(rotationY -120, 1, 2, 1);*/
			modelStack.Rotate(-120, 1, 2, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 1, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCUBE]->Render();
		}
		modelStack.PopMatrix();


		// FAR RIGHT SLANTED
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
	/*		modelStack.Translate(planePos.x + 1, 3.3f, planePos.z + 2);*/
			modelStack.Translate(1, 3.3f, 2);
			/*modelStack.Rotate(rotationY - 120, 1, 2, 1);*/
			modelStack.Rotate(-120, 1, 2, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 1, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCUBE]->Render();
		}
		modelStack.PopMatrix();

		// Horizontal Bar Top
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x + 1.45f, 3.55f, planePos.z + 1.55f);*/
			modelStack.Translate(1.5f, 3.63f, 1.55f);
			/*modelStack.Rotate(rotationY + 180, 0, 1, 1);*/
			modelStack.Rotate( 90, 0, 0, 1);
			modelStack.Rotate( 90, 1, 0, 0);
			// Scale the back rest to its position
			// z makes it thicker , x makes it wider
			modelStack.Scale(0.1f, 1.3f, 0.2f);

			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,	
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_SATANGRAYCUBE]->Render();

		}
		modelStack.PopMatrix();

		// TREADMILL BASE FAR RIGHT
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			modelStack.Translate(1.5f, 3.2f, 1);
			modelStack.Rotate(90, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.7f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();
		}
		modelStack.PopMatrix();


		// TREADMILL BASE LEFT
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			modelStack.Translate(1.5f, 3.2f, 2);
			modelStack.Rotate(90, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.6f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

		// HORIZONTAL BAR
			// Translate the back rest to its position
			modelStack.Translate(1, 2.9f, 2);
			modelStack.Rotate(90, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 1, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLUECUBE]->Render();
		}
		modelStack.PopMatrix();


		// HORIZONTAL BAR RIGHT

		modelStack.PushMatrix();
		{

			// HORIZONTAL BAR
				// Translate the back rest to its position
			modelStack.Translate(1, 2.9f, 1);
			modelStack.Rotate(90, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 1, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLUECUBE]->Render();
		}
		modelStack.PopMatrix();


		// TREADMILL FLAT BASE
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(1, 2.9f, 1.5f);
			modelStack.Rotate(180, 0, 1, 1);
			// Scale the back rest to its position
			modelStack.Scale(2, 1.5f, 0);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();
		}
		modelStack.PopMatrix();

		// TREADMILL STACKSSS
		modelStack.PushMatrix();
		{

			// --------------------- BAR FRONT--------------------
			// Translate the back rest to its position
			modelStack.Translate(1, 2.9f, 1.5f);
			modelStack.Rotate(180, 0, 1, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.9f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CUBE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- BAR FRONT--------------------
			// Translate the back rest to its position
			modelStack.Translate(0.85f, 2.9f, 1.5f);
			modelStack.Rotate(180, 0, 1, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.9f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CUBE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- BAR FRONT--------------------
			// Translate the back rest to its position
			modelStack.Translate(0.7f, 2.9f, 1.5f);
			modelStack.Rotate(180, 0, 1, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.9f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CUBE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- BAR FRONT--------------------
			// Translate the back rest to its position
			modelStack.Translate(1.15f, 2.9f, 1.5f);
			modelStack.Rotate(180, 0, 1, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.9f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CUBE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- BAR FRONT--------------------
			// Translate the back rest to its position
			modelStack.Translate(1.30f, 2.9f, 1.5f);
			modelStack.Rotate(180, 0, 1, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.9f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CUBE]->Render();
		}
		modelStack.PopMatrix();

		// TV
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x + 1.45f, 3.55f, planePos.z + 1.55f);*/
			modelStack.Translate(1.65f, 3.85f, 1.55f);
			/*modelStack.Rotate(rotationY + 180, 0, 1, 1);*/
			modelStack.Rotate(-30, 0 , 0, 1);
			// Scale the back rest to its position
			// z makes it thicker , x makes it wider
			modelStack.Scale(0.05f, 0.4f, 0.5f);

			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();

		}
		modelStack.PopMatrix();
		
	}
	modelStack.PopMatrix();



	// ------------------------- SECOND TREADMILL -----------------------------------

	modelStack.PushMatrix();
	{
		modelStack.Translate(3, -1.1f, 0.9f);
		modelStack.Rotate(270, 0, 1, 0);
		modelStack.Scale(1.2f, 0.9f, 0.9f);
		modelStack.PushMatrix();
		{
			// FAR LEFT SLANTED BAR
			// Translate the back rest to its position
	/*		modelStack.Translate(planePos.x + 1, 3.3f, planePos.z + 1);*/
			modelStack.Translate(1, 3.3f, 1);
			/*	modelStack.Rotate(rotationY -120, 1, 2, 1);*/
			modelStack.Rotate(-120, 1, 2, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 1, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCUBE]->Render();
		}
		modelStack.PopMatrix();


		// FAR RIGHT SLANTED
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
	/*		modelStack.Translate(planePos.x + 1, 3.3f, planePos.z + 2);*/
			modelStack.Translate(1, 3.3f, 2);
			/*modelStack.Rotate(rotationY - 120, 1, 2, 1);*/
			modelStack.Rotate(-120, 1, 2, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 1, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCUBE]->Render();
		}
		modelStack.PopMatrix();

		// Horizontal Bar Top
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x + 1.45f, 3.55f, planePos.z + 1.55f);*/
			modelStack.Translate(1.5f, 3.63f, 1.55f);
			/*modelStack.Rotate(rotationY + 180, 0, 1, 1);*/
			modelStack.Rotate(90, 0, 0, 1);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			// z makes it thicker , x makes it wider
			modelStack.Scale(0.1f, 1.3f, 0.2f);

			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_SATANGRAYCUBE]->Render();

		}
		modelStack.PopMatrix();

		// TREADMILL BASE FAR RIGHT
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			modelStack.Translate(1.5f, 3.2f, 1);
			modelStack.Rotate(90, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.7f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();
		}
		modelStack.PopMatrix();


		// TREADMILL BASE LEFT
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			modelStack.Translate(1.5f, 3.2f, 2);
			modelStack.Rotate(90, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.6f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// HORIZONTAL BAR
				// Translate the back rest to its position
			modelStack.Translate(1, 2.9f, 2);
			modelStack.Rotate(90, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 1, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLUECUBE]->Render();
		}
		modelStack.PopMatrix();


		// HORIZONTAL BAR RIGHT

		modelStack.PushMatrix();
		{

			// HORIZONTAL BAR
				// Translate the back rest to its position
			modelStack.Translate(1, 2.9f, 1);
			modelStack.Rotate(90, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 1, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLUECUBE]->Render();
		}
		modelStack.PopMatrix();


		// TREADMILL FLAT BASE
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(1, 2.9f, 1.5f);
			modelStack.Rotate(180, 0, 1, 1);
			// Scale the back rest to its position
			modelStack.Scale(2, 1.5f, 0);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();
		}
		modelStack.PopMatrix();

		// TREADMILL STACKSSS
		modelStack.PushMatrix();
		{

			// --------------------- BAR FRONT--------------------
			// Translate the back rest to its position
			modelStack.Translate(1, 2.9f, 1.5f);
			modelStack.Rotate(180, 0, 1, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.9f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CUBE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- BAR FRONT--------------------
			// Translate the back rest to its position
			modelStack.Translate(0.85f, 2.9f, 1.5f);
			modelStack.Rotate(180, 0, 1, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.9f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CUBE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- BAR FRONT--------------------
			// Translate the back rest to its position
			modelStack.Translate(0.7f, 2.9f, 1.5f);
			modelStack.Rotate(180, 0, 1, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.9f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CUBE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- BAR FRONT--------------------
			// Translate the back rest to its position
			modelStack.Translate(1.15f, 2.9f, 1.5f);
			modelStack.Rotate(180, 0, 1, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.9f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CUBE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- BAR FRONT--------------------
			// Translate the back rest to its position
			modelStack.Translate(1.30f, 2.9f, 1.5f);
			modelStack.Rotate(180, 0, 1, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.9f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CUBE]->Render();
		}
		modelStack.PopMatrix();

		// TV
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x + 1.45f, 3.55f, planePos.z + 1.55f);*/
			modelStack.Translate(1.65f, 3.85f, 1.55f);
			/*modelStack.Rotate(rotationY + 180, 0, 1, 1);*/
			modelStack.Rotate(-30, 0, 0, 1);
			// Scale the back rest to its position
			// z makes it thicker , x makes it wider
			modelStack.Scale(0.05f, 0.4f, 0.5f);

			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();

		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();

	// PLANTS
	modelStack.PushMatrix();
	{
		modelStack.Translate(5, -3.5f, -1);
		//PLANT POT
		modelStack.PushMatrix();
		{
			
			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.5, 0.8f);
			modelStack.Rotate(180, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.4f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BROWNCYLINDER]->Render();
		}
		modelStack.PopMatrix();
		// PLANTS
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.7, 0.8f);
			modelStack.Rotate(0 , 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.5f, 1, 0.5f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();

	// PLANTS
	modelStack.PushMatrix();{
	modelStack.Translate(5, -3.5f, 0);
	//PLANT POT
	modelStack.PushMatrix();
	{

		// Translate the back rest to its position
		/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
		modelStack.Translate(-0.5f, 3.5, 0.8f);
		modelStack.Rotate(180, 0, 0, 1);
		// Scale the back rest to its position
		modelStack.Scale(0.2f, 0.4f, 0.2f);
		// Compute the MVP using projection, view and model
		MVP = projectionStack.Top() * viewStack.Top() *
			modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			&MVP.a[0]);
		// Render the chair part accordingly
		meshList[GEO_BROWNCYLINDER]->Render();
	}
	modelStack.PopMatrix();
	// PLANTS
	modelStack.PushMatrix();
	{

		// Translate the back rest to its position
		/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
		modelStack.Translate(-0.5f, 3.7, 0.8f);
		modelStack.Rotate(0, 0, 0, 1);
		// Scale the back rest to its position
		modelStack.Scale(0.5f, 1, 0.5f);
		// Compute the MVP using projection, view and model
		MVP = projectionStack.Top() * viewStack.Top() *
			modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			&MVP.a[0]);
		// Render the chair part accordingly
		meshList[GEO_GREENCONE]->Render();
	}
	modelStack.PopMatrix();
	}
	modelStack.PopMatrix();

	// LEFT OF GYM PLANTS

	modelStack.PushMatrix(); {
		modelStack.Translate(1 , -3.5f, 4);
		//PLANT POT
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.5, 0.8f);
			modelStack.Rotate(180, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.4f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BROWNCYLINDER]->Render();
		}
		modelStack.PopMatrix();
		// PLANTS
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.7, 0.8f);
			modelStack.Rotate(0, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.5f, 1, 0.5f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();


	modelStack.PushMatrix(); {
		modelStack.Translate(0, -3.5f, 4);
		//PLANT POT
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.5, 0.8f);
			modelStack.Rotate(180, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.4f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BROWNCYLINDER]->Render();
		}
		modelStack.PopMatrix();
		// PLANTS
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.7, 0.8f);
			modelStack.Rotate(0, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.5f, 1, 0.5f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();


	// SMALL PLANT 2ND FLOOR
	modelStack.PushMatrix(); {
		modelStack.Translate(-1.5f, 0.8f, 3);
		modelStack.Scale(0.2f, 0.2f, 0.1f);
		//PLANT POT
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.9f, 0.8f);
			modelStack.Rotate(180, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.5f, 0.6f, 1);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BROWNCYLINDER]->Render();
		}
		modelStack.PopMatrix();
		// PLANTS
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.8f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1.2f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1.4f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1.6f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.8f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.6f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.4f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.2f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.0f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.2f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.4f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.6f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();

	// ----------------------------------------------------------------------------------------------------------------------------
	modelStack.PushMatrix(); {
		modelStack.Translate(-1.5f, 0.8f, 2);
		modelStack.Scale(0.2f, 0.2f, 0.1f);
		//PLANT POT
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.9f, 0.8f);
			modelStack.Rotate(180, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.5f, 0.6f, 1);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BROWNCYLINDER]->Render();
		}
		modelStack.PopMatrix();
		// PLANTS
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.8f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1.2f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1.4f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1.6f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.8f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.6f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.4f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.2f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.0f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.2f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.4f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.6f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();


	//----------------------------------------------------------------------------------------------------------
	modelStack.PushMatrix(); {
		modelStack.Translate(-1.5f, 0.8f, 1);
		modelStack.Scale(0.2f, 0.2f, 0.1f);
		//PLANT POT
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.9f, 0.8f);
			modelStack.Rotate(180, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.5f, 0.6f, 1);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BROWNCYLINDER]->Render();
		}
		modelStack.PopMatrix();
		// PLANTS
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.8f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1.2f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1.4f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1.6f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.8f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.6f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.4f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.2f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.0f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.2f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.4f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.6f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();


	//----------------------------------------------------------------------------------------------------------------------
	modelStack.PushMatrix(); {
		modelStack.Translate(2.8f, 0.8f, 2);
		modelStack.Scale(0.2f, 0.2f, 0.1f);
		//PLANT POT
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.9f, 0.8f);
			modelStack.Rotate(180, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.5f, 0.6f, 1);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BROWNCYLINDER]->Render();
		}
		modelStack.PopMatrix();
		// PLANTS
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.8f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1.2f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1.4f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1.6f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.8f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.6f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.4f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.2f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.0f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.2f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.4f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.6f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();


	//---------------------------------------------------------------------------------------------------------------------
	modelStack.PushMatrix(); {
		modelStack.Translate(2.8f, 0.8f, 3);
		modelStack.Scale(0.2f, 0.2f, 0.1f);
		//PLANT POT
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.9f, 0.8f);
			modelStack.Rotate(180, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.5f, 0.6f, 1);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BROWNCYLINDER]->Render();
		}
		modelStack.PopMatrix();
		// PLANTS
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.8f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1.2f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1.4f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1.6f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.8f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.6f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.4f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.2f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.0f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.2f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.4f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.6f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();
	// --------------------------------------------------------------------------------------------------------------------------------
	modelStack.PushMatrix(); {
		modelStack.Translate(2.8f, 0.8f, 1);
		modelStack.Scale(0.2f, 0.2f, 0.1f);
		//PLANT POT
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.9f, 0.8f);
			modelStack.Rotate(180, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.5f, 0.6f, 1);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BROWNCYLINDER]->Render();
		}
		modelStack.PopMatrix();
		// PLANTS
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.8f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1.2f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1.4f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1.6f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.8f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.6f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.4f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.2f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.0f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.2f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.4f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.6f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	{
		modelStack.Translate(-0.5f, 0, 0);
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			modelStack.Translate(0, 2, 0);
			// Scale the back rest to its position
			modelStack.Scale(1, 1, 1);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_WHITEQUAD]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			modelStack.Translate(1, 2, 0);
			// Scale the back rest to its position
			modelStack.Scale(1, 1, 1);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_RANDQUADCOLOR]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	{
		// Translate the back rest to its position
		modelStack.Translate(2 , 2, -0.5f);
		/*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
		modelStack.Rotate(90, 1, 0, 0);
		// Scale the back rest to its position
		modelStack.Scale(0.35, 0.35f, 0.35f);
		// Compute the MVP using projection, view and model
		MVP = projectionStack.Top() * viewStack.Top() *
			modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			&MVP.a[0]);
		// Render the chair part accordingly
		meshList[GEO_REDSPHERE]->Render();
	}
	modelStack.PopMatrix();

	// --------------------------- STAIRS TO GYM -----------------------------
	modelStack.PushMatrix();
	{
		// Translate the back rest to its position
		modelStack.Translate(2.6f, 2, -0.5f);
		/*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
		modelStack.Rotate(90, 1, 0, 0);
		// Scale the back rest to its position
		modelStack.Scale(0.35, 0.35f, 0.35f);
		// Compute the MVP using projection, view and model
		MVP = projectionStack.Top() * viewStack.Top() *
			modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			&MVP.a[0]);
		// Render the chair part accordingly
		meshList[GEO_RANDSPHERECOLOR]->Render();
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	{
		modelStack.Translate(0, 0, 0);
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(3.1f, 0, 2.5f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.9f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(3.25f, - 0.1f, 2.5f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.9f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(3.4f, -0.2f, 2.5f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.9f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();

	// --------------------------------------------- DOOR ------------------------------------------------
	modelStack.PushMatrix();
	{
		modelStack.Translate(0, 0, 0);
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(3, 0.62f, 3);
			modelStack.Rotate(rotationY + 180, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 1.65f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(3, 0.62f, 2);
			modelStack.Rotate(rotationY + 180, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 1.65f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(3, 1.399f, 2.5f);
			modelStack.Rotate(180, 0, 1, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.9f, 0.1f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCUBE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(2.25f, 0.66f, 2.5f);
			modelStack.Rotate(rotationY + 90, 0, 0, 1.5f);
			// Scale the back rest to its position
			modelStack.Scale(1.3f, 1.5f, 0.9f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_RANDQUADCOLOR]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x + 0.05f, planePos.y - 0.4f, planePos.z);*/
			modelStack.Translate(3.05f, 0.66f, 2.35f);
			///*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
			//modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.08f, 0.08f  , 0.08f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLUEGREENSPHERE]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	{
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(-1.4f , 0.6f, 2.2f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.8f, 2.5f, 1.5f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_ORANGECUBE]->Render();
		}
		modelStack.PopMatrix();



		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(0, 0.2f, 2.2f);
			modelStack.Rotate(90, 1,0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.8f, 2.5f, 0.7f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_REDCUBE]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();



	modelStack.PushMatrix();
	{
		/*modelStack.Translate(planePos.x, 0, planePos.z);*/
		modelStack.Translate(3.05f, 0.55f, -4);
		modelStack.Rotate(90, 1.0f, 0.0f, 0.0f);
		modelStack.Scale(1, 1, 0.5f);
		modelStack.PushMatrix();
		{
			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.5, 0.8f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
	/*		modelStack.Translate(planePos.x - 0.5f, 3.5f, planePos.z + 0.2f);*/
			modelStack.Translate(-0.5f, 3.5f, 0.2f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		// ------------------------BARBELL----------------------
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(-0.5f, 3.5f, 0.5f);
			/*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.0f, 0.4f, 0.03f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CYLINDER]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	{
		/*modelStack.Translate(planePos.x, 0, planePos.z);*/
		modelStack.Translate(2.9f, 0.55f, -4);
		modelStack.Rotate(90, 1.0f, 0.0f, 0.0f);
		modelStack.Scale(1, 1, 0.5f);
		modelStack.PushMatrix();
		{
			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.5, 0.8f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
	/*		modelStack.Translate(planePos.x - 0.5f, 3.5f, planePos.z + 0.2f);*/
			modelStack.Translate(-0.5f, 3.5f, 0.2f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		// ------------------------BARBELL----------------------
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(-0.5f, 3.5f, 0.5f);
			/*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.0f, 0.4f, 0.03f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CYLINDER]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	{
		/*modelStack.Translate(planePos.x, 0, planePos.z);*/
		modelStack.Translate(3.05f, 0.55f, -3.8f);
		modelStack.Rotate(90, 1.0f, 0.0f, 0.0f);
		modelStack.Scale(1, 1, 0.5f);
		modelStack.PushMatrix();
		{
			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.5, 0.8f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
	/*		modelStack.Translate(planePos.x - 0.5f, 3.5f, planePos.z + 0.2f);*/
			modelStack.Translate(-0.5f, 3.5f, 0.2f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		// ------------------------BARBELL----------------------
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(-0.5f, 3.5f, 0.5f);
			/*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.0f, 0.4f, 0.03f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CYLINDER]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	{
		/*modelStack.Translate(planePos.x, 0, planePos.z);*/
		modelStack.Translate(3.05f, 0.55f, -3.6);
		modelStack.Rotate(90, 1.0f, 0.0f, 0.0f);
		modelStack.Scale(1, 1, 0.5f);
		modelStack.PushMatrix();
		{
			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.5, 0.8f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// --------------------- Cylinder for weights --------------------
			// Translate the back rest to its position
	/*		modelStack.Translate(planePos.x - 0.5f, 3.5f, planePos.z + 0.2f);*/
			modelStack.Translate(-0.5f, 3.5f, 0.2f);
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.1f, 0.1f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLACKCYLINDER]->Render();
		}
		modelStack.PopMatrix();

		// ------------------------BARBELL----------------------
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(-0.5f, 3.5f, 0.5f);
			/*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
			modelStack.Rotate(90, 1, 0, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.0f, 0.4f, 0.03f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_CYLINDER]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();


	// ----------------------------------------------- PLANTS -------------------------------------------------
	modelStack.PushMatrix(); {
		modelStack.Translate(-0.1f, -0.1f, 3);
		modelStack.Scale(0.2f, 0.2f, 0.1f);
		//PLANT POT
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.9f, 0.8f);
			modelStack.Rotate(180, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.5f, 0.6f, 1);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BROWNCYLINDER]->Render();
		}
		modelStack.PopMatrix();
		// PLANTS
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.8f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1.2f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1.4f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1.6f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.8f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.6f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.4f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.2f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.0f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.2f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.4f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.6f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();



	// ----------------------------------------------- PLANTS -------------------------------------------------
	modelStack.PushMatrix(); {
		modelStack.Translate(0.4f, -0.1f, 3);
		modelStack.Scale(0.2f, 0.2f, 0.1f);
		//PLANT POT
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.9f, 0.8f);
			modelStack.Rotate(180, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.5f, 0.6f, 1);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BROWNCYLINDER]->Render();
		}
		modelStack.PopMatrix();
		// PLANTS
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.8f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1.2f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1.4f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1.6f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.8f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.6f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.4f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.2f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.0f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.2f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.4f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.6f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();


	// ----------------------------------------------- PLANTS -------------------------------------------------
	modelStack.PushMatrix(); {
		modelStack.Translate(0.4f, -0.1f, 1.5f);
		modelStack.Scale(0.2f, 0.2f, 0.1f);
		//PLANT POT
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.9f, 0.8f);
			modelStack.Rotate(180, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.5f, 0.6f, 1);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BROWNCYLINDER]->Render();
		}
		modelStack.PopMatrix();
		// PLANTS
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.8f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1.2f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1.4f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1.6f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.8f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.6f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.4f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.2f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.0f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.2f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.4f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.6f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();




	// ----------------------------------------------- PLANTS -------------------------------------------------
	modelStack.PushMatrix(); {
		modelStack.Translate(-0.1f, -0.1f, 1.5f);
		modelStack.Scale(0.2f, 0.2f, 0.1f);
		//PLANT POT
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 3.9f, 0.8f);
			modelStack.Rotate(180, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.5f, 0.6f, 1);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BROWNCYLINDER]->Render();
		}
		modelStack.PopMatrix();
		// PLANTS
		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.8f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1.2f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1.4f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1.6f);
			modelStack.Rotate(45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 1);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.8f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.6f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.4f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.2f);
			modelStack.Rotate(-45, 0, 0, 1);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.0f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.2f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.4f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{

			// Translate the back rest to its position
			/*modelStack.Translate(planePos.x - 0.5f, 3.5, planePos.z + 0.8f);*/
			modelStack.Translate(-0.5f, 4.7f, 0.6f);
			modelStack.Rotate(-45, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.8f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_GREENCONE]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	{
		// Translate the back rest to its position
		modelStack.Translate(1.5f, 0.1f, 3);
		/*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
		modelStack.Rotate(90, 1, 0, 0);
		// Scale the back rest to its position
		modelStack.Scale(0.35, 0.35f, 0.35f);
		// Compute the MVP using projection, view and model
		MVP = projectionStack.Top() * viewStack.Top() *
			modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			&MVP.a[0]);
		// Render the chair part accordingly
		meshList[GEO_GREENSPHERE]->Render();
	}
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	{
		// Translate the back rest to its position
		modelStack.Translate(1.5f, 0.7f, 3);
		/*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
		modelStack.Rotate(90, 1, 0, 0);
		// Scale the back rest to its position
		modelStack.Scale(0.35, 0.35f, 0.35f);
		// Compute the MVP using projection, view and model
		MVP = projectionStack.Top() * viewStack.Top() *
			modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			&MVP.a[0]);
		// Render the chair part accordingly
		meshList[GEO_ORANGESPHERE]->Render();
	}
	modelStack.PopMatrix();




	modelStack.PushMatrix();
	{
		// Translate the back rest to its position
		modelStack.Translate(-0.5f, 0.4f, -0.5f);
		/*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
		modelStack.Rotate(90, 1, 0, 0);
		// Scale the back rest to its position
		modelStack.Scale(0.35, 0.35f, 0.35f);
		// Compute the MVP using projection, view and model
		MVP = projectionStack.Top() * viewStack.Top() *
			modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			&MVP.a[0]);
		// Render the chair part accordingly
		meshList[GEO_BLUEGREENSPHERE]->Render();
	}
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	{
		// Translate the back rest to its position
		modelStack.Translate(-0.5f, 1, -0.505f);
		/*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
		modelStack.Rotate(90, 1, 0, 0);
		// Scale the back rest to its position
		modelStack.Scale(0.35, 0.35f, 0.35f);
		// Compute the MVP using projection, view and model
		MVP = projectionStack.Top() * viewStack.Top() *
			modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			&MVP.a[0]);
		// Render the chair part accordingly
		meshList[GEO_REDSPHERE]->Render();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{
			modelStack.Translate(0.8f, 0, 1.2f);
			modelStack.PushMatrix();
		// PUNCHING BAG
		modelStack.PushMatrix();
		{
			// HANGING
			// Translate the back rest to its position
			modelStack.Translate(1.75f, 1.15f, -0.505f);
			/*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
			modelStack.Rotate(90, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.05f, 0.25f, 0.05f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_REDCYLINDER]->Render();
		}
		modelStack.PopMatrix();


		// BAG
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(1.75f, 0.7f, -0.5f);
			/*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
			modelStack.Rotate(90, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.5f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLUECYLINDER]->Render();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();


	// PUNCHING BAG
	modelStack.PushMatrix();
		{
		modelStack.Translate(0, 0, 2.5f);
		modelStack.PushMatrix();
		{
			// HANGING
			// Translate the back rest to its position
			modelStack.Translate(1.75f, 1.15f, -0.505f);
			/*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
			modelStack.Rotate(90, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.05f, 0.25f, 0.05f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_REDCYLINDER]->Render();
		}
		modelStack.PopMatrix();


		// BAG
		modelStack.PushMatrix();
		{
			// Translate the back rest to its position
			modelStack.Translate(1.75f, 0.7f, -0.5f);
			/*modelStack.Translate(planePos.x -0.5f, 3.5f, planePos.z + 0.5f);*/
			modelStack.Rotate(90, 0, 1, 0);
			// Scale the back rest to its position
			modelStack.Scale(0.2f, 0.5f, 0.2f);
			// Compute the MVP using projection, view and model
			MVP = projectionStack.Top() * viewStack.Top() *
				modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]);
			// Render the chair part accordingly
			meshList[GEO_BLUECYLINDER]->Render();
		}
		modelStack.PopMatrix();
		}
	modelStack.PopMatrix();

	}
	modelStack.PopMatrix();
}








	


void Scene1::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
		{
			delete meshList[i];
		}
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void Scene1::HandleKeyPress() 
{
	if (Application::IsKeyPressed(0x31))
	{
		// Key press to enable culling
		glEnable(GL_CULL_FACE);
	}
	if (Application::IsKeyPressed(0x32))
	{
		// Key press to disable culling
		glDisable(GL_CULL_FACE);
	}
	if (Application::IsKeyPressed(0x33))
	{
		// Key press to enable fill mode for the polygon
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	}
	if (Application::IsKeyPressed(0x34))
	{
		// Key press to enable wireframe mode for the polygon
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode
	}

	// codes for if w key or s key is pressed
	if (Application::IsKeyPressed('W'))
	{

		moveZ.Set(0.0f, 0.0f, -2.0f); // move inwards
	}
	else if (Application::IsKeyPressed('S'))
	{
		moveZ.Set(0.0f, 0.0f, 2.0f); // move forwards
	}


	// codes for if left or right key is pressed
	if (Application::IsKeyPressed(VK_LEFT))
	{
		// To add code to handle left key
		rotationSpeed = -45.0f;
	}
	else if (Application::IsKeyPressed(VK_RIGHT)) 
	{   
		// To add code to handle right key
		rotationSpeed = 90;
	}


	if (Application::IsKeyPressed(VK_UP)) {
		scaleSize = 5.0f;
	}
	else if (Application::IsKeyPressed(VK_DOWN)) {
		scaleSize = -5.0f;
		
	}

}
