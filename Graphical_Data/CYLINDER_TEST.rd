Display "CYLINDER_TEST" "Screen" "rgbdouble"

Background 0.5 0.5 0.5

FrameBegin 1

CameraEye  0 0 3
CameraAt   0 0 -1
CameraUp   0 1 0 

WorldBegin

Translate 0 0 0

Rotate "Y" 90

Rotate "X" 90

Color 0.0 0.0 0.0

Cylinder 1 0 2 360 

WorldEnd
FrameEnd
