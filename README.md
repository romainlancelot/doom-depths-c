<h1>Doomsdepths</h1>

Project made for the ESGI C pool. This is the remake of the game Doomdepths in C.

<img src="https://media.tenor.com/BIdLVTMDhbcAAAAC/nelliel-bleach.gif" width="100%">

<h1>Table of contents</h1>

- [Installation](#installation)
- [How to play](#how-to-play)

# Installation

The game runs in the CLI, so you need to have a terminal to run it.
If you are on Windows and / or don't have a compiler, don't worry i got you covered 😎.
Simply run the docker image with the following command:

```bash
you@your_machine:~$ docker compose up -d --build
you@your_machine:~$ docker exec -it doomdepths bash -c "make && ./prog"
```

Otherwise, if you're truly a king and have a UNIX system 🤩, you can simply run the following command:

```bash
you@your_machine:~$ make install # This will install the required dependencies
you@your_machine:~$ make && ./prog
```

# How to play

The game is pretty simple, you have to kill the enemies and survive as long as you can.

<h1>Thanks for reading and playing our game</h1>

<img src="https://media.tenor.com/xVvOi8dFvh0AAAAC/goodbye-my-friends.gif" width="100%">
