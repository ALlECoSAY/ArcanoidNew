# ArcanoidNew

[Game zip GoogleDrive](https://drive.google.com/file/d/1fWzluTVWSZh634rYb1p-8l0f7dNGVnMX/view?usp=sharing)

## launch prerequisites:
win64
cmd\Dir>ArcanoidNew.exe $width $height
or just launch .exe and type width and height thereafter

## Controls
(as described in the task) 

## Additional tasks:
### 1) The game window is divided into a few areas. The ball moves faster or slower by 50% in some areas.
![task1](https://user-images.githubusercontent.com/41470575/188628640-2fdf1b27-b3a3-450c-888d-b06feb8fc3e3.png)

+ \+ : velocity increased

+ \- : velocity decreased

+ 0 : velocity not changed

### 2) The first ability (positive) gives a “saving wall” and the second one (negative) damages the player. Saving wall appears on the bottom edge and deflects a ball if a player misses it. Wall can be used one time (it can save from only one missed ball).
### 3) The first ability (positive) increases the size of the platform and the second one (negative) decreases it by 40%. Ability stays active for 20 seconds. Abilities are “stackable”, it means that it is possible to have multiple active instances of the same ability. Minimum size is 50%, maximum is 200% of a default size.
### 4) Abilities cost 20 points. The player can buy an ability on the right mouse button. Points can be obtained by destroying blocks. A random ability is chosen and has an immediate impact.

Abilities percentage chance per buy action:

+ 40% - increase platform width  
+ 30% - decrease platform width
+ 20% - saving wall
+ 10% - damage player (implies the presence of *health*, which is set to 3 for the game)
Can't be more than 2 active saving walls at one moment.
