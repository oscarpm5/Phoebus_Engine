#include "GameObject.h"
#include "Component.h"
#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Material.h"
#include "C_Camera.h"
#include "Application.h"
#include "imgui/imgui.h"

#include "MathGeoLib/include/MathGeoLib.h"
//#include "glmath.h"//new, we should include glMath here but C_Transform already has it and we cannot remove it from there


int GameObject::numberOfObjects = 0;


GameObject::GameObject(GameObject* parent, std::string name, float4x4 transform, bool showAABB, bool isLocalTrans) :name(name), transform(nullptr), focused(false),selected(false), displayBoundingBox(showAABB)
{
	App->editor3d->AddObjName(this->name);
	this->parent = parent;
	isActive = true;
	bbHasToUpdate = true;
	if (parent)
	{
		parent->children.push_back(this);
	}
	this->transform = new C_Transform(this, transform, 0, isLocalTrans);
	components.push_back(this->transform);

	numberOfObjects++;

	globalAABB.SetNegativeInfinity();
	globalOBB.SetNegativeInfinity();

	this->ID = App->renderer3D->seed.Int(); //begone T H O T
}

void GameObject::Awake()
{
}

void GameObject::Update(float dt)
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i]->toDelete)
		{
			delete components[i];
			components[i] = nullptr;
			components.erase(components.begin() + i);
			i--;
		}
	}

	if (isActive)
	{

		for (int i = 0; i < components.size(); i++)
		{
			if (components[i]->IsActive())
			{
				components[i]->Update(dt);
			}
		}



		for (int i = 0; i < children.size(); i++)
		{
			if (children[i]->isActive)
			{
				children[i]->Update(dt);
			}
		}

		if (bbHasToUpdate)
		{
			UpdateBoundingBox();
		}
		DrawGameObject();
	}
}

GameObject::~GameObject()
{
	App->editor3d->RemoveName(name);

	numberOfObjects--;

	for (int i = 0; i < components.size(); i++)
	{
		if (components[i] != transform)
		{
			delete components[i];
			components[i] = nullptr;
		}
	}
	components.clear();

	if (transform != nullptr)//This wont be needed as transform is deleted from the component vector
		delete transform;

	transform = nullptr;

	while (!children.empty())
	{
		GameObject* child = children.back();
		children.pop_back();
		delete child;
	}
	children.clear();

	RemoveMyselfFromParent();

	if (App)
		App->editor3d->RemoveGameObjFromSelected(this);

}

void GameObject::RemoveChildren(GameObject* toRemove)
{
	std::vector<GameObject*>::iterator iterator = children.begin();

	for (iterator; iterator != children.end(); iterator++)
	{
		if (*iterator == toRemove)
		{

			children.erase(iterator);

			break;
		}

	}

}

void GameObject::ChangeParent(GameObject* newParent)
{
	RemoveMyselfFromParent();
	//newParent->children

	if (newParent == nullptr)
	{
		parent = App->editor3d->root;
	}
	else
	{
		parent = newParent;
	}

	parent->children.push_back(this);

	C_Transform* transform = GetComponent<C_Transform>();
	transform->UpdateLocalMat();
	UpdateChildTransforms();

}

void GameObject::RemoveMyselfFromParent()
{
	if (parent)
	{
		parent->RemoveChildren(this);
	}
}

Component* GameObject::CreateComponent(ComponentType type, unsigned int compID)
{
	Component* ret = nullptr;
	//TODO add diferent components here
	switch (type)
	{
	case ComponentType::MESH:
		ret = new C_Mesh(this, compID);
		break;
	case ComponentType::MATERIAL:
		//only one instance of material for a certain gameObj
		if (GetComponent<C_Material>() == nullptr)
			ret = new C_Material(this, compID);
		break;
	case ComponentType::CAMERA:
		//only one instance of camera for a certain gameObj
		if (GetComponent<C_Camera>() == nullptr)
			ret = new C_Camera(this, compID);
		break;
	default:
		break;
	}


	if (ret)
	{
		components.push_back(ret);
	}

	return ret;
}

std::string GameObject::GetName()
{
	return this->name;
}

//recursive function that returns false if some game object from which this inherits is not active 
bool GameObject::IsParentActive()
{
	if (!isActive)
		return false;

	if (parent)
	{
		return parent->IsParentActive();
	}

	return isActive;
}

void GameObject::GetChildWithID(unsigned int ID, GameObject*& childOut)//Note that child out must be nullptr at the start
{
	if (this->ID == ID)
	{
		childOut = this;
	}
	else
	{
		for (int i = 0; i < children.size(); i++)
		{
			if (childOut == nullptr)
			{
				children[i]->GetChildWithID(ID, childOut);
			}
		}
	}
}

void GameObject::UpdateChildTransforms()
{
	GetComponent<C_Transform>()->UpdateGlobalMat();
	bbHasToUpdate = true;
	for (int i = 0; i < children.size(); i++)
	{
		children[i]->UpdateChildTransforms();
	}
}

