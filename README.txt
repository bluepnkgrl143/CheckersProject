CS 3503 Project- BitBoard Checkers Game

Author: Paola Di Cesare

Description: Creating a checkers game using bitboards.

Building Instructions and Progress comments:
    Starting with Phase 1:
        First, started working on building the header files for defining the bit operations.
        Then moved to the BitBoard.c file to create the functions that will later be
        implemented in the main.c file.
        Then coded the main.c file to test the functions.
        Debugging throughout the files, while running tests to achieve the desired outcomes.

    Once phase 1 was completed, moved to phase 2:
        Defining new game functions in the header files.
        Implementing the new functions in the BitBoard.c file.
        Implementing the new functions in the main.c file.
        Debugging throughout the files, while running tests to achieve the desired outcomes.

    Phase 2 Continue: Improving graphics, user interface, and implementing AI as opponent.
        I want a graphic interface that will allow user to play- click the game, and setup AI as opponent.
        Researched how to implement AI into code and enable graphic display.
        - worked on graphics first (simple shapes and simple colors)
        - worked on clicking input functions
        - worked on AI functions
            - had some issues downloading correct SDL3 file and implementing it correctly, but i resolved it.
            - Updated header file to include AI functions
            - Updated BitBoard.c file to include AI functions (including updating library,
                reading board, and randomizing next move) while simplifying code a bit.
        - debugged game and AI functions and enhanced rules(for example it has to eat opponenet if able, and)
            make best possible move for challenge)
        - implemented and debugged multi-jump, which enforces the rule of same turn capture if able.


Technologies Used: Used CLion for programming in C.

Challenges: Debugging certain features of the game, such as the implementation of AI as opponent
        was challenging and time consuming, needed help to debug and implement. Continuously had
        issues with King not being able to move after being crowned, but I found the issue having to do
        with the way I was reading the board and it's value at the time of the move. (I guess AI opponent
       is a stretch since it's not a very smart AI as far as playing strategy goes,
       it's more of a computer opponent).