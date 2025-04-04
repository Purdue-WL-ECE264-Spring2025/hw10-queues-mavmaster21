#include "queue.h"
#include "tile_game.h"

/////MODIFY THIS FILE

void enqueue(struct queue *q, struct game_state state) 
{
    int serialized = serialize(state);
    insert_at_tail(&q->data, serialized);
}

struct game_state dequeue(struct queue *q) 
{ 
    size_t value = remove_from_head(&q->data);
    return (deserialize(value)); 
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

int number_of_moves(struct game_state start) 
{
    struct queue q;
    q.data.head = NULL;

    enqueue(&q, start);

    bool visited[1 << 16] = {0};

    visited[serialize(start)] = true;

    while (q.data.head != NULL) {
        struct game_state current_state = dequeue(&q);
        
        // Check if current state is the solved state
        if (is_solved(&current_state)) {
            return current_state.num_steps;  // Return number of moves taken to reach the goal
        }

        // Try all 4 possible moves
        struct game_state next_state = current_state;
        
        // Move up
        next_state = current_state;
        move_up(&next_state);
        if (!visited[serialize(next_state)]) {
            visited[serialize(next_state)] = true;
            enqueue(&q, next_state);
        }

        // Move down
        next_state = current_state;
        move_down(&next_state);
        if (!visited[serialize(next_state)]) {
            visited[serialize(next_state)] = true;
            enqueue(&q, next_state);
        }

        // Move left
        next_state = current_state;
        move_left(&next_state);
        if (!visited[serialize(next_state)]) {
            visited[serialize(next_state)] = true;
            enqueue(&q, next_state);
        }

        // Move right
        next_state = current_state;
        move_right(&next_state);
        if (!visited[serialize(next_state)]) {
            visited[serialize(next_state)] = true;
            enqueue(&q, next_state);
        }
    }

    return -1;  // Return -1 if no solution is found (shouldn't happen in a solvable puzzle)
}


