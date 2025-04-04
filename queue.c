#include "queue.h"
#include "tile_game.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/////MODIFY THIS FILE

void enqueue(struct queue *q, struct game_state state) 
{
    uint64_t serialized = serialize(state);
    insert_at_tail(&q->data, serialized);
}

struct game_state dequeue(struct queue *q) 
{ 
    size_t value = remove_from_head(&q->data);
    return deserialize(value); 
}

bool is_solved(struct game_state *state) {
    // Define the solved configuration (goal state)
    uint8_t goal[4][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 0}
    };

    // Compare current state with the goal state
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (state->tiles[row][col] != goal[row][col]) {
                return false;
            }
        }
    }
    return true;
}

// Helper structure to store just the board configuration
struct board_config {
    uint8_t tiles[4][4];
    struct board_config *next;
};

// Function to check if a board configuration has been visited
bool is_board_visited(struct board_config *visited_head, uint8_t tiles[4][4]) {
    struct board_config *current = visited_head;
    while (current != NULL) {
        // Compare the entire 4x4 grid
        bool match = true;
        for (int row = 0; row < 4 && match; row++) {
            for (int col = 0; col < 4 && match; col++) {
                if (current->tiles[row][col] != tiles[row][col]) {
                    match = false;
                }
            }
        }
        
        if (match) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// Function to add a board configuration to the visited list
void add_board_visited(struct board_config **visited_head, uint8_t tiles[4][4]) {
    struct board_config *new_node = malloc(sizeof(struct board_config));
    
    // Copy the tile configuration
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            new_node->tiles[row][col] = tiles[row][col];
        }
    }
    
    new_node->next = *visited_head;
    *visited_head = new_node;
}

// Function to free the visited list
void free_visited(struct board_config *visited_head) {
    struct board_config *current = visited_head;
    while (current != NULL) {
        struct board_config *next = current->next;
        free(current);
        current = next;
    }
}

int number_of_moves(struct game_state start) {
    // If already solved, return 0
    if (is_solved(&start)) {
        return 0;
    }
    
    struct queue q = {0}; // Initialize the queue
    struct board_config *visited_head = NULL; // Initialize visited list
    
    // Add the initial state to visited and queue
    add_board_visited(&visited_head, start.tiles);
    enqueue(&q, start);
    
    while (q.data.head != NULL) {
        struct game_state current = dequeue(&q); // Dequeue the current state
        
        // Check if the current state is the goal state
        if (is_solved(&current)) {
            int moves = current.num_steps;
            free_visited(visited_head); // Clean up before returning
            printf("%d\n",moves);
            return moves;
        }
        
        // Try all four possible moves
        struct game_state moves[4];
        
        // Make copies of the current state for each move
        moves[0] = current;
        moves[1] = current;
        moves[2] = current;
        moves[3] = current;
        
        // Apply each move
        move_up(&moves[0]);
        move_down(&moves[1]);
        move_left(&moves[2]);
        move_right(&moves[3]);
        
        // Check and enqueue valid moves
        for (int i = 0; i < 4; i++) {
            // Check if the move actually changed the state (by comparing num_steps)
            if (moves[i].num_steps > current.num_steps) {
                // If this state hasn't been visited yet, add it to queue and visited list
                if (!is_board_visited(visited_head, moves[i].tiles)) {
                    add_board_visited(&visited_head, moves[i].tiles);
                    enqueue(&q, moves[i]);
                }
            }
        }
    }
    
    // Clean up memory
    free_visited(visited_head);
    
    return -1; // No solution found
}