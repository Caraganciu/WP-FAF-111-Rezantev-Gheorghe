Windows Programming Laboratory Work #3
======================================

Title
-----
Basics of Working with Mouse. GDI Primitives. Bezier Curve.

##Contents
----------
- Mouse
- Device context
- GDI Primitives
   - Line
   - Curve
   - Plane
   - Bitmap image
- Bezier curve

##Mandatory Objectives (completed)
----------------------------------
- Draw few lines of different colors and weights.
- Draw a Bezier curve.
- Draw few plane objects of different colors, weights, filled and not.
- Draw 2 different objects using mouse.

##Objectives With Points (completed)
------------------------------------
- Draw a custom bitmap image. **(1 pt)**
- Add a switch that will change mouse ability to draw objects. **(2 pt)**
- Draw a Bezier curve using mouse. **(1 pt)**
- Fill an object with a gradient. **(1 pt)**
- Delete objects using mouse clicking. **(2 pt)**
- Zoom in and out application working area using keyboard . **(2 pt)**

##Application Creation Steps
----------------------------
  Nothing unusual here . I just started the work and step by step added needed features . The only remarcable thing in the 
creation was the use of a pencil and some paper in order to figure out how resizing should be done. 

The reference was as usual the book and some other lab works of my coleagues . Another useful reference was msdn which makes 
me realize more and more that in it's simplistic explanation form it offers all the knowledge I might need . 

 The resizing uses the keyboard up and down keys ,but make sure the focus is on the main window by clickind with the mouse anywhere in the
client area , except for the buttons .

 To delete objects use right click ( based on the undo principle ) 

 The PSG I used was the USC Aerial Robotics Programming Style Guide (C++) *(I still need a lot of getting used to in order to write everything accoring to the PSG)
This is what I got in the end :
![Result](https://raw.github.com/TUM-FAF/WP-FAF-111-Rezantev-Gheorghe/lab3/lab%233/picture.png)


## Outro
--------
  This one , was something different . Even though the basic parts like :buttons , static lines , static figures didn't prove
much of a problem , the resizing , figure tracing , flickering and similar stuff did . The resizing is present and it works for the most part ,
but it's not all rainbows and sunshine . After a certain number of resizes, because the points are integers and the precision is cut the figures might 
not resize to the perfet form . So far i haven't found a way around this . If the window is resized then depending on how fast you pull the value of the 
new window could be too small of a difference to make a change ( once again float -> integer ) . The figure tracing was a problem at first because I tried 
to avoid using the paint message for that functionality . After a good number of failures and my canvas going dark from overlapping lines I made it work with 
paint . And the flickering was mostly reduced by making only the canvas redraw itself instead of the whole client area . 
 All in all , it was another interesting lab . 