void GameObject::UpdateBoundingBox()
{
	C_Mesh* mesh = GetComponent <C_Mesh>(); //TODO for now we will only make it that the first mesh draws the bounding box, add support for multiple boundingboxes (if more than 1 mesh)

	if (mesh != nullptr)
	{
		globalOBB = mesh->GetAABB();
		globalOBB.Transform(GetComponent< C_Transform>()->GetGlobalTransform());

		globalAABB.SetNegativeInfinity();
		globalAABB.Enclose(globalOBB);
	}
	else
	{
		globalAABB.SetNegativeInfinity();
		globalAABB.SetFromCenterAndSize(transform->GetGlobalPosition(), float3(1, 1, 1));
		globalOBB = globalAABB;
	}



	bbHasToUpdate = false;
}

void GameObject::GetObjAndAllChilds(std::vector<GameObject*>& childs)
{
	childs.push_back(this);

	for (int i = 0; i < children.size(); i++)
	{
		children[i]->GetObjAndAllChilds(childs);
	}


}

AABB GameObject::GetWorldAABB() const
{
	return globalAABB;
}

void GameObject::DrawOnEditorAllComponents()
{
	if (ImGui::BeginChild("Object name window", ImVec2(0.0f, 30.0f)))
	{
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Checkbox(" ", &isActive);
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Active");
			ImGui::EndTooltip();
		}


		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 10);
		char* auxName = (char*)name.c_str();
		std::string oldName = name;
		if (ImGui::InputText("Object Name", auxName, 100))
		{
			name = auxName;
			name.shrink_to_fit();
			if (name.empty())
			{
				name = "Untitled";
			}
			App->editor3d->ChangeObjName(oldName, name);
		}
	}
	ImGui::EndChild();
	//ImGui::Text("My name is %s", name.c_str());
	ImGui::Separator();
	ImGui::Spacing();


	for (int i = 0; i < this->components.size(); i++)
	{
		components[i]->OnEditor();

	}

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Separator();

	int buttonWidth = 150;
	float wWidth=ImGui::GetWindowWidth();
	ImVec2 cursPos = ImGui::GetCursorPos();
	cursPos.x = cursPos.x + (wWidth * 0.5f)-(buttonWidth*0.5f); //60 is half button width
	ImGui::SetCursorPos(cursPos);

	if (ImGui::Button("Add Component##objComponent", ImVec2(buttonWidth, 20)))
	{
		ImGui::OpenPopup("AddComponent##Popup");

	}
	ImGui::SameLine();
	if (ImGui::BeginPopup("AddComponent##Popup"))
	{
		ImGui::Text("Select New Component to Add");
		ImGui::Separator();

		//TODO this can be made pretty in the future
		if (GetComponent<C_Mesh>() == nullptr)//support multiple meshes in the future?
		{
			if(ImGui::Selectable("Mesh Component##addComponent"))
			CreateComponent(ComponentType::MESH);
		}
		if (GetComponent<C_Material>() == nullptr)
		{
			if(ImGui::Selectable("Material Component##addComponent"))
			CreateComponent(ComponentType::MATERIAL);
		}
		if (GetComponent<C_Camera>() == nullptr)
		{
			if(ImGui::Selectable("Camera Component##addComponent"))
			CreateComponent(ComponentType::CAMERA);
		}



		ImGui::EndPopup();
	}
}

std::vector<Component*> GameObject::GetAllComponents()
{
	return this->components;
}

void GameObject::DrawGameObject()
{
	std::vector<float3> aabbVec;
	GetPointsFromAABB(globalAABB, aabbVec);

	if (App->renderer3D->IsInsideFrustum(aabbVec))
	{


		std::vector<C_Mesh*>meshes = GetComponents<C_Mesh>();

		C_Material* mat = GetComponent<C_Material>();
		if (mat != nullptr && !mat->IsActive())
		{
			mat = nullptr;
		}

		for (int i = 0; i < meshes.size(); i++)
		{
			if (meshes[i]->IsActive() && meshes[i]->GetMesh() != nullptr)
			{
				App->renderer3D->AddMeshToDraw(meshes[i], mat, transform->GetGlobalTransform(), selected);
			}

		}


	}

	if ( App->editor3d->root!=this &&(App->renderer3D->displayAABBs || (displayBoundingBox && (focused||selected))))
	{
		Color c = Color(1.0f,1.0f,1.0f,1.0f);
		if (focused)
			c = Color FOCUSED_COLOR;
		else if (selected)
			c = Color SELECTED_COLOR;

		App->renderer3D->AddBoxToDraw(aabbVec, c);
	}

	C_Camera* cam = GetComponent<C_Camera>();
	if (cam&&cam->IsActive())
	{
		std::vector<float3> vec;
		cam->GetFrustumPoints(vec);
		Color c = Color(1.0f, 1.0f, 1.0f, 1.0f);
		if (cam->GetIsCulling())
			c = Color(0.0f, 0.75f, 0.75f, 1.0f);

		App->renderer3D->AddBoxToDraw(vec,c);
	}
}

//Takes an aabb and fills empty vector with its points
void GameObject::GetPointsFromAABB(AABB& aabb, std::vector<float3>& emptyVector)
{
	float3* frustrumPoints = new float3[8];
	memset(frustrumPoints, NULL, sizeof(float3) * 8);
	aabb.GetCornerPoints(frustrumPoints);

	emptyVector.clear();

	for (int i = 0; i < 8; i++)
	{
		emptyVector.push_back(frustrumPoints[i]);
	}
	delete[]frustrumPoints;
	frustrumPoints = nullptr;
}
