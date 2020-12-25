TABLE OF CONTENT
    1-  EXTRA FEATURES!!!
    2- 	PROJECT ABSTRACT
    3- 	GENERAL SPECIFICATIONS
    4- 	DELIVERABLES
    5- 	ASSUMPTIONS
    6- 	CONFIGURATION
    7- 	USER NAVIGATION AND CONTROLS AND CONTROLS
    8- 	NOTES
    9- 	REFERENCES (SOURCES)
    10- PROJECT TEAM
    
NOTE: 
- IF OPENING THE SOLUTION FILE IN VISUAL STUDIO, PLEASE ASSIGN YOUR CONFIRUGATION MODE TO DEBUG X86 TO RUN THE CODE; THE LIBRARIES ARE 32 BIT
    THERE IS A RELEASE_FOLDER INTENDED FOR DEPLOYMENT TO USERS IF NECESSARY FOR THE ASSIGNMENT, WHERE YOU CAN RUN THE PROGRAM BY OPENING THE .EXE
- ALL HEADER AND .CPP FILES ARE CONTAINED IN THE PA1_PRACTICE FOLDER
- THE DEAR IMGUI IMPLEMENTATION IS FOR DEBUGGING PURPOSES AS WELL AS DISPLAYING THE EXTRA FEATURES

1- EXTRA FEATURES!!!

  1.1- Course-related extra features
	- POINT-LIGHT SHADDOW MAPPING WITH A CUBEMAP DEPTH TEXTURE
	- STENCIL OUTLINE FOR SELECTED MODELS: Inspired by the stencil buffer mentioned in lecture 7, we implemented a 'stencil outline' around the currently selected models using the stencil buffer

  1.2- Cool extra features
			
	- RAY CAST MOUSE PICKING!: All of our alphanumeric models can be selected using mouse picking and ray casting (just by clicking). 
        			   Multiple models can be selected at once. When any control to orient or move a model is used, it affects all selected models. They can be de-selected by clicking them again.

	- GUI WITH LOTS OF COOL FEATURES!: 
		We used the Dear ImGui library to create a on-screen GUI with drop-down menus that control a host of extra features:
		1. The point light's position and color can be customized with sliders and color pickers
		2. A wave function can be applied to the height values for the vertices of the grid to create a wave effect:
			2.1. The function used to create the noise can be selected ('None', 'Perlin', 'Fractional Brownian Motion X Perlin (FBM)')
			2.2. The waves' magnitude and frequency/spread across the grid (density/sparsity of the waves relative to world coordinates across the grid)
			2.3. Texture movement and speed - the rate of change in the texture coordinate positions in the world's X and Z directions can be altered with a slider
			2.4. NOTE: This isn't a 100x100 grid anymore! It's just a cool feature. A height map can be selected to be mapped to the grid's vertices with differing resolutions (1024,512,256,128); the height maps were generated at https://cpetry.github.io/TextureGenerator-Online/
			     The height of the height map can be scaled using the height scaling factor slider.
		3. A submenu called 'Metrics' which currently displays the current FPS/frame rate.

	- A CUBIC 'ROOM' SURROUNDING THE GRID TO HELP DISPLAY OUR SHADOWS!:
		1. We created a 3D room around the 100x100 grid to better demonstrate the point-shadowing effects. 
		   There are walls and a ceiling which are cubes transformed into the shape of prisms with shading properties defined by the material class.
		2. We created a 'return to bounds' function for fun, and for practicality, so that the models always teleport back into the room when they move throw a wall 
		   eg. If a model goes through the wall at -X, it returns moving in the same direction through the +X wall, as though the walls are portals.
  
  1.3- Extra features on top of requirements

	SHADOWS
	- We implemented a depth cubemap texture with a geometry shader and projectional mapping to create realistic 'point-light-based' shadow mapping instead of the simple two-pass directional light shadow mapping.
	
	SPHERE
	- The transparent sphere's shadow can be toggled by pressing upper case (CAPS_LOCK or LEFT/RIGHT SHIFT) 'B' (we weren't sure if a transparent sphere should have a shadow)

	LIGHTING
	- The point light source's position and ambient, diffuse, and specular colors can be changed in real-time in the on-screen GUI drop-down menu
	- The light source is textured with a white moon image
	- Added a linear and quadratic attenuation factor to the lighting calculations
 	
	TEXTURES
	- The sphere for everyone's model has a unique texture which is also toggled with 'X' 
	- The ground mesh has a specular map texture multipled with the specular light result in the shader, which makes the white parts of the tiles shinier
	- The box texture is a 'wood' texture, while the digit is given a shiny metal finish with a material class we implemented

	WINDOW RESIZING
	- The window re-centers the gl viewport (main 'rendering' frame) to the center of the window, as well as keeping the aspect ratio as per requirements

