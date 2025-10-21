// CS 3503 W04- Comp Org & Arch
// Created by Paola on 10/15/2025.
// Assignment: BitBoard Checkers Game Engine Project

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL3/SDL.h>
#include "BitBoardGE.h"

//========== Graphics ==========
// Window constants
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640
#define SQUARE_SIZE 80
#define BOARD_OFFSET_X 0
#define BOARD_OFFSET_Y 0

// Colors
#define COLOR_LIGHT_SQUARE 0xFFE4C4
#define COLOR_DARK_SQUARE 0x8B4513
#define COLOR_SELECTED 0xFFFF00
#define COLOR_RED_PIECE 0xFF0000
#define COLOR_BLACK_PIECE 0x000000
#define COLOR_KING_CROWN 0xFFD700

// Draw a filled circle (for pieces)
void DrawCircle(SDL_Renderer* renderer, int cx, int cy, int radius) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x*x + y*y <= radius*radius) {
                SDL_RenderPoint(renderer, cx + x, cy + y);
            }
        }
    }
}

// Draw the checkers board
void DrawBoard(SDL_Renderer* renderer, GameState* game, int selected_pos) {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
    SDL_RenderClear(renderer);

    // Draw board squares
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            int x = BOARD_OFFSET_X + col * SQUARE_SIZE;
            int y = BOARD_OFFSET_Y + row * SQUARE_SIZE;

            SDL_FRect rect = {x, y, SQUARE_SIZE, SQUARE_SIZE};

            int position = RowColToPosition(7 - row, col);

            // Color the square
            if (position == selected_pos) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);  // Yellow for selected
            } else if ((row + col) % 2 == 1) {
                SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);  // Dark brown
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 228, 196, 255);  // Light beige
            }

            SDL_RenderFillRect(renderer, &rect);

            // Draw border
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderRect(renderer, &rect);

            // Draw pieces
            unsigned long long mask = 1ULL << position;
            int cx = x + SQUARE_SIZE / 2;
            int cy = y + SQUARE_SIZE / 2;
            int radius = SQUARE_SIZE / 3;

            if (game->player1_pieces & mask) {
                // Red piece
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                DrawCircle(renderer, cx, cy, radius);
            } else if (game->player1_kings & mask) {
                // Red king
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                DrawCircle(renderer, cx, cy, radius);
                SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
                DrawCircle(renderer, cx, cy, radius / 3);
            } else if (game->player2_pieces & mask) {
                // Black piece
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                DrawCircle(renderer, cx, cy, radius);
            } else if (game->player2_kings & mask) {
                // Black king
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                DrawCircle(renderer, cx, cy, radius);
                SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
                DrawCircle(renderer, cx, cy, radius / 3);
            }
        }
    }
    SDL_RenderPresent(renderer);
}

// Convert mouse click to board position
int GetPositionFromClick(int mouse_x, int mouse_y) {
    if (mouse_y < BOARD_OFFSET_Y) return -1;

    int col = (mouse_x - BOARD_OFFSET_X) / SQUARE_SIZE;
    int row = (mouse_y - BOARD_OFFSET_Y) / SQUARE_SIZE;

    if (row < 0 || row >= 8 || col < 0 || col >= 8) return -1;

    return RowColToPosition(7 - row, col);
}

