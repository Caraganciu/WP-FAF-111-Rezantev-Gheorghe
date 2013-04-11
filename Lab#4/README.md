# Windows Programming Laboratory Work #4

## Title
Windows Timer. Animation.

## Contents
* Windows timer

## Completed Mandatory Objectives
* Create an animation based on Windows timer which involves at least 5 different drawn objects

## Completed Objectives With Points
* Increase and decrease animation speed using mouse wheel **(2 pt)**
* Solve flicking problem **(2 pt)**
* Add animated objects which interact with each other **(2-6 pt)**, ex.:
   Few balls which have different velocity . In order to get max points, add balls with mouse, make balls to change color on interaction.

## Application creation steps 
To accomplish this task i've used the reference to flickering solving and double buffering . 

Another thing I've taken into account is the info on the timer functionality in win32 .

The idea was to create a simple animation of some balls jumping around and bouncing off walls and other balls .
The result of that was mostly what i wished for with something interesting to add .
The main things :

1. Create balls on click that move under the same angle but have a different color and speed based on the position of creation 
2. Speed up or down the animation using the mouse wheel 
3. When the balls with different speeds interact they balance their speeds 
4. When the balls interact , the ball considered the first in the interaction turns dark green . Because of how the array is 
read , the ball that will keep its normal color until the end is the last one in the array . The balls that already turned 
won't turn other balls because of how the order the array is read in . 
5. The flickering was solved using double buffering and avoiding the call to erase background 

The PSG I used was the USC Aerial Robotics Programming Style Guide (C++)

![Result](https://raw.github.com/TUM-FAF/WP-FAF-111-Rezantev-Gheorghe/lab4/Lab%234/picture.png)

## Outro
This one was a very pleasant one because of the ability to solve flickering which was a big problem in previous app i tried doing.
The biggest problem in this one happens when more balls interact together forcing two of them to overlap and start spazzing out . On 
consecutive interaction with the problem pair can untie the pair . 

That's all i can say concerning this lab . 
