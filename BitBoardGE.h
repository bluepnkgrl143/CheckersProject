// CS 3503 W04- Comp Org & Arch
// Created by Paola on 10/15/2025.
// Assignment: BitBoard Checkers Game Engine Project

#ifndef CHECKERSPROJECT_BITBOARDGE_H
#define CHECKERSPROJECT_BITBOARDGE_H

// ========== PHASE 1: Basic bit operations ==========

// Basic bit operations- Set bit to 1
unsigned int SetBit(unsigned int value, int position);

// Set bit to 0
unsigned int ClearBit(unsigned int value, int position);

// Flip bit
unsigned int ToggleBit(unsigned int value, int position);

// Get bit value (0 or 1)
int GetBit(unsigned int value, int position);

// Counting and finding
int CountBits(unsigned int value);

// Shift operations
unsigned int ShiftLeft(unsigned int value, int positions);
unsigned int ShiftRight(unsigned int value, int positions);

// Display functions
void PrintBinary(unsigned int value);
void PrintHex(unsigned int value);

// ========== PHASE 2: Game Structure and Functions ==========
// Game state structure
typedef struct {
    unsigned long long player1_pieces;  // Red pieces (bottom)
    unsigned long long player1_kings;   // Red kings
    unsigned long long player2_pieces;  // Black pieces (top)
    unsigned long long player2_kings;   // Black kings
    int current_player;                 // 1 or 2
} GameState;

// Game initialization
void InitGame(GameState* game);

// Display functions
void PrintBoard(GameState* game);

// Move validation and execution
int IsValidMove(GameState* game, int from, int to, int* captured_pos);
int MakeMove(GameState* game, int from, int to);

// Game logic
void PromoteToKing(GameState* game, int position);
int CheckWinner(GameState* game);

// Helper functions
int IsOccupied(GameState* game, int position);
int PositionToRow(int position);
int PositionToCol(int position);
int RowColToPosition(int row, int col);
void GetPieces(GameState* game, int player, unsigned long long* my_pieces, unsigned long long* opp_pieces);

// ========== AI Functions ==========
typedef struct {
    int from;
    int to;
    int score;
    int is_capture;  // 1 if this move captures a piece
    int captured_pos; // Position of captured piece
} Move;

// Get all valid moves for a player
int GetAllValidMoves(GameState* game, int player, Move* moves, int max_moves);

// Get all capture moves (jumps)
int GetAllCaptureMoves(GameState* game, int player, Move* moves, int max_moves);

// Check if player has any capture moves available
int HasCaptureMoves(GameState* game, int player);

// Check if a specific piece at a position has capture moves available
int HasCaptureMovesFromPosition(GameState* game, int player, int position);

// AI makes a move
int AIMove(GameState* game);

#endif // BITBOARD_H
