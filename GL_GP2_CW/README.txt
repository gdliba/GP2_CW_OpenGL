All movement is performed relative to the camera’s local space.
This provides more intuitive navigation within the scene.

The scene starts using Camera 1.
Camera 2 is mounted with a white light (Light 1)
Camera 3 is mounted with a blue light (Light 2) with lower intensity
Cameras 2 and 3 are represented as emissive cubes, making them visible in the scene
Camera 1 is not visually represented

=== CONTROLS ===

GENERAL 
C		Cycle through available cameras
R		Completely reset the scene to starting position (lights, cameras and models)
ESC 		Close the program

LIGHTING 
1 		Toggle Light 1 ON/OFF
2		Toggle Light 2 ON/OFF

MOVEMENT (ACTIVE CAMERA)
W/A/S/D		Move forward/left/backward/right
SPACE		Move up
LCTRL		Move down
Hold LSHIFT	Increase camera movement speed

CAMERA VIEW
ARROW KEYS	Adjust camera rotation
MOUSE		Look around (yaw & pitch)


Additional Features:

• Multiple textured 3D models rendered in a single scene
• Implementation of ADS (Ambient, Diffuse, Specular) lighting with specular highlights
• Real-time dynamic lighting (light positions updated every frame, not precomputed)
• Camera-mounted light sources with independent toggle controls
• Lights encapsulated into a dedicated Light class for improved architecture
• Multiple switchable cameras with independent transforms
• Mouse-look (yaw/pitch) and keyboard-based camera movement
• Frame-independent animation using delta time
• Real-time synchronisation between camera movement and light position
• Scene reset functionality restoring initial state
• VSync enabled for smoother rendering
• Improved depth buffering (32-bit) to reduce visual artifacts
• Emissive cube markers used to visually represent light sources