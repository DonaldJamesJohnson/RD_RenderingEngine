# This is a test/example rd file

Display "My_Scene"  "Screen" "rgbdouble"
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

# Horizontal Lines
Color 0.1 0 0.2
Line 0 479 0 639 479 0
Color 0.11 0 0.2
Line 0 439 0 639 439 0 #-40
Color 0.12 0 0.25
Line 0 404 0 639 404 0 #-35
Color 0.13 0 0.3
Line 0 374 0 639 374 0 #-30
Color 0.14 0 0.35
Line 0 349 0 639 349 0 #-25
Color 0.15 0 0.4
Line 0 329 0 639 329 0 #-20
Color 0.16 0 0.43
Line 0 312 0 639 312 0 #-17
Color 0.17 0 0.46
Line 0 298 0 639 298 0 #-14
Color 0.18 0 0.49
Line 0 285 0 639 285 0 #-13
Color 0.19 0 0.5
Line 0 274 0 639 274 0 #-11
Color 0.20 0 0.51
Line 0 264 0 639 264 0 #-10
Color 0.21 0 0.52
Line 0 255 0 639 255 0 #-9
Color 0.22 0 0.53
Line 0 247 0 639 247 0 #-8
Color 0.23 0 0.54
Line 0 240 0 639 240 0 #-7
Color 0.24 0 0.55
Line 0 234 0 639 234 0 #-6
Color 0.25 0 0.56
Line 0 229 0 639 229 0 #-5
Color 0.26 0 0.57
Line 0 225 0 639 225 0 #-4
Color 0.27 0 0.58
Line 0 222 0 639 222 0 #-3
Color 0.28 0 0.59
Line 0 220 0 639 220 0 #-2

# Vertical Lines
Color 0 0 0.1
Line 0 1 0 0 479 0
Line 639 1 0 639 479 0

Color 0 0.01 0.15
Line 40 1 0 40 479 0
Line 599 1 0 599 479 0

Color 0 0.02 0.20
Line 75 1 0 75 479 0 
Line 564 1 0 564 479 0

Color 0 0.03 0.25
Line 105 1 0 105 439 0
Line 534 1 0 534 439 0

Color 0 0.04 0.30
Line 130 1 0 130 404 0
Line 509 1 0 509 404 0

Color 0 0.05 0.33
Line 150 1 0 150 374 0
Line 489 1 0 489 374 0

Color 0 0.06 0.36
Line 165 1 0 165 349 0 #15
Line 474 1 0 474 349 0

Color 0 0.07 0.39
Line 179 1 0 179 329 0 #14
Line 460 1 0 460 329 0

Color 0 0.08 0.41
Line 192 1 0 192 312 0 #13
Line 447 1 0 447 312 0

Color 0 0.09 0.43
Line 204 1 0 204 298 0 #12
Line 435 1 0 435 298 0 

Color 0 0.10 0.45
Line 215 1 0 215 285 0 #11
Line 424 1 0 424 285 0

Color 0 0.11 0.47
Line 226 1 0 226 274 0 #10
Line 414 1 0 414 274 0

Color 0 0.12 0.49
Line 235 1 0 235 264 0 #9
Line 405 1 0 405 264 0

Color 0 0.13 0.51
Line 243 1 0 243 255 0 #8
Line 397 1 0 397 255 0

Color 0 0.14 0.52
Line 250 1 0 250 247 0 #7
Line 390 1 0 390 247 0

Color 0 0.15 0.53
Line 256 1 0 256 240 0 #6 
Line 384 1 0 384 240 0

Color 0 0.16 0.54
Line 261 1 0 261 234 0 #5
Line 379 1 0 379 234 0

Color 0 0.17 0.55
Line 265 1 0 265 229 0 #4
Line 375 1 0 375 229 0

Color 0 0.18 0.56
Line 268 1 0 268 225 0 #3
Line 372 1 0 372 225 0

Color 0 0.19 0.57
Line 270 1 0 270 222 0 #2
Line 370 1 0 370 222 0

Color 0 0.10 0.58
Line 271 1 0 271 220 0 #1
Line 369 1 0 369 220 0

# Circle
Color .01 0.01 0.01
Circle 320 100 0 100

# Circle Fill
Color 0.05 0 0 
Fill 320 440 0

Color 0.06 0 0
Fill 320 405 0

Color 0.07 0 0
Fill 320 375 0

Color 0.08 0 0
Fill 320 350 0

Color .09 0 0 
Fill 320 330 0 

Color .1 0 0
Fill 320 313 0
Fill 225 219 0
Fill 417 219 0

Color .12 0 0
Fill 320 299 0
Fill 234 219 0
Fill 406 219 0

Color .14 0 0
Fill 320 286 0
Fill 242 219 0
Fill 398 219 0

Color .16 0 0
Fill 320 275 0
Fill 249 219 0
Fill 391 219 0

Color .18 0 0
Fill 320 265 0
Fill 255 219 0
Fill 385 219 0

Color .2 0 0
Fill 320 256 0
Fill 260 219 0
Fill 380 219 0

Color .22 0 0
Fill 320 248 0
Fill 264 219 0
Fill 376 219 0

Color .24 0 0
Fill 320 241 0
Fill 267 219 0
Fill 373 219 0

Color .26 0 0
Fill 320 235 0
Fill 269 219 0
Fill 371 219 0

Color .27 0 0
Fill 320 230 0

Color .28 0 0
Fill 320 226 0

Color .29 0 0
Fill 320 223 0

Color .3 0 0
Fill 320 221 0

Color .3 0 0
#Fill 320 219 0

WorldEnd
FrameEnd
