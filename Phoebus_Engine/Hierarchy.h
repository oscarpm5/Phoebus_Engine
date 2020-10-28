#pragma once
#include <vector>
#include <string>

#define ACTIVE_COLOR (1.0f, 1.0f, 1.0f, 1.0f)
#define PASIVE_COLOR (0.7f, 0.7f, 0.7f, 0.7f)

#define SELECTED_COLOR (1.0f, 0.25f,0.0f,0.1f) //all of selected objs
#define FOCUSED_COLOR (1.0f, 0.5f, 0.0f, 1.0f) //focused color of last selected obj
 

class GameObject;
struct ImVec4;

//value ret tied to SHow Hierarchy
bool ShowHierarchyTab(); 

//This is the recursive func that makes the hierarchy
void SeekMyChildren(GameObject* myself); 

//Quick func that return the color vector based on IsActive
ImVec4 ChooseMyColor(GameObject* myself);

