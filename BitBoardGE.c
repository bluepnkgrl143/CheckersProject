// CS 3503 W04- Comp Org & Arch
// Created by Paola on 10/15/2025.
// Assignment: BitBoard Checkers Game Engine Project

#include "BitBoardGE.h"
#include <stdio.h>
#include <stdlib.h>

// ========== PHASE 1 FUNCTIONS ==========

// Set bit at position to 1
unsigned int SetBit(unsigned int value, int position) {
    if (position < 0 || position >= 32) return value;
    return value | (1<< position);
}

// Clear bit at position (set to 0)
unsigned int ClearBit(unsigned int value, int position) {
    if (position <0 || position >=32) return value;
    return value & ~(1 << position);
}

// Toggle bit at position (flip it)
unsigned int ToggleBit(unsigned int value, int position) {
    if (position <0 || position >=32) return value;
    return value ^(1 << position);
}

// Get the value of bit at position
int GetBit(unsigned int value, int position) {
    if (position <0 || position >=32) return 0;
    return (value >> position) & 1;
}

// Count number of 1s in the value
int CountBits(unsigned int value) {
    int count = 0;
    while (value) {
        value &= value -1;  // Clear the lowest set bit
        count++;
    }
    return count;
}


// Shift value left by positions
unsigned int ShiftLeft(unsigned int value, int positions) {
    if (positions <0 || positions >=32) return value;
    return value << positions;
}

// Shift value right by positions
unsigned int ShiftRight(unsigned int value, int positions){
    if (positions <0 || positions >=32) return value;
    return value >> positions;
}

// Print value in binary format
void PrintBinary(unsigned int value) {
    printf("Binary: ");
    for (int i=31; i >=0; i--){
        printf("%d", (value >> i) & 1);
        if (i %4 == 0 && i !=0) printf(" ");  // Space every 4 bits
    }
    printf("\n");
}

// Print value in hexadecimal format
void PrintHex(unsigned int value){
    printf("Hex: 0x%08X\n", value);
}

// ========== PHASE 2: GAME FUNCTIONS ==========
// Starting game with starting positions
void InitGame(GameState* game) {
    // Player 1 (Red) starts at bottom (rows 0, 1, 2)
    // Manually set each dark square position
    game->player1_pieces = 0ULL;
    
    // Row 0: positions 1, 3, 5, 7 (dark squares in row 0)
    game->player1_pieces |= (1ULL << 1);
    game->player1_pieces |= (1ULL << 3);
    game->player1_pieces |= (1ULL << 5);
    game->player1_pieces |= (1ULL << 7);
    
    // Row 1: positions 8, 10, 12, 14 (dark squares in row 1)
    game->player1_pieces |= (1ULL << 8);
    game->player1_pieces |= (1ULL << 10);
    game->player1_pieces |= (1ULL << 12);
    game->player1_pieces |= (1ULL << 14);
    
    // Row 2: positions 17, 19, 21, 23 (dark squares in row 2)
    game->player1_pieces |= (1ULL << 17);
    game->player1_pieces |= (1ULL << 19);
    game->player1_pieces |= (1ULL << 21);
    game->player1_pieces |= (1ULL << 23);
    
    game->player1_kings = 0ULL;
    
    // Player 2 (Black) starts at top (rows 5, 6, 7)
    game->player2_pieces = 0ULL;
    
    // Row 5: positions 40, 42, 44, 46 (dark squares in row 5)
    game->player2_pieces |= (1ULL << 40);
    game->player2_pieces |= (1ULL << 42);
    game->player2_pieces |= (1ULL << 44);
    game->player2_pieces |= (1ULL << 46);
    
    // Row 6: positions 49, 51, 53, 55 (dark squares in row 6)
    game->player2_pieces |= (1ULL << 49);
    game->player2_pieces |= (1ULL << 51);
    game->player2_pieces |= (1ULL << 53);
    game->player2_pieces |= (1ULL << 55);
    
    // Row 7: positions 56, 58, 60, 62 (dark squares in row 7)
    game->player2_pieces |= (1ULL << 56);
    game->player2_pieces |= (1ULL << 58);
    game->player2_pieces |= (1ULL << 60);
    game->player2_pieces |= (1ULL << 62);
    
    game->player2_kings = 0ULL;
    game->current_player = 1;  // Player 1 starts
}

// Convert position to row (0-7)
int PositionToRow(int position) {
    return position >> 3; //Divide by 8 using right shifts
}

