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

bool is_solved(struct game_state *state) 
{
    uint8_t goal[4][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 0}
    };

    //compare state
    for (int row = 0; row < 4; row++) 
    {
        for (int col = 0; col < 4; col++) 
        {
            if (state->tiles[row][col] != goal[row][col]) 
            {
                return false;
            }
        }
    }
    return true;
}

//the gd serialized thingy is not helpful for this
struct board_config 
{
    uint8_t tiles[4][4];
    struct board_config *next;
};

bool is_board_visited(struct board_config *visited_head, uint8_t tiles[4][4]) 
{
    struct board_config *current = visited_head;
    while (current != NULL) 
    {
        //compare grids
        bool match = true;
        for (int row = 0; row < 4 && match; row++) 
        {
            for (int col = 0; col < 4 && match; col++) 
            {
                if (current->tiles[row][col] != tiles[row][col]) 
                {
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

void add_board_visited(struct board_config **visited_head, uint8_t tiles[4][4]) 
{
    struct board_config *new_node = malloc(sizeof(struct board_config));

    for (int row = 0; row < 4; row++) 
    {
        for (int col = 0; col < 4; col++) 
        {
            new_node->tiles[row][col] = tiles[row][col];
        }
    }
    new_node->next = *visited_head;
    *visited_head = new_node;
}

//it needs freed
void free_visited(struct board_config *visited_head) 
{
    struct board_config *current = visited_head;
    while (current != NULL) 
    {
        struct board_config *next = current->next;
        free(current);
        current = next;
    }
}

int number_of_moves(struct game_state start) 
{
    
    if (is_solved(&start)) 
    {
        return 0;//default soln
    }
    
    struct queue q = {0}; //initialize queue
    struct board_config *visited_head = NULL; 
    add_board_visited(&visited_head, start.tiles);
    enqueue(&q, start); //add initial state
    
    int result = -1; //default
    
    while (q.data.head != NULL) 
    {
        struct game_state current = dequeue(&q);//deque to check if we won
        if (is_solved(&current)) 
        {
            result = current.num_steps;
            break; 
        }
        struct game_state moves[4];// Try each direction
        moves[0] = current;
        moves[1] = current;
        moves[2] = current;
        moves[3] = current;
        move_up(&moves[0]);
        move_down(&moves[1]);
        move_left(&moves[2]);
        move_right(&moves[3]);
        
        for (int i = 0; i < 4; i++) {
            if (moves[i].num_steps > current.num_steps) 
            {
                if (!is_board_visited(visited_head, moves[i].tiles)) 
                {
                    add_board_visited(&visited_head, moves[i].tiles);
                    enqueue(&q, moves[i]);
                }
            }
        }
    }
    
    free_visited(visited_head);
    free_list(q.data);
    
    return result; 
}