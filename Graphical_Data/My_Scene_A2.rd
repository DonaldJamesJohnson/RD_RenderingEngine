# This is a test/example rd file

Display "PolySet"  "Screen" "rgbsingle"

Format 640 480
CameraFOV  45.0  # Degrees

FrameBegin 1

CameraEye   0     0    15.8
CameraAt    1.0   1.0  1.0
CameraUp    0.0   0.0  1.0

Clipping    1.0   1.0e+09


WorldBegin

Color 1 0 0 
Translate 5 0 0
Rotate "Y" 60

Cone 5 1 300

Color 0.8 0.2 0
Translate -1.5 0 0
Cone 4.5 1 300

Color 0.6 0.4 0
Translate -1.4 0 0 
Cone 4 1 300

Color 0.4 0.6 0
Translate -1.3 0 0 
Cone 3.5 1 300

Color 0.2 0.8 0
Translate -1.2 0 0 
Cone 3 1 300

Color 0 1 0
Translate -1.1 0 0 
Cone 2.5 1 300

Color 0 0.8 0.2
Translate -1.0 0 0 
Cone 2 1 300

Color 0 0.6 0.4
Translate -0.9 0 0 
Cone 1.5 1 300

Color 0 0.4 0.6
Translate -0.8 0 0 
Cone 1 1 300

Color 0 0.2 0.8
Translate -0.7 0 0 
Cone .5 1 300

Color 0 0.0 1
Translate -0.6 0 0 
Cone .5 1 300

Color 0.2 0.0 .8
Translate -0.5 0 0 
Cone .4 1 300

Color 0.4 0.0 0.6
Translate -0.4 0 0 
Cone .3 1 300

Color 0.4 0.0 0.4
Translate -0.3 0 0 
Cone .2 1 300

Color 0.2 0.0 0.2
Translate -0.3 0 0 
Cone .1 1 300

WorldEnd
FrameEnd


