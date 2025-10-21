CS 3503 Project- BitBoard Checkers Game

Author: Paola Di Cesare

Description: Creating a checkers game using bitboards.

Building Instructions: 
    Download files from the repository and run on CLion.
    Also, download the SDL3 developer (whichever matches your operating system) and update the corresponding CMakiLists.txt file with the SDL3 local path.
    1) Clone repository
    2) Download SDL3:**
       - Go to [SDL3 Releases](https://github.com/libsdl-org/SDL/releases)
       - Download `SDL3-3.2.24-win32-x64.zip` (or latest version)
    3) Extract to "C:\SDL3\" (or update CMakeLists.txt)
    4) Open in CLion** and build
    5) Run the game

Progress documentation and comments:
    Starting with Phase 1:
        First, started working on building the header files for defining the bit operations.
        Then, I moved to the BitBoard.c file to create the functions that will later be
        implemented in the main.c file.
        Then coded the main.c file to test the functions.
        Debugging throughout the files while running tests to achieve the desired outcomes.

    Once phase 1 was completed, I moved to phase 2:
        Defining new game functions in the header files.
        Implementing the new functions in the BitBoard.c file.
        Implementing the new functions in the main.c file.
        Debugging throughout the files while running tests to achieve the desired outcomes.

    Phase 2 Continue: Improving graphics, user interface, and implement AI as an opponent.
        I wanted a graphic interface that would allow the user to play- click the game, and set up AI as an opponent, I have previously worked with simple WindowGraphics, so I wanted to try out implementing higher graphics (hence the need for SDL3).
        Researched how to implement AI into code and enable graphic display.
        - worked on graphics first (simple shapes and simple colors)
        - worked on clicking input functions
        - worked on AI functions
            - had some issues downloading the correct SDL3 file and implementing it correctly, but I resolved it.
            - Updated header file to include AI functions
            - Updated BitBoard.c file to include AI functions (including updating library, reading board, 
                and randomizing next move) while simplifying code a bit.
        - debugged game and AI functions and enhanced rules(for example, it has to eat                
            opponenet if able, and) make the best possible move for challenge)
        - Implemented and debugged multi-jump, which enforces the rule of the same turn capture if able.


Technologies Used: Used CLion for programming in C.

Challenges: Debugging certain features of the game, such as the implementation of AI as an opponent, was challenging
    and time-consuming; I needed help to debug and implement. Continuously had issues with King not being able to 
    move after being crowned, but I found the issue had to do with the way I was reading the board and its value at
    the time of the move. (I guess AI opponent is a stretch since it's not a very smart AI as far as playing strategy
    goes, it's more of a computer opponent.)
