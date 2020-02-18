# This is a test/example rd file

Display "Circle_Test"  "Screen" "rgbsingle"
Format 500 300 

# Testing circles

# The next three lines will be ignored by your renderer
CameraEye 250 150 -150
CameraAt  250 150 0
CameraUp  0.0 -1.0 0.0

FrameBegin 1

Background 0.3 0.3 0.3

WorldBegin

# x y z of center, then radius

Circle 125 150 0 1

Circle 125 150 0 3

Circle 125 150 0 5

Circle 125 150 0 7

Circle 125 150 0 9

Circle 125 150 0 11

Circle 375 150 0 2

Circle 375 150 0 4

Circle 375 150 0 6

Circle 375 150 0 8

Circle 375 150 0 10

Circle 375 150 0 12

WorldEnd
FrameEnd