2- PROJECT ABSTRACT
    This deliverable (PA2) is the second part of the term project divided in 3 deliverables (PA1, PA2 and PA3). The goal of the project is to learn practical aspects of OpenGL programming through a series of programming 
    assignments.
    PA1 implements the following requirements:
    - Create a simple virtual scenes consisting of objects. 
    - Display them as wireframe meshes by drawing triangles
    - Set up a virtual camera  and the necessary controls to view the scene from varied angles and distances.
    - Setup the necessary controls to manipulate the different models.
	
    PA2 builds upon PA1 and implements the following requirements:
    - Lighting using the Phong model.
    - Add textures to models.
    - Generate shadows. 
    - Hierarchical model motion.
	
3- GENERAL SPECIFICATIONS (SEE PROJECT DESCIPTION FOR DETAILED SPECIFICATIONS)
    PA1
    - Creates a 100x100 square grid (ground surface) in the XZ plane centered at the origin.
    - Window size must be 1024x768 with double buffering support.
    - One model for each team member must be created according to specifications indicated in project description document.
    - Models must be independent and be placed in the four corners of the grid; the fifth model is initially positioned at the center of the grid, aligned along the X axis.
    - The application must use OpenGL 3.1 and onwards and must include brief comments explaining each step.
	- Enclose the entire model in a big sphere with lower body of model outside the sphere.
	PA2
	- Illuminate the scene by adding light source (white) 30 units above model using Phong model.
	- Render the scene with tiled texture on the ground mesh, box texture on the alphabet and shiny metal finish (color of your choice) for the digit (set key `X' for toggling texture on/off).
	- Render the scene with shadows using the two-pass shadow algorithm (set key `B' to toggle).
	- Make the model move by moving the segments touching the ground (lower half of the model), while also providing either lateral/vertical movement to the upper half of the model. (
	  One key of your choice for one small forward movement, and other key for continuous forward movement. Same for reverse movement).
	- Make the model turn right or left by a small amount (choose your own keys and the amount).

4- DELIVERABLES
	- General specifications and requirements of PA1 and PA2 must be met.
	- All features and functionalities of PA1 should work as before
	- Navigation and controls:
		- Pressing the spacebar should re-position the model at a random location on the grid.
		- The user can incrementally size up the Model by pressing `U' for scale-up and `J' for scale-down. Each key press should result in a small size change (define delta change in a variable).
		- The user can control the Model position and orientation using keyboard input, i.e., `A' -> move left, `D' -> move right, `W' -> move up, `S' -> move down, a -> rotate left 5 degrees 
		  about Y axis, `d' -> rotate right 5 degrees about Y axis. You may add other rotations about other axis, if you want.
		- The world orientation is changed by using keyboard input, i.e., left arrow -> Rx, right arrow -> R-x, up arrow -> Ry, down arrow -> R-y. (Rx denotes a small anti- clockwise rotation 
		  about positive x axis, R-x about negative x axis, etc.). Pressing the `Home' button should reset to the initial world position and orientation.
		- A user can change rendering mode for the Model, i.e,. points, lines, or triangles, based on keyboard input, i.e., use key `P' for points, key `L' for lines, key `T' for triangles.
		- The user can pan and tilt the camera as follows:
			1- While right button is pressed ! use mouse movement in x direction to pan and
			2- While middle button is pressed ! use mouse movement in y direction to tilt.
		- The user can zoom in and out of the scene - while left button is pressed -> use mouse movement to move into/out of the scene.
		- Window resize handling: The application should handle window resize events and correctly adjust the aspect ratio accordingly. This means, the meshes should not be distorted in anyway.

5- OUR ASSUMPTIONS
    Assumptions we've made about the instructions:
    - Whenever "the model" is mentioned, we assume that the instructions are referring to an individual instance of one of the group member's letter & number model, such as the 'U9' model in 
      the instructions. As such, we made it possible for the user to select any of the five models on the grid by pressing one of the corresponding function keys between 'F1' and 'F5', 
      as described in the user navigations and controls section (eg. Press F1 to control group member's 1 model, F2 to control member 2's model, etc.). Once a model is selected, the movement, 
      scaling, and rotation controls are bound to the selected model until it has been de-selected. When a group member's model is selected, its color changes to show it is currently being controlled by the user.
    - Similarly, when "the world" is described, we assume that this refers to all models in the scene, including the grid and coordinate axes. When the arrow keys are used to change world orientation, the 
      world matrix is rotated to affect all models at once.
    - Forward/backward and general movement of the models is relative to the models' own coordinate axes - not the world's. For example, moving a model forward will move it forward towards where it is currently facing.
    
6- REQUIREMENTS/CONFIGURATION
    The project is developped on Visual Studio 2017 and requires installation and configuration of the following libraries:
    - GLFW-3.3.2
    - GLEW-2.1.0
    - GLM-0.9.9.8
    
7- USER NAVIGATION AND CONTROLS

ASSIGNMENT 2
     -  MODEL CONTROL

  	MOUSE
	Left clicking on a model selects or deselects it

        KEYS
	Note: All controls for model movement, orientation, scaling, etc. affect all selected models
	. 'Spacebar'     -> Re-positions the model(s) at a random location on the grid (X-Z plane)
	.  Uppercase F   -> Continuous movement in the negative z axis for the model ('forward')
	.  Lowercase F   -> One small movement forward
	.  Uppercase G   -> Continuous movement in the positive z axis for the model ('backward')
	.  Lowercase G   -> One small movement backward
     -  ENABLING/DISABLING SHADOWS & TEXTURES
	.  'X'           -> Disable/enable texturing for all models in the world/scene
        .  Lowercase B   -> Disable/enable all shadows
        .  Uppercase B   -> Disable/enable shadow for the transparent spheres on our models

ASSIGNMENT 1 AND ASSIGNMENT 1 EXTENSIONS 

    - MODEL SELECTION
        . '1' -> Select/deselect Nick's Model (Center)
	. '2' -> Select/deselect Mohamad's Model (Back Left)
	. '3' -> Select/deselect Adrien's Model (Back Right)
	. '4' -> Select/deselect Sami's Model (Front Right)
	. '5' -> Select/deselect Ale's Model (Front Left)

    - RENDERING MODES
      All models are drawn with the GL_TRIANGLES primitive by default. Pressing the following keys will change the rendering mode:
        . 'P' -> Render/draw all models using the GL_POINTS primitive.
        . 'L' -> Render/draw all models using the GL_LINES primitive.
        . 'T' -> Render/draw all models using the GL_TRIANGLES primitive.
    
        To scale the selected model, use the following keys:
        . U -> Scape Up by 2%
        . J -> Scale Down by 2%
        
        The user can control the selected model's position and orientation by using the following keys:
        . Uppercase A -> Move left (the negative x direction of that model).
        . Lowercase a -> Rotate left by 5 degrees (the positive y axis of that model).
        . Uppercase D -> Move right (the positive x direction of that model).
        . Lowercase d -> Rotate right by 5 degrees (the negative y axis of that model).
        . Uppercase W -> Move up (the positive y direction of that model).
        . Lowercase w -> Rotate around the positive x axis by 5 degrees.
        . Uppercase S -> Move down (the negative y direction of that model).
        . Lowercase s -> Rotate around the negative x axis by 5 degrees.
        
    - WORLD ORIENTATION
        . Left arrow    -> Rotate the world 1 radian CCW about the positive x axis.
        . Right arrow   -> Rotate the world 1 radian CCW about the negative x axis.
        . Up arrow      -> Rotate the world 1 radian CCW about the positive y axis.
        . Down arrow    -> Rotate the world 1 radian CCW about the negative y axis.
        . HOME          -> Reset the world matrix to its original orientation, and reset the camera's position, pitch, yaw, field of view and focus/viewing angle to their original values.
        
    - PAN AND TILT
        . With the right mouse button pressed, movement in the x direction of the window will pan the camera accordingly.
        . With the middle mouse button pressed, movement in the y direction of the window will tilt the camera accordingly.
        
    - ZOOM
        With the left mouse button pressed, movement in the y direction of the window will zoomin/out of the screen. 
        Dragging towards the top of the window zooms 'in' , while dragging towards the bottom zooms 'out'. There is no constraint to the FOV.
        
    - CAMERA CONTROL
        While holding the LEFT CONTROL button, pressing the following keys will:
        . 'A' -> Move the camera along the negative x axis.
        . 'D' -> Move the camera along the positive x axis.
        . 'W' -> Move the camera forward.
        . 'S' -> Move the camera backward.
        . 'Q' -> Move the camera upward.
        . 'Z' -> Move the camera downward.	
        
8- NOTES
        i- Beginning camera position and orientation
            The camera begins at the position (0,30,90), with its viewing angle tilted down by -tan(30/90) degrees. 
            This means that the original point of focus is exactly the origin (0,0,0) as per the assignment instructions.
        
        ii- Grid orientation
            The grid is composed of 100 x 100 squares in the XZ-plane. It is centered at x=0, z=0, such that its center is at the origin in the XZ plane. 
            It is translated 0.001 units in the negative y direction to make the coordinate axes more visible.
        
        iii- Coordinate axes
             The coordinate axes are always drawn as GL_LINES to make them more visible, and are all 5 grid units in length. Their starting points are all at the origin (0,0,0).
             
        iV- World rotation normalization
            We have produced a function that gives a fixed speed movement, which depends on the FPS of each unique machine that will be running this project, so that the movement speed of the models will be the same regardless of the machine's speed.

9- REFERENCES (SOURCES)
	- https://learnopengl.com/
	- Song Ho Ahn's web page for our icosphere/sphere mesh : http://www.songho.ca/opengl/gl_sphere.html
	- Dear ImGui library for our lighting control GUI (intended for a cool extra feature and our own debugging purposes) from github: https://github.com/ocornut/imgui
	- STB_image.h header from the stb library github: https://github.com/nothings/stb
	- Our materials properties from Frédéric Devernay's website: http://devernay.free.fr/cours/opengl/materials.html
	- A few of our noise functions in the shaders/code from: https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
	
10- PROJECT TEAM 
    Our project team is composed of the following members:
    - Nicholas Joanette
    - Sami Merhi
    - Mohamad Ghanmeh
    - Adrien Kamran
    - Ale Niang