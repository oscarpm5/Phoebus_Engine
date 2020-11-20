#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"
#include "C_Transform.h"

#include "MathGeoLib/include/MathGeoLib.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	viewportClickRecieved = false;
	isGizmoInteracting = false;
	lastKnowMousePos = float2(-1.0f, -1.0f);
	//CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(2.0f, 2.0f, 2.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
	//foV = 60.0f;
	//nearPlaneDist = 0.125f;
	//farPlaneDist = 512.0f;

	editorCam = new C_Camera(nullptr);
	editorCam->SetNewFoV(60.0f);
	editorCam->SetNearPlane(0.125f);
	editorCam->SetFarPlane(500.0f);

}

ModuleCamera3D::~ModuleCamera3D()
{
	delete editorCam;
	editorCam = nullptr;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;
	//debugcamera = false;

	zoomLevel = 20;
	CamZoom(0);//sets cam zoom to its level variable

	camSpeed = 16.0f;
	camSpeedMult = 2.0f;


	//NEW
	editorCam->CalcCamPosFromDirections(
		float3(Position.x, Position.y, Position.z),
		float3(Z.x, Z.y, Z.z),
		float3(Y.x, Y.y, Y.z));


	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");
	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	if (App->editor3d->mouseActive&&!isGizmoInteracting)
	{

		float3 targetpos = { 0,0,0 };
		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			GameObject* target = nullptr;
			if (!App->editor3d->selectedGameObjs.empty())
			{
				target = App->editor3d->selectedGameObjs.back();
				targetpos = target->GetComponent<C_Transform>()->GetGlobalPosition(); //get global pos
			}
			MoveTo(vec3(targetpos.x, targetpos.y, targetpos.z), CamObjective::REFERENCE);
		}



		// Mouse motion ----------------
		if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT)
		{
			int dx = -App->input->GetMouseXMotion();
			int dy = App->input->GetMouseYMotion();

			float Sensitivity = sqrt(length(Position - Reference) * 0.005f) * 0.5f;/*Pow(2.0f,zoomLevel*0.075f)*0.005f;*/
			Sensitivity = max(Sensitivity, 0.05f);

			if (dx != 0)
			{
				float DeltaX = (float)dx * Sensitivity;
				Move(X * Sensitivity * DeltaX);
			}

			if (dy != 0)
			{
				float DeltaY = (float)dy * Sensitivity;
				Move(Y * Sensitivity * DeltaY);
			}



		}
		else if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)//Camera Rotate TODO: Camera rotate & camera orbit have duplicated code
		{
			vec3 newPos(0, 0, 0);
			float speed = camSpeed * dt;
			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
				speed *= camSpeedMult;

			if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos.y += speed;
			if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y -= speed;




			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

			Move(newPos);
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			float Sensitivity = 0.25f;

			Reference -= Position;

			if (dx != 0)
			{
				float DeltaX = (float)dx * Sensitivity;

				X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));//TODO rotate still works with gl math
				Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			}

			if (dy != 0)
			{
				float DeltaY = (float)dy * Sensitivity;

				Y = rotate(Y, DeltaY, X);
				Z = rotate(Z, DeltaY, X);

				if (Y.y < 0.0f)
				{
					Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
					Y = cross(Z, X);
				}
			}

			Reference = Position - Z * length(Reference);
		}
		else if (viewportClickRecieved && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_IDLE)
		{
			CreateRayFromScreenPos(lastKnowMousePos.x, lastKnowMousePos.y);
		}
		else if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)//camera orbit
		{
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			float Sensitivity = 0.25f;

			Position -= Reference;

			if (dx != 0)
			{
				float DeltaX = (float)dx * Sensitivity;

				X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			}

			if (dy != 0)
			{
				float DeltaY = (float)dy * Sensitivity;

				Y = rotate(Y, DeltaY, X);
				Z = rotate(Z, DeltaY, X);

				if (Y.y < 0.0f)
				{
					Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
					Y = cross(Z, X);
				}
			}

			Position = Reference + Z * length(Position);
		}

		if (App->input->GetMouseZ() != 0)
		{
			CamZoom(App->input->GetMouseZ());
		}



	}
	viewportClickRecieved = false;

	editorCam->CalcCamPosFromDirections(
		float3(Position.x, Position.y, Position.z),
		float3(Z.x, Z.y, Z.z),
		float3(Y.x, Y.y, Y.z));

	if (isGizmoInteracting)
		isGizmoInteracting = false;

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
/*
void ModuleCamera3D::Look(const vec3& Position, const vec3& Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}
}
*/
// -----------------------------------------------------------------
/*
void ModuleCamera3D::LookAt(const vec3& Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);
}
*/

// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3& Movement)
{
	Position += Movement;
	Reference += Movement;
}

void ModuleCamera3D::MoveTo(const vec3& Destination, CamObjective toMove)
{
	vec3 movement;

	if (toMove == CamObjective::REFERENCE)
		movement = Destination - Reference;
	else
		movement = Destination - Position;

	Move(movement);
}

void ModuleCamera3D::CamZoom(int addZoomAmount)
{
	zoomLevel -= addZoomAmount;
	if (zoomLevel < 0)zoomLevel = 0;

	Position = Reference + (Z * (Pow(2.0f, zoomLevel * 0.1f) - 1));

}

void ModuleCamera3D::CreateRayFromScreenPos(float normalizedX, float normalizedY)
{
	LineSegment picking = editorCam->GetFrustum().UnProjectLineSegment(normalizedX, normalizedY);

	App->editor3d->TestRayHitObj(picking);
}

// -----------------------------------------------------------------
/*
float* ModuleCamera3D::GetRawViewMatrix()
{
	CalculateViewMatrix();
	return &ViewMatrix;
}
*/
/*
mat4x4 ModuleCamera3D::GetViewMatrix()
{
	CalculateViewMatrix();
	return ViewMatrix;
}
*/

// -----------------------------------------------------------------
/*
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}
*/
