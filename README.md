# PHOEBUS ENGINE

Phoebus is a game engine made by two students as a 3rd year assigment in Bachelor’s Degree in Video Games by UPC at CITM, Terrassa, under the supervision of lecturer [Marc Garrigó](www.linkedin.com/in/mgarrigo/).



## Check our project!

Github:		https://github.com/oscarpm5/Phoebus_Engine


## Team Members:

### Adrià Serrano López
* [adriaserrano97](https://github.com/adriaserrano97)


### Oscar Pérez Martín
* [oscarpm5](https://github.com/oscarpm5)




## Controls:
Note that the 3D viewport controls will only work when interacting with the 3D view and won't work in the menus


* Right Mouse Click (hold)
	- Enter FPS mode
	
		* Right Mouse Click(drag)
			- Rotate view
	
		* WASD keys
			- Control camera movement
	
		* E/R Keys
			- Move camera Up & Down respectively
	
		* Left Shift
			- Multiply movement velocity


* F Key
	- Focus selected Object


* Middle Mouse Click (hold)
	- Pan view


* Middle Mouse Wheel (scroll)
	- Change Zoom in/out distance from the target


* Left Alt + Left Mouse Click (drag)
	- Orbit around target


* Delete Key (not backspace)
	- Deletes all selected objects and their children if any


* Esc: open/close the exit menu (this menu can be accessed also from the exit tab in the menu bar and by clicking the window cross)


* Left Mouse Click
	- Select/Deselect/Multiselect (it works on the hierarchy & the 3D Viewport)
	
		* Left Shift (hold)
			- Multiselect mode. Add object to selection.
	
		* Left Ctrl (hold)
			- Multiselect mode. Remove object from selection.


When not in Multiselect mode, selecting an object will remove all the previous selections. When using Left Mouse Click + Left Shift(hold) without clicking any object the previous selection will remain unchanged. When doing so without Left Shift (hold) will cause the selection to be erased (interpreting that the user wants to deselect all)

Holding Shift when dragging the Far/Near plane buttons in the config/camera panels allows extra precision.



* Q key
	- Hide Gizmos

* W key
	- Use Move Gizmo

* E key
	- Use Rotate Gizmo

* R key
	- Use Scale Gizmo


Specific Controls for controller component:

* I key
	- Move forward (positive Z)

* K key
	- Move forward (negative Z)

* J key
	- Move left (positive x)

* L key
	- Move right (negative x)

* Left Shift (hold)
	- Increase movement speed 3x




## Aditional functionality:

### First Assignment

* Unique Naming system that prevents two objects from having the same name by renaming them when needed. 

* Global Draw mode (each object can be drawn as Wire/Filled/Both independently but the scene has a "maximum draw type" that overwrites the object draw mode if it is more restrictive).
(Ex: wireframe cube - Global draw mode fill: All the meshes will be shown as filled except the cube. 
Ex2: filled cube - Global draw mode wireframe: All the meshes will be shown as wireframe including the cube)

* Smart 3D Viewport interaction: 3D Viewport knows when it is being interacted with and prevents interaction when on the menus.

* Inspector window has aditional features: Mesh normals size can be configured, Transform matrices are displayed, a preview of the texture is displayed and its size can be changed.

* From inspector, you can display normals of faces and vertex with resizable properties (+ draw modes)

* Dynamic grid that adjusts its tile size depending on the user distance.

* Game Objects can be deleted. Select the game obj and press Delete key. Note: it will also delete its childs

* Components can be deleted from the inspector (if the component is able to do so).

* Game objects can be moved, scaled & rotated (simple implementation just to avoid all the objects getting crammed in the world origin).

* Components & game objects can now be activated/ deactivated independently.

### Second Assignment

* Objects have an outline when selected, outline size can be configured via the config panel. A filled outline from the seleted object can be seen through other objects if they are in front of it.

* Depth buffer can be displayed in the editor cam. To enable this go to Menu -> config -> rendering and check "Depth display"

* Object Translate/Rotate/Scale can be done in both local & global space coordinates.

* Objects can be translated/rotated/scaled from the 3d View using gizmos. 

* The resource manager has an automatic garbage collector which deletes ".meta" & lib files when their asset is missing.

* Components can be added to an existing object through the "Add Component" button at the bottom part of the Inspector when a game object is selected.

* Material color & transparency of a selected object can be changed through the inspector.
 
### Third Assignment
* Objects support multiSelection from the hierarchy & 3D Viewport (see Controls section for more details).

## Aditional comments for the teacher:

### First Assignment

* A .ini file is included as it stores the information of how we want the windows to be displayed when the engine is opened for the first time.
* The license for DevIL is not included as it was not provided in the library files. 

### Second Assignment

* When tring to drag & drop external assets into the engine, make sure the path & the file don't have special caracters such as accents, otherwise the file won't be imported.
* Basic Forms (Primitives) have been disabled for this assignment due to meshes being converted to resources. They will be brought back in the future
* AABBs for all objects are showing by default. If you want to hide the AABBs for every object but the selected one go to the Config window and under Render settings un-check "Display All AABBs".
* AABBs & Camera frustum are visible even outside frustum of the culling cam to let the user know where they are.
* If you want to check the different time variable vaules(both the engine and in-game ones) go to the Config window -> "FPS".
* Mouse click Ray can be disabled in config -> rendering -> "DrawDebugCamRay". If you don't see anything when debug ray is active move the camera as the ray is pointing straight through the camera and is represented as just a pixel.
* Gizmos are only displayed for the selected object. If you don't have any selected objects no gizmo will be displayed. If an object is selected and the gizmo is still not being displayed press W, E or R. (you have pressed Q and hidden the gizmos by accident)
* Resources can be dragged from the Active Resource window & dropped into the Inspector->Component(the one that matches the dragged resource type)->"Used Resource" selectable to apply it to the object. 
* Resource Manager will check the Assets folder every 60 sec, if you need to view a file right away press the refresh button in the "Asset Explorer" window.
* You can add files only in offline mode my ddragging the file into the Asset folders and pressing the refresh button in the "Asset Explorer" window.
* In the exceptional case of an ".fbx" missing in assets, the garbage collector will handle the deletion of ".meta" & model files but won't delete ".mesh" & ".dds" files which originated from that asset as they are independent resources. The garbage collector will handle other asset types correctly. It executes once every minute or if "refresh" button is pressed. 
* You can manually save scenes in Menu->Save. They will be saved in Library/Scenes with an automatic name which you can later change. You can load any saved scene in Menu->Load
* Hierarchy elements can be re-parented through dragging them. If you want to re-parent the object to the scene root, simply drag it to the bottom and let it go in the blank space below the hierarchy.
* Pressing the outer white circle in the rotation gizmo when it turns orange will reset the object rotation to 0. 

### Third Assignment

* The project ".Zip" weighs more than the allowed 50Mb. This is due to the audio files inside the sound banks, which are pretty heavy (around 95 Mb - MainBank+InitBank) 
* Do NOT delete "wwise" folder located in "Assets/" because the audio system depends on it.
* The system where the user has to input the events names in the Audio Source component has been implemented to make the designer handle all the work and to not depend on a programmer when implementing sounds.
* You can add more than 1 Audio Source component for each object, that allows the sounds in a single object to be customized further as each component has its own settings.

## Licenses
* The license for the project is included in a separated .md file in the same directory as this readme.
* Licenses for 3rd party libraries and assets can be found under the "Libraries" folder.
