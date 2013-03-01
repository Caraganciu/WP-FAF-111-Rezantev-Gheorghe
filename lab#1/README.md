# Windows Programming
###### student gr. FAF-111: Reazantev Gheorghe

# Topic: WIN32 API programming basics
## Intro
### Purpose
* Event-driven programming (message-oriented)
* API
* Windows applications development

### Additional Purposes
* Version Control Systems (GIT)
* Programming Style Guidelines
* Debugging
* Testing

## Completed Mandatory Objectives
* Create a Windows application
* Choose Programming Style Guidelines that you'll follow
* Add 2 buttons to window: one with default styles, one with custom styles (size, background, text color, font family, font size)
* Add 2 text inputs to window: one with default styles, one with custom styles (size, background, text color, font family, font size)
* Add 2 text elements to window: one with default styles, one with custom styles (size, background, text color, font family, font size)

## Completed Objectives With Points
* Make elements to fit window on resize **(1 pt)**
* Make elements to interact or change other elements (1 pt for 2 different interactions) **(0-2 pt)**
* Change behavior of different window actions (at least 3) **(1pt)**

## The application creation steps

In order to accomplish this task I read the chapters required from the referenced book 
and also used a lot of help from online sites like msdn, StackOverflow and a lot of other google related searches . 
    
As my working enviroment I used CodeBlock 12.11 with the mingw packages and started my work as a win32 GUI frame project.
    
As a main idea around which my project evolved was a "text editor " and based on that i added the following elements
and functionality to the app.
    

1. 4 Buttons(font change button , color change button , input the text button and clear the text button )
2. 2 Text windows(A text window for input and one for read only output)
3. Made elements fit on window resize and set the minimal window dimensions .
4. Changed the default window buttons functionality(close,minimize,maximize) for trolling purposes .
The PSG I used was the USC Aerial Robotics Programming Style Guide (C++) *(I still need a lot of getting used to in order to write everything accoring to the PSG)

![Result](https://raw.github.com/TUM-FAF/WP-FAF-111-Reazantev-Gheorghe/master/Lab%231/screenshot.png)

## Outro
Doing the laboratory work was certainly interesting ,but also very frustrating getting to understand how the messages fly around from procedure to window and
so on so forth . There were complications the most memorable of which was the inability to change the button color in a more "user friendly"
manner . The work with different controls made me realize more indepth at how windows programs actually work and it is a thing I very much enjoyed .

All in all it was a good experience.