// Convert position to column (0-7)
int PositionToCol(int position) {
    return position & 7;  // Modulo 8 using AND with 0b111
}

// Convert row and column to position
int RowColToPosition(int row, int col) {
    return (row << 3) | col;
}

// Check if a position is occupied by any piece
int IsOccupied(GameState* game, int position) {
    unsigned long long mask= 1ULL << position;
    unsigned long long all_pieces = game->player1_pieces | game->player1_kings |
                                     game->player2_pieces | game->player2_kings;
    return (all_pieces & mask) != 0;
}

// Print the game board
void PrintBoard(GameState* game) {
    printf("\n  0 1 2 3 4 5 6 7\n");
    printf(" +----------------+\n");

    for (int row= 7; row >= 0; row--) {
        printf("%d|", row);
        for (int col= 0; col <8; col++) {
            int position = RowColToPosition(row, col);
            unsigned long long mask = 1ULL << position;

            // Check what's at this position
            if (game->player1_kings & mask) {
                printf("R ");  // Red King
            } else if (game->player1_pieces & mask) {
                printf("r ");  // Red piece
            } else if (game->player2_kings & mask) {
                printf("B ");  // Black King
            } else if (game->player2_pieces & mask) {
                printf("b ");  // Black piece
            } else if ((row + col) %2 == 1) {
                printf("□ ");  // Dark square (playable)
            } else {
                printf("  ");  // Light square
            }
        }
        printf("|\n");
    }
    printf(" +----------------+\n");
    printf("  0 1 2 3 4 5 6 7\n");
    printf("\nPlayer %d's turn\n", game->current_player);
}

// Promote a piece to king if it reaches the opposite end
void PromoteToKing(GameState* game, int position) {
    int row = PositionToRow(position);
    unsigned long long mask = 1ULL << position;

    if (row == 7 && (game->player1_pieces & mask)) {
        game->player1_pieces &= ~mask;
        game->player1_kings |= mask;
        printf("Player 1 promoted to KING!\n");
    }
    if (row == 0 && (game->player2_pieces & mask)) {
        game->player2_pieces &= ~mask;
        game->player2_kings |= mask;
        printf("Player 2 promoted to KING!\n");
    }
}

// Helper: Get player and opponent pieces
void GetPieces(GameState* game, int player, unsigned long long* my_pieces, unsigned long long* opp_pieces) {
    if (player == 1) {
        *my_pieces = game->player1_pieces | game->player1_kings;
        *opp_pieces = game->player2_pieces | game->player2_kings;
    } else {
        *my_pieces = game->player2_pieces | game->player2_kings;
        *opp_pieces = game->player1_pieces | game->player1_kings;
    }
}

// Check if a jump/capture is possible for any piece
int HasCaptureMoves(GameState* game, int player) {
    unsigned long long my_pieces, opp_pieces;
    GetPieces(game, player, &my_pieces, &opp_pieces);

    for (int from = 0; from < 64; from++) {
        if (!(my_pieces & (1ULL << from))) continue;

        int is_king = (player == 1) ? (game->player1_kings & (1ULL << from)) :
                                      (game->player2_kings & (1ULL << from));
        int from_row = PositionToRow(from);
        int from_col = PositionToCol(from);

        // Try all 4 jump directions (2 squares diagonally)
        int jumps[4][2] = {{2,2}, {2,-2}, {-2,2}, {-2,-2}};
        
        for (int d =0; d < 4; d++) {
            // Non-kings can't jump backward
            if (!is_king && ((player == 1 && jumps[d][0] <0) || (player == 2 && jumps[d][0] >0)))
                continue;
            
            int to_row = from_row + jumps[d][0];
            int to_col = from_col + jumps[d][1];
            
            if (to_row < 0 || to_row >= 8 || to_col < 0 || to_col >= 8) continue;
            
            int mid_pos = RowColToPosition(from_row + jumps[d][0]/2, from_col + jumps[d][1]/2);
            int to_pos = RowColToPosition(to_row, to_col);

            // Valid jump: opponent in middle, empty destination
            if ((opp_pieces & (1ULL << mid_pos)) && !IsOccupied(game, to_pos)) {
                return 1; // Found a capture!
            }
        }
    }
    return 0;
}
// Check if a specific piece at a position has capture moves available
int HasCaptureMovesFromPosition(GameState* game, int player, int position) {
    unsigned long long my_pieces, opp_pieces;
    GetPieces(game, player, &my_pieces, &opp_pieces);

    // Check if piece exists at this position
    if (!(my_pieces & (1ULL << position))) return 0;

    int is_king = (player == 1) ? (game->player1_kings & (1ULL << position)) :
                                  (game->player2_kings & (1ULL << position));
    int from_row = PositionToRow(position);
    int from_col = PositionToCol(position);

    // Try all 4 jump directions
    int jumps[4][2] = {{2, 2}, {2, -2}, {-2, 2}, {-2, -2}};

    for (int d = 0; d < 4; d++) {
        // Non-kings can't jump backward
        if (!is_king && ((player == 1 && jumps[d][0] < 0) || (player == 2 && jumps[d][0] > 0)))
            continue;

        int to_row = from_row + jumps[d][0];
        int to_col = from_col + jumps[d][1];

        if (to_row < 0 || to_row >= 8 || to_col < 0 || to_col >= 8) continue;

        int mid_pos = RowColToPosition(from_row + jumps[d][0]/2, from_col + jumps[d][1]/2);
        int to_pos = RowColToPosition(to_row, to_col);

        // Valid jump: opponent in middle, empty destination
        if ((opp_pieces & (1ULL << mid_pos)) && !IsOccupied(game, to_pos)) {
            return 1; // Found a capture from this position!
        }
    }
    return 0;
}

