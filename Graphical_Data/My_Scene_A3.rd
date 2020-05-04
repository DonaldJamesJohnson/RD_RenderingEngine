# This is a test/example rd file

Display "My_Scene_A3"  "Screen" "rgbsingle"

Format 640 480
CameraFOV  45.0  # Degrees

FrameBegin 1

CameraEye   0     0   95
CameraAt    1.0   1.0  1.0
CameraUp    0.0   0.0  1.0

Clipping    1.0   1.0e+09

WorldBegin
Rotate "X" 90

Color 0.05 0 0.05
Sphere 1.0 -1.0 1.0 360.0

Color 0.1 0 0.1
Sphere 3.0 -3.0 3.0 360.0

Color 0.2 0 0.2
Sphere 5.0 -5.0 5.0 360.0

Color 0.4 0 0.4
Sphere 7.0 -7.0 7.0 360.0

Color 0.4 0 0.6
Sphere 9.0 -9.0 9.0 360.0

Color 0.2 0 0.8
Sphere 11.0 -11.0 11.0 360.0

Color 0 0 1
Sphere 13.0 -13.0 13.0 360.0

Color 0 0.2 0.8
Sphere 15.0 -15.0 15.0 360.0

Color 0 0.4 0.6
Sphere 17.0 -17.0 17.0 360.0

Color 0 0.6 0.4
Sphere 19.0 -19.0 19.0 360.0

Color 0 0.8 0.2
Sphere 21.0 -21.0 21.0 360.0

Color 0 1 0
Sphere 23.0 -23.0 23.0 360.0

Color 0.2 0.8 0
Sphere 25.0 -25.0 25.0 360.0

Color 0.4 0.6 0
Sphere 27.0 -27.0 27.0 360.0

Color 0.4 0.6 0
Sphere 29.0 -29.0 29.0 360.0

Color 0.6 0.4 0
Sphere 31.0 -31.0 31.0 360.0

Color 0.8 0.2 0
Sphere 33.0 -33.0 33.0 360.0

Color 1.0 0 0 
Sphere 35.0 -35.0 35.0 360.0

WorldEnd
FrameEnd