# This is a test/example rd file

Display "Fill_Test_2"  "Screen" "rgbdouble"
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

Color .8 0 0.8
Circle 320 240 0 200
Fill 320 420 0

Color 0 .4 .8
Circle 320 240 0 180
Fill 320 400 0

Color .5 0.8 0
Circle 320 240 0 160
Fill 320 380 0

Color .8 0.8 0
Circle 320 240 0 140
Fill 320 360 0

Color .8 0.4 0
Circle 320 240 0 120
Fill 320 340 0

Color .8 0.2 0.2
Circle 320 240 0 100
Fill 320 320 0

Color 1.0 0.6 0.6
Circle 320 240 0 80
Fill 320 300 0

Color .6 .6 .6
Circle 320 240 0 60
Fill 320 280 0

Color .3 .3 .3
Circle 320 240 0 40
Fill 320 260 0

Color .4 .3 .2
Circle 320 240 0 20
Fill 320 240 0

WorldEnd
FrameEnd
