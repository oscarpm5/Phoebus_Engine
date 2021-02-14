<p align="center"><img src="https://raw.githubusercontent.com/oscarpm5/Phoebus_Engine/master/Phoebus_Engine/Game/Assets/our_pics/PhoebusIcon.png?token=ALMIHWWE2CTPST6X2FY4FMC77GRBO" alt="Phoebus Icon" class="center" align="center"></p>
   <br><br>

# Phoebus Engine
Welcome to Phoebus Engine! <br>
<p align="justify">This engine is made by two 3rd grade students of Design and Development of Videogames on CITM university (Terrassa, Barcelona, Spain) under the supervision and guidance of lecturer <a href="https://www.linkedin.com/in/mgarrigo/?originalSubdomain=es">Marc Garrigó</a>.</p> <br>

<p align="justify">The intent is to create a functional 3D engine with a focus on performance and Audio capabilities (hence the engine name). Because of that, we'll provide a deeper analysis of the audio system further down the web. <br>
If you wish to contact us or provide any kind of feedback, we'll gladly answer all your questions.<br><br>
We hope you'll enjoy using it as much as we enjoyed making it! </p>
<br>
## Check out our latest Release!

[Check it here!](https://github.com/oscarpm5/Phoebus_Engine/releases/tag/1.0)
<br><br>

# Engine Overview
Although our focus is in the Audio System, Phouebus Engine does have the basic utilities you would hope to find in an Engine:
- Game Object structure
	- Loading external FBX and saving them as Game Objects
	- Material, Scale, etc. can be changed in-engine
	- Info is saved in a ".pho" JSON which the user has access to
- Component System to customize your Game Objects
- Scene viewer and editor
- Real time rendering (scene managed: pause - play - stop)

[Engine Showcase](https://www.youtube.com/watch?v=MWt5GjqJqls)
<br><br>
[Audio System demonstration](https://www.youtube.com/watch?v=HzbuzyZ3If8&ab_channel=Adri%C3%A0Serrano)

<br><br>

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

Holding Shift when dragging the Far/Near plane buttons in the config/camera panels allows extra precision.



* Q key
	- Hide Gizmos

* W key
	- Use Move Gizmo

* E key
	- Use Rotate Gizmo

* R key
	- Use Scale Gizmo
	
If you have a Component Control on a Game Object AND Engine is Playing:

* I
	-Move fwd
* J
	-Move left
* K
	-Move back
* L
	-Move right
	
<br><br>

# Audio System
<p align="justify">Phoebus Engine has a focus on it's audio department. Thanks to a Wwise integration in our engine, we're able to use funcionts and structures derived from the Wwise application directly into our engine. In order to do so, we've implemented the following components and systems:</p>
<br>
## Listener
<p align="justify">The Listener component can be assigned to any Gaame Object to (you guessed it) listen to the audios in scene. In games, this component would we assigned to the player character (plural, if more than one player). If thaat's the case, a passive-active listener hierarchy has to be implemented in-game. We've already taken care of that problem for you; and the listener component can be flagged both as Active/Nonactive and ActiveListener/PassiveListener.</p>
<br>
## Source
<p align="justify">The Source component correlates to a sound emitter. This sounds (be it SFX or Music) must have been pre-stablished in your Wwise project previously linked tom the engine. Once those sounds have been defined in Wwise, you only need to create a Play-Type event inside Wwise. The *name* of the event is what you need to type into this component. The engine will handle all the rest.</p>
<br>
![](/House.gif)
<br>
<p align="justify">In order to make some adjustment possibles in-engine, we've added some utility functions and options:</p>
### Volume
<p align="justify">Each sound/music has it's own volume, apart from the total volume of the user. Real volume will be calculated taking both into account (+ possible effects applied)</p><br>
### Pitch
<p align="justify">Although the Engine has a PlaySpeed option (config->FPS) if you want to implement slow motion mechanics, each audio has it's own self-pitch RTCP. This makes it possible to slow down select sounds while keeping others normal (if you want to have music keep the same, or have slow-mo effects apply only to some characters).</p><br>
### Blend
<p align="justify">We have a pre-determined Blend RTCP into our music tracks, so you can decide when does a transition occur. By default, this transition is linear, but it can be changed and modified inside the Wwise project.</p><br>
## Reverb Zone / Mod Zone
<p align="justify">This component defines an area in-game in which sounds reverberate. The "type" of reverberation is defined in Wwise using an auxiliary Bus, but things such as shape, volume or intensity of effect can be controlled in-game. </p><br>
![](/Revzone.gif)
<br>
<p align="justify">This component can be used to apply effects that are not reverb effects; to do so, create a new auxiliary bus in your Wwise project and apply your wished effects. After that, include the related plugins in-engine. Add the reverb component and change the bus from default to the name of the bus you created (remember to update soundbanks). Phoebus engine will detect the bus and use it as the effect modifier instead of a classic reverb.</p>


<br><br>
# Meet the Team

<table>   
  <tr>
    <th>Member</th>
    <th>Portrait</th>
    <th>Linkedin</th>
  </tr>
  <tr> 
    <tr>
    <td><a href="https://github.com/oscarpm5">Oscar Pérez</a></td>
    <td><img src="https://raw.githubusercontent.com/oscarpm5/Phoebus_Engine/master/Phoebus_Engine/Game/Assets/our_pics/OPM.png?token=ALMIHWSZQ4YUMH2ZFV4HRWK77GR3U" alt="Oscar" class="inline"></td>
    <td> <a href="https://www.linkedin.com/in/oscar-p%C3%A9rez-mart%C3%ADn-94a3a5196/">Linkedin!</a></td>
    </tr>
    <tr>
    <td><a href="https://github.com/adriaserrano97">Adrià Serrano</a></td>
    <td><img src="https://raw.githubusercontent.com/oscarpm5/Phoebus_Engine/master/Phoebus_Engine/Game/Assets/our_pics/ASL.png?token=ALMIHWS2U63RUBCC2TWBUNK77GR32" alt="Adrià" class="inline"></td>
    <td><a href="https://www.linkedin.com/in/adri%C3%A0-serrano-l%C3%B3pez-7196a91a3/">Linkedin!</a></td>
    </tr>
   </tr>
</table>

<br><br>

# Work division

Disclaimer: Making an Engine (at our currelt level) proved to demand an ungodly amount of time and effort. Not a single feature was done alone without the help, guidance and support of the other member. Nevertheless, some systems were "assigned" to one individual to take the blunt of the workload and the responsability of that feature's functioning. These assigments are as follows:

* Importer - Adrià
* File System - Adrià
* Component System - Oscar
* Primitives - Adrià
* OpenGL implementation - Oscar
* Wwise Implementation - Oscar
* Wwise project - True 50/50
* Engine Configuration Options - True 50/50
* Game Object Structure - Oscar
* Camera work - Oscar
* Resource Manager - True 50/50
* Scene editor - True 50/50

# License
MIT License

Copyright (c) 2020 oscarpm5

<p align="justify">Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:</p>

<p align="justify">The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.</p>

<p align="justify">THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.</p>