// Main game loop with SDL
void PlayGraphicalGame() {
    // Initialize SDL3
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return;
    }
 SDL_Window* window = SDL_CreateWindow(
        "Checkers - Player vs AI",
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );

    if (!window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

    if (!renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    // Initialize game
    GameState game;
    InitGame(&game);
    srand(time(NULL));

    int selected_pos =-1;
    int running =1;
    int winner =0;

    printf("=== Checkers: Player (Red) vs AI (Black) ===\n");
    printf("Click a piece to select it, then click where to move.\n\n");

    int multi_jump_piece = -1;  // Track piece that must continue multi-jump

    while (running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running =0;
            }
            else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && game.current_player == 1) {
                int clicked_pos = GetPositionFromClick(event.button.x, event.button.y);

                if (clicked_pos == -1) continue;

                unsigned long long mask = 1ULL << clicked_pos;
                unsigned long long p1_pieces = game.player1_pieces | game.player1_kings;

                if (selected_pos ==-1) {
                    // Select a piece
                    // If in multi-jump mode, can only select the piece that's jumping
                    if (multi_jump_piece != -1) {
                        if (clicked_pos == multi_jump_piece) {
                            selected_pos = clicked_pos;
                            printf("Continue multi-jump from position %d\n", selected_pos);
                        } else {
                            printf("Must continue multi-jump with piece at position %d\n", multi_jump_piece);
                        }
                    } else if (p1_pieces & mask) {
                        selected_pos =clicked_pos;
                        printf("Selected position %d\n", selected_pos);
                    }
                } else {
                    // Try to move
                    int move_result = MakeMove(&game, selected_pos, clicked_pos);
                    if (move_result >0) {
                        printf("Moved from %d to %d\n", selected_pos, clicked_pos);

                        if (move_result == 2) {
                            // Multi-jump available - must continue
                            multi_jump_piece = clicked_pos;
                            selected_pos = clicked_pos;  // Keep piece selected
                            printf("You must continue capturing!\n");
                        } else {
                            // Normal move completed
                            multi_jump_piece = -1;
                            selected_pos = -1;

                            winner = CheckWinner(&game);
                            if (winner != 0) {
                                printf("\n PLAYER %d WINS! \n", winner);
                            }
                        }
                    } else {
                        // Reselect if clicking own piece
                        if (multi_jump_piece == -1 && (p1_pieces & mask)) {
                            selected_pos = clicked_pos;
                        } else {
                            selected_pos = -1;
                        }
                    }
                }
            }
        }

        // AI's turn
        if (game.current_player == 2 && winner == 0 && multi_jump_piece == -1) {
            SDL_Delay(500);  // Short delay so you can see AI thinking

            // AI handles multi-jumps automatically
            int ai_position = -1;
            int continue_jumping = 1;

            while (continue_jumping && game.current_player == 2) {
                Move moves[100];
                int count = GetAllValidMoves(&game, 2, moves, 100);

                if (count == 0) break;

                // If in multi-jump, only consider moves from the jumping piece
                if (ai_position != -1) {
                    Move filtered_moves[100];
                    int filtered_count = 0;
                    for (int i = 0; i < count; i++) {
                        if (moves[i].from == ai_position) {
                            filtered_moves[filtered_count++] = moves[i];
                        }
                    }
                    if (filtered_count == 0) break;

                    // Copy filtered moves back
                    for (int i = 0; i < filtered_count; i++) {
                        moves[i] = filtered_moves[i];
                    }
                    count = filtered_count;
                }

                // Pick best move (prefer captures)
                int best = 0;
                for (int i = 1; i < count; i++) {
                    if (moves[i].score > moves[best].score) best = i;
                }

                if (moves[best].is_capture) {
                    printf("AI captures at position %d!\n", moves[best].captured_pos);
                }

                int move_result = MakeMove(&game, moves[best].from, moves[best].to);

                if (move_result == 2) {
                    // Multi-jump - continue with same piece
                    ai_position = moves[best].to;
                    printf("AI continues multi-jump from position %d\n", ai_position);
                    SDL_Delay(500);  // Delay between jumps
                    DrawBoard(renderer, &game, -1);
                } else if (move_result == 1) {
                    // Normal move completed
                    continue_jumping = 0;
                    printf("AI completed move!\n");
                } else {
                    // Move failed
                    break;
                }
            }

            winner = CheckWinner(&game);
            if (winner != 0) {
                printf("\n PLAYER %d WINS! \n", winner);
            }
        }

        DrawBoard(renderer, &game, selected_pos);
        SDL_Delay(16);  // ~60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    PlayGraphicalGame();
    return 0;
}



//=============== TESTING =============
void TestPhase1() {
    unsigned int test = 0;

    printf("=== Phase 1: Bit Manipulation Tests ===\n\n");

    // Test SetBit
    printf("Test SetBit:\n");
    test = SetBit(test, 3);
    printf("After setting bit 3:\n");
    PrintBinary(test);
    PrintHex(test);
    printf("Expected: 0000 0000 0000 0000 0000 0000 0000 1000\n\n");

    // Test CountBits
    printf("Test CountBits:\n");
    printf("Number of 1s: %d\n", CountBits(test));
    printf("Expected: 1\n\n");

    // Test SetBit multiple times
    printf("Test multiple SetBit calls:\n");
    test= SetBit(test, 0);
    test= SetBit(test, 7);
    test= SetBit(test, 15);
    PrintBinary(test);
    printf("Number of 1s: %d\n", CountBits(test));
    printf("Expected: 4\n\n");

    // Test ClearBit
    printf("Test ClearBit:\n");
    test = ClearBit(test, 3);
    PrintBinary(test);
    printf("Number of 1s: %d\n", CountBits(test));
    printf("Expected: 3\n\n");

    // Test ToggleBit
    printf("Test ToggleBit:\n");
    test = ToggleBit(test, 0);  // Toggle off
    test = ToggleBit(test, 3);  // Toggle on
    PrintBinary(test);
    printf("Number of 1s: %d\n", CountBits(test));
    printf("Expected: 3\n\n");

    // Test GetBit
    printf("Test GetBit:\n");
    printf("Bit 3: %d (Expected: 1)\n", GetBit(test, 3));
    printf("Bit 5: %d (Expected: 0)\n", GetBit(test, 5));
    printf("Bit 7: %d (Expected: 1)\n\n", GetBit(test, 7));

    // Test ShiftLeft
    printf("Test ShiftLeft:\n");
    unsigned int shifted= ShiftLeft(test, 2);
    PrintBinary(shifted);
    printf("Original for comparison:\n");
    PrintBinary(test);
    printf("\n");

    // Test ShiftRight
    printf("Test ShiftRight:\n");
    shifted = ShiftRight(test, 2);
    PrintBinary(shifted);
    printf("\n");
}

// Phase 2: Checkers Game
void PlayGame() {
    GameState game;
    InitGame(&game);

    printf("\n=== Phase 2: Checkers Game ===\n");
    printf("Legend: r=Red piece, R=Red King, b=Black piece, B=Black King\n");
    printf("Dark squares (□) are playable\n\n");

    int winner= 0;
    int move_count= 0;

    while (winner ==0 && move_count <100) {  // Max 100 moves for testing
        PrintBoard(&game);

        int from, to;
        printf("\nEnter move (from to), e.g., '9 14' or '-1 -1' to quit: ");
        scanf("%d %d", &from, &to);

        if (from == -1) {
            printf("Game ended by user.\n");
            break;
        }

        if (MakeMove(&game, from, to)) {
            move_count++;
            winner = CheckWinner(&game);

            if (winner != 0) {
                PrintBoard(&game);
                printf("\n PLAYER %d WINS! \n", winner);
            }
        } else {
            printf("Invalid move! Try again.\n");
        }
    }
}


