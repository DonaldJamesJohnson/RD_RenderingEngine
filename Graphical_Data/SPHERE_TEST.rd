Display "SPHERE_TEST" "Screen" "rgbdouble"

Background 0.5 0.5 0.5

FrameBegin 1

CameraEye  0 0 10
CameraAt   0 0 -1
CameraUp   0 1 0 

CameraFOV 30.0

WorldBegin

Translate 0 0 0

Color 0.0 0.0 0.0

Sphere 3 0 5 360 

WorldEnd
FrameEnd
