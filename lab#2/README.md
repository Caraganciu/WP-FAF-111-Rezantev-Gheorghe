# Windows Programming
###### student gr. FAF-111: Reazantev Gheorghe

# Topic: Advanced Form Elements. Child Windowses. Basics of Working With Keyboard.
## Intro
### Purpose
* The Keyboard
* Child Window Controls
  * Scroll Bar
  * Listbox
* Menus and Other Resources
* Dialog Boxes

## Completed Mandatory Objectives
## Mandatory Objectives
* Display a dialog box on some event (ex. on clicking some button)
* Add a system menu to your application with at least 3 items (add actions to that items)
* Add a scroll bar that will change any visible parameter of any other element (color of a text)
* Hook keyboard input. Add 2 custom events for 2 different keyboard combinations (ex. change window background on ctrl+space) 

## Completed Objectives With Points
* Add a listbox and attach some events when any element is accessed (clicked) **(2 pt)**
* Add 2 scroll bars that will manage main window size or position **(1 pt)**
* Customize your application by adding an icon and using different cursor in application **(1 pt)**

## The application creation steps

For this application I documented myself with the win32 api even more using online sources and the given reference material . The idea behind
this project was very simple . I just added all the needed elements and made interatction between them . There was no grand design to begin with this time 
mostly because the time was limited and it was a rushed project . Even though it was rushed i still got satisfying results .

The interactions and things i added to my project can be summed up in the following :
    

1. A menu with 3 compartments (file->exit , listboxcolor->white or red, info -> about) 
The first 2 are self explanatory while the last one displays a dialog box with information about the app.
2. 2 Scroll bars that manage the position of the window on the desktop for a small interval . For this I had to remove thumbtracking because 
it would be impossible to work with a moving thumb and window at the same time (incredible flickering )
3. Added some text the main purpose of which is to change color when the user interacts with another scroll bar (blue->red)
4. Added a list box that has 5 names in it . When double-clicking on a name you can get a message box with details about the person ( just the age) 
5. Added 3 keyboard inputs . The shift-l that sets focus on the list box , ctrl-s that sets focus one the color scroll bar and 
gave the space button the same actions in a listbox as double clicking a name .
6. Added custom cursor and icon 
The PSG I used was the USC Aerial Robotics Programming Style Guide (C++) *(I still need a lot of getting used to in order to write everything accoring to the PSG)

![Result](https://raw.github.com/TUM-FAF/WP-FAF-111-Rezantev-Gheorghe/lab2/lab%232/picture.png)

## Outro
  This one was good , even if a little rushed from my part . None the less it was interesting and the idea of subclassing is becoming more and more 
familiar to me which is a good thing . From the things i did on this lab the most interesting i think was using resources . It was unusual at first but 
when i got the hang of it the plusses of using resources were obvious . I enjoyed this one maybe a bit more than the last because
it taught me how to use subclassing and keyboard controls , not to mention the ability to change the icon and cursor which makes for a good 
level of customization in an application .
