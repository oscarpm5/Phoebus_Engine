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
	- Deletes the selected object and its children if any


* Esc: open/close the exit menu (this menu can be accessed also from the exit tab in the menu bar and by clicking the window cross)


To select an object click in the desired object in the Hierarchy Tab (it will turn orange). 
Note that objects with children in the hierarchy will have to be deployed before being selected.

Holding Shift when dragging the Far/Near plane buttons in the config/camera panels allows extra precision.


## Aditional functionality:

### First Assigment

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

### Second Assigment

* Objects have an outline when selected, outline size can be configured via the config panel.

* [TODO] Still WiP Object Translate/Rotate/Scale can be done in both local & global space.

 
## Aditional comments for the teacher:

### First Assigment
* A .ini file is included as it stores the information of how we want the windows to be displayed when the engine is opened for the first time.
* The license for DevIL is not included as it was not provided in the library files. 
* The two .fbx included in this project (Poste.fbx & Espada.fbx are not directly included in the project as they are intended to be drag & dropped into the engine from the assets folder)
* Be careful when rotating a game object! This feature is still work in progress and can mess with the scale.

### Second Assigment
* Only the AABB of the select object is showing by default. If you want all the object to show their AABB go to the Config window and under Render settings check "Display All AABBs"


## Licenses
* The license for the project is included in a separated .md file in the same directory as this readme.
* Licenses for 3rd party libraries and assets can be found under the "Libraries" folder.