// Validate and detect captures
int IsValidMove(GameState* game, int from, int to, int* captured_pos) {
    if (captured_pos) *captured_pos = -1;
    if (from < 0 || from >= 64 || to < 0 || to >= 64) return 0;
    
    unsigned long long my_pieces, opp_pieces;
    GetPieces(game, game->current_player, &my_pieces, &opp_pieces);
    
    unsigned long long from_mask = 1ULL << from;
    
    if (!(my_pieces & from_mask)) return 0; // No piece at start
    if (IsOccupied(game, to)) return 0;     // Destination occupied
    
    int from_row = PositionToRow(from);
    int from_col = PositionToCol(from);
    int to_row = PositionToRow(to);
    int to_col = PositionToCol(to);
    
    if ((to_row + to_col) % 2 == 0) return 0; // Not a dark square
    
    int row_diff = to_row - from_row;
    int col_diff = to_col - from_col;
    
    // Check if the piece at 'from' is a king - explicitly convert to boolean
    int is_king = 0;
    if (game->current_player == 1) {
        is_king = ((game->player1_kings & from_mask) != 0);
    } else {
        is_king = ((game->player2_kings & from_mask) != 0);
    }

    // Check for mandatory captures
    int must_capture = HasCaptureMoves(game, game->current_player);
    
    // Jump move (2 squares)
    if (abs(row_diff) == 2 && abs(col_diff) == 2) {
        // Non-kings can't jump backward, kings can jump in any direction
        if (!is_king) {
            if ((game->current_player == 1 && row_diff < 0) || 
                (game->current_player == 2 && row_diff > 0)) {
                return 0;
            }
        }
        
        int mid_pos = RowColToPosition(from_row + row_diff/2, from_col + col_diff/2);
        
        if (opp_pieces & (1ULL << mid_pos)) {
            if (captured_pos) *captured_pos = mid_pos;
            return 1; // Valid capture
        }
        return 0;
    }
    
    // Simple move (1 square)
    if (abs(row_diff) == 1 && abs(col_diff) == 1) {
        if (must_capture) {
            printf("Must capture when possible!\n");
            return 0; // Can't do simple move if capture available
        }
        
        // Non-kings can't move backward, kings can move in any direction
        if (!is_king) {
            if ((game->current_player == 1 && row_diff < 0) || 
                (game->current_player == 2 && row_diff > 0)) {
                return 0;
            }
        }
        
        return 1; // Valid simple move
    }
    return 0; // Invalid move
}

