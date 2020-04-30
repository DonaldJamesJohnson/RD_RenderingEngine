# This is a test/example rd file

Display "PolySet"  "Screen" "rgbsingle"

Format 640 480
CameraFOV  45.0  # Degrees

FrameBegin 1

CameraEye   3   10  30
CameraAt    1.0   1.0  1.0
CameraUp    0.0   0.0  1.0

Clipping    1.0   1.0e+09


WorldBegin

Color 1 0 0

Cube

Color 0.7 0.3 0
Scale 1.5 1.5 1.5
Rotate "Z" 20

Cube

Color 0.5 0.5 0
Scale 1.5 1.5 1.5
Rotate "Z" 20

Cube 

Color 0.3 0.7 0
Scale 1.5 1.5 1.5
Rotate "Z" 20

Cube 
Color 0.2 0.8 0
Scale 1.5 1.5 1.5
Rotate "Z" 20

Cube 
Color 0.0 1 0
Scale 1.5 1.5 1.5
Rotate "Z" 20

Cube 
Color 0.0 0.7 0.3
Scale 1.5 1.5 1.5
Rotate "Z" 20

Cube 
WorldEnd
FrameEnd
