// Include the dependencies
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Define the variables
#define EMPTY 0
#define X 1
#define O 2 // Change O to 2 for indexing

// Define some active elements of the game
typedef struct {
    int board[3][3];
    int current_player;
} TicTacToe;

// Initialize the game
void init_game(TicTacToe *game) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            game->board[i][j] = EMPTY;
        }
    }
    game->current_player = X;
}

// Print the current state of the board
void print_board(TicTacToe *game) {
    printf("Current Board State:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (game->board[i][j] == X) {
                printf(" X ");
            } else if (game->board[i][j] == O) {
                printf(" O ");
            } else {
                printf(" . "); // Empty cell
            }
            if (j < 2) printf("|"); // Print column separator
        }
        printf("\n");
        if (i < 2) printf("-----------\n"); // Print row separator
    }
    printf("\n");
}

// Check if there is a winner
int check_winner(TicTacToe *game) {
    // Check for horizontal wins
    for (int i = 0; i < 3; i++) {
        if (game->board[i][0] == game->board[i][1] &&
            game->board[i][1] == game->board[i][2] &&
            game->board[i][0] != EMPTY) {
            return game->board[i][0]; // Return the winner (X or O)
            }
    }
    // Check for vertical wins
    for (int i = 0; i < 3; i++) {
        if (game->board[0][i] == game->board[1][i] &&
            game->board[1][i] == game->board[2][i] &&
            game->board[0][i] != EMPTY) {
            return game->board[0][i]; // Return the winner (X or O)
            }
    }
    // Check for diagonal wins
    if (game->board[0][0] == game->board[1][1] &&
        game->board[1][1] == game->board[2][2] &&
        game->board[0][0] != EMPTY) {
        return game->board[0][0]; // Return the winner (X or O)
        }
        if (game->board[0][2] == game->board[1][1] &&
            game->board[1][1] == game->board[2][0] &&
            game->board[0][2] != EMPTY) {
            return game->board[0][2]; // Return the winner (X or O)
            }
            // Check for draw or ongoing
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (game->board[i][j] == EMPTY) {
                        return 0; // Game is still ongoing
                    }
                }
            }
            return EMPTY; // Draw
}

// Swap players
void swap(TicTacToe *game) {
    game->current_player = (game->current_player == X) ? O : X;
}

// Temporal difference agent
typedef struct {
    double V[19683]; // Value function for all states
    double alpha;
    double gamma;
    double epsilon;
} TDAgent;

// Convert state to index just assigning a unique identifier to a possible state
int state_to_index(int state[9]) {
    int index = 0;
    for (int i = 0; i < 9; i++) {
        index += state[i] * (int)pow(3, i);
    }
    return index;
}

// Choose action using epsilon-greedy algorithm
int choose_action(TDAgent *agent, int state[9]) {
    if ((double)rand() / RAND_MAX < agent->epsilon) {
        int available_moves[9];
        int count = 0;
        for (int i = 0; i < 9; i++) {
            if (state[i] == EMPTY) { // Only consider empty cells as available moves
                available_moves[count++] = i;
            }
        }
        if(count > 0) { // Ensure there are available moves
            return available_moves[rand() % count]; // Randomly select from available moves
        } else {
            return -1; // No valid action available, should not happen in normal play
        }
    } else {
        int best_action = -1;
        double best_value = -9999.0;

        for (int i = 0; i < 9; i++) {
            if (state[i] == EMPTY) { // Only consider empty cells
                int next_state[9];
                for (int j = 0; j < 9; j++) {
                    next_state[j] = state[j];
                }
                next_state[i] = X; // Assume the agent is X

                int index = state_to_index(next_state);
                if (agent->V[index] > best_value) {
                    best_value = agent->V[index];
                    best_action = i;
                }
            }
        }
        return best_action;
    }
}

// Update value function
void update_value_function(TDAgent *agent, int state[9], double reward, int next_state[9]) {
    int index = state_to_index(state);
    int next_index = state_to_index(next_state);

    agent->V[index] += agent->alpha * (reward + agent->gamma * agent->V[next_index] - agent->V[index]);
}

int main() {
    srand(time(NULL)); // Seed random number generation

    TDAgent agent = {.alpha = 0.1, .gamma = 0.9, .epsilon = 1.0}; // Start with higher epsilon

    int num_episodes = 10000; // Define number of episodes to train

    for (int episode = 0; episode < num_episodes; episode++) {
        TicTacToe game;
        init_game(&game);

        int state[9];

        while (1) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    state[i * 3 + j] = game.board[i][j];
                }
            }

            int action = choose_action(&agent, state);
            if(action == -1){ break;} // No valid action

            game.board[action / 3][action % 3] = game.current_player;

            print_board(&game); // Print board after each move

            int winner = check_winner(&game);
            int next_state[9];

            for (int k = 0; k < 9; k++) {
                next_state[k] = state[k];
            }

            if (winner != EMPTY) {
                double reward;
                if (winner == X) reward = 1.0;
                else if (winner == O) reward = -1.0;
                else reward = 0.5;

                update_value_function(&agent, state, reward, next_state);
                break;
            }

            update_value_function(&agent, state, 0.0, next_state); // No immediate reward

            swap(&game); // Switch players
        }

        agent.epsilon *= 0.99; // Decay epsilon after each episode
    }

    printf("Mama Ooooooo completed.\n");

    return 0;
}
