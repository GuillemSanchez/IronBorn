# Ironborn a plataformer game

## Links

## Introduction:

Hi everyone im Guillem Sanchez Herranz a college student from CITM/UPC. 
This is ironborn, a game where you will play as a hero that will have to sort some obstacles to get to the enemie castle.
This game was developed for a college Project (Game development) i hope you like it.

<video src="video/Dev_final.mp4.mp4" width="512" height="384" controls preload></video>


## Team:

Guillem Sanchez

<img src="https://github.com/GuillemSanchez/IronBorn/blob/master/docs/photo/Guillem.jpg?raw=true">




### Work Done:

I work alone so i did everthing, some importants parts that im proud are:

The player controls and physics.

The Ui scroll bar.

The Console.



## Core and Subsystems:

My game have for a main core a engine that works with a basic loop that contains 3 stages, Preupdate, update, and postupdate,
also we use the module system to control all the diference states and cores of the game. SDL is a good tool to make that happen so special thanks to them.

### Subsystem entities:

Also know as entity manager, is a subsystem that controls all the entities in the game (in my case, player and coins), this helps us
know what are they doing and how are they doing it, also helps us to create multiple exact entities.

### Subsystem GUI:

The same as the subsystem above but with the GUI, really usfull in this game to be able to create a UI.

### Console:

This is one of the things that i made at the end, and its really impressive how it works, bassicaly its a lot of ifs so we know exactly
what the dev/player wants to execute. Also i made some "INOVATION" making that the comands are stored in a var and then the dev/player
can run this var to get older comands.


## Instruccions to play:


### Controls:

- A · Go left
- D · Go right
- Space · Jump
- 2 · Roll


#### Debug Functions:

- F1 Start from the very first level
- F2 Sart from the second of the current level
- F3 start from the beginning of the current level
- F5 Save the current state
- F6 Load the previous state
- F8 view UI debug.
- F9 View Colliders
- F10 God Mode
- F11 Cap FPS to 30 or 60

#### More information:

Console:

- You can open/close the console with º
- Hitting enter you execute the comand
-"inovation" Arrows cycle through all the previously sent commants.



## License

MIT License

Copyright (c) 2019 GuillemSanchez

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

## Special thanks:

### Art:

### Envioroment:

https://szadiart.itch.io/pixel-dark-forest

### Character:

https://oco.itch.io/medieval-fantasy-character-pack

## Testers:

Diego Muñoz, Ricard Codina, Ricard Riven, Laura Aguilar.