int MakeMove(GameState* game, int from, int to) {
    int captured_pos = -1;
    if (!IsValidMove(game, from, to, &captured_pos)) return 0;
    
    unsigned long long from_mask = 1ULL << from;
    unsigned long long to_mask = 1ULL << to;
    
    // Move piece
    if (game->current_player == 1) {
        if (game->player1_pieces & from_mask) {
            game->player1_pieces = (game->player1_pieces & ~from_mask) | to_mask;
        } else {
            game->player1_kings = (game->player1_kings & ~from_mask) | to_mask;
        }
    } else {
        if (game->player2_pieces & from_mask) {
            game->player2_pieces = (game->player2_pieces & ~from_mask) | to_mask;
        } else {
            game->player2_kings = (game->player2_kings & ~from_mask) | to_mask;
        }
    }
    
    // Remove captured piece
    int was_capture = 0;
    if (captured_pos != -1) {
        was_capture = 1;
        unsigned long long cap_mask = ~(1ULL << captured_pos);
        game->player1_pieces &= cap_mask;
        game->player1_kings &= cap_mask;
        game->player2_pieces &= cap_mask;
        game->player2_kings &= cap_mask;
        printf("Captured piece at %d!\n", captured_pos);
    }

    PromoteToKing(game, to);

    // Check for multi-jump: if this was a capture, check if same piece can capture again
    if (was_capture && HasCaptureMovesFromPosition(game, game->current_player, to)) {
        printf("Multi-jump available! Continue capturing with the same piece.\n");
        // Don't switch players - same player continues with same piece
        return 2; // Return 2 to indicate multi-jump available
    }
    // Normal end of turn - switch players
        game->current_player = (game->current_player == 1) ? 2 : 1;
    return 1;
}


int CheckWinner(GameState* game) {
    unsigned long long p1 = game->player1_pieces | game->player1_kings;
    unsigned long long p2 = game->player2_pieces | game->player2_kings;
    if (p1 == 0) return 2;
    if (p2 == 0) return 1;
    return 0;
}

// ========== AI FUNCTIONS ==========

int GetAllValidMoves(GameState* game, int player, Move* moves, int max_moves) {
    int count = 0;
    unsigned long long my_pieces, opp_pieces;
    GetPieces(game, player, &my_pieces, &opp_pieces);
    
    int must_capture = HasCaptureMoves(game, player);
    int saved_player = game->current_player;
    game->current_player = player;
    
    for (int from = 0; from < 64 && count < max_moves; from++) {
        if (!(my_pieces & (1ULL << from))) continue;
        
        int is_king = (player == 1) ? (game->player1_kings & (1ULL << from)) : 
                                      (game->player2_kings & (1ULL << from));
        int from_row = PositionToRow(from);
        int from_col = PositionToCol(from);
        
        // Try all moves (1 square and 2 square jumps)
        int dirs[8][2] = {{1,1}, {1,-1}, {-1,1}, {-1,-1}, {2,2}, {2,-2}, {-2,2}, {-2,-2}};
        
        for (int d = 0; d < 8; d++) {
            int is_jump = (abs(dirs[d][0]) == 2);
            
            // Skip if must capture but this isn't a jump
            if (must_capture && !is_jump) continue;
            
            // Non-kings can't move backward
            if (!is_king && ((player == 1 && dirs[d][0] < 0) || (player == 2 && dirs[d][0] > 0)))
                continue;
            
            int to_row = from_row + dirs[d][0];
            int to_col = from_col + dirs[d][1];
            
            if (to_row < 0 || to_row >= 8 || to_col < 0 || to_col >= 8) continue;
            
            int to = RowColToPosition(to_row, to_col);
            int cap_pos = -1;
            
            if (IsValidMove(game, from, to, &cap_pos)) {
                moves[count].from = from;
                moves[count].to = to;
                moves[count].is_capture = (cap_pos != -1);
                moves[count].captured_pos = cap_pos;
                moves[count].score = (cap_pos != -1) ? 10 : 1;
                count++;
            }
        }
    }
    
    game->current_player = saved_player;
    return count;
}

int GetAllCaptureMoves(GameState* game, int player, Move* moves, int max_moves) {
    Move all_moves[100];
    int total = GetAllValidMoves(game, player, all_moves, 100);
    int count = 0;
    
    for (int i = 0; i < total && count < max_moves; i++) {
        if (all_moves[i].is_capture) {
            moves[count++] = all_moves[i];
        }
    }
    return count;
}

int AIMove(GameState* game) {
    Move moves[100];
    int count = GetAllValidMoves(game, 2, moves, 100);
    
    if (count == 0) return 0;
    
    // Pick best move (prefer captures)
    int best = 0;
    for (int i = 1; i < count; i++) {
        if (moves[i].score > moves[best].score) best = i;
    }
    if (moves[best].is_capture) {
        printf("AI captures at position %d!\n", moves[best].captured_pos);
    }
    return MakeMove(game, moves[best].from, moves[best].to);
}


