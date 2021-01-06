<p align="center"><img src="https://raw.githubusercontent.com/oscarpm5/Phoebus_Engine/master/Phoebus_Engine/Game/Assets/our_pics/PhoebusIcon.png?token=ALMIHWWE2CTPST6X2FY4FMC77GRBO" alt="Phoebus Icon" class="center" align="center"></p>
   <br><br>

# Phoebus Engine
Welcome to Phoebus Engine! <br>
<p align="justify">This engine is made by two 3rd grade students of Design and Development of Videogames on CITM university (Terrassa, Barcelona, Spain) under the supervision and guidance of lecturer <a href="https://www.linkedin.com/in/mgarrigo/?originalSubdomain=es">Marc Garrigó</a>.</p> <br>

<p align="justify">The intent is to create a functional 3D engine with a focus on performance and Audio capabilities (hence the engine name). Because of that, we'll provide a deeper analysis of the audio system further down the web. <br>
If you wish to contact us or provide any kind of feedback, we'll gladly answer all your questions.<br>
We hope you'll enjoy using it as much as we enjoyed making it! </p>

## Check out our latest Release!

Link to code repo and release
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

<br><br>
<iframe width="510" height="412" src="https://github.com/oscarpm5/Phoebus_Engine/blob/master/docs/PHOEBUS_DEMO.mp4" frameborder="0" allowfullscreen> </iframe>
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
	
<br><br>

# Audio System
Audio detailed explanation (add at east 2 gifs)




<br><br>
# Meet the Team

<table>   
  <tr>
    <th>Member</th>
    <th>Portrait</th>
    <th>Linkedin</th>
    <th>Work</th>
  </tr>
  <tr> 
    <tr>
    <td><a href="https://github.com/oscarpm5">Oscar Pérez</a></td>
    <td><img src="https://raw.githubusercontent.com/oscarpm5/Phoebus_Engine/master/Phoebus_Engine/Game/Assets/our_pics/OPM.png?token=ALMIHWSZQ4YUMH2ZFV4HRWK77GR3U" alt="Oscar" class="inline"></td>
    <td> <a href="https://www.linkedin.com/in/oscar-p%C3%A9rez-mart%C3%ADn-94a3a5196/">Linkedin!</a></td>
    <td> Here goes what I've done </td>
    </tr>
    <tr>
    <td><a href="https://github.com/adriaserrano97">Adrià Serrano</a></td>
    <td><img src="https://raw.githubusercontent.com/oscarpm5/Phoebus_Engine/master/Phoebus_Engine/Game/Assets/our_pics/ASL.png?token=ALMIHWS2U63RUBCC2TWBUNK77GR32" alt="Adrià" class="inline"></td>
    <td><a href="https://www.linkedin.com/in/adri%C3%A0-serrano-l%C3%B3pez-7196a91a3/">Linkedin!</a></td>
    <td> Here goes what I've done </td>
    </tr>
   </tr>
</table>

<br><br>
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


