# This is a test/example rd file

Display "Fill_Test_1"  "Screen" "rgbsingle"
Format 640 480

# Testing line drawing

# The next three lines will be ignored by your renderer
CameraEye 320 240 -240
CameraAt  320 240 0
CameraUp  0.0 -1.0 0.0

FrameBegin 1
WorldBegin

# This line will also be ignored by your renderer
#Translate 0.5 0.5 0.0

Line 320 240 0  440 240 0
Line 440 360 0  200 360 0
Line 320 240 0  320 360 0
Line 200 360 0  200 120 0
Line 320 240 0  200 240 0
Line 200 120 0  440 120 0
Line 320 240 0  320 120 0
Line 440 120 0  440 360 0

Color 1.0 1.0 0.0
Point 320 240 0

Color 1 .4 .4
Fill 380 300 0

Color .4 1 .4
Fill 260 300 0

Color .4 .4 1
Fill 380 180 0

Color 1 .7 .1
Fill 260 180 0


WorldEnd
FrameEnd
