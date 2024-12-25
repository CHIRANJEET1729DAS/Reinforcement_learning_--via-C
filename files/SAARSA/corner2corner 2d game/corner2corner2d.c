#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GRID_SIZE 5
#define NUM_ACTIONS 4
#define EPSILON 0.1
#define ALPHA 0.1
#define HAMMA 0.9
#define NUM_EPISODES 10000

double Q[GRID_SIZE][GRID_SIZE][NUM_ACTIONS];

int get_reward(int x ,int y){
  if(x==GRID_SIZE-1&&y==GRID_SIZE-1){
    return 10;
  }
  return -1;
}

int choose_action(int x,int y){
  if ((double)rand()/RAND_MAX <EPSILON){
    return rand()%NUM_ACTIONS;
  }else{
    int best_action = 0 ;
    for(int a=1; a<NUM_ACTIONS;a++){
      if (Q[x][y][a] > Q[x][y][best_action]){
        best_action = a;
      }
    }
    return best_action;
  }
}

void sarsa() {
  for (int episode = 0; episode < NUM_EPISODES; episode++) {
    int x = 0, y = 0;
    int action = choose_action(x, y);
    int steps = 0; // Step counter

    while (x != GRID_SIZE - 1 || y != GRID_SIZE - 1) { // Until reaching bottom-right corner
      steps++; // Increment step counter
      int next_x = x;
      int next_y = y;

      switch (action) {
        case 0: next_x = (x > 0) ? x - 1 : x; break; // Up
        case 1: next_x = (x < GRID_SIZE - 1) ? x + 1 : x; break; // Down
        case 2: next_y = (y > 0) ? y - 1 : y; break; // Left
        case 3: next_y = (y < GRID_SIZE - 1) ? y + 1 : y; break; // Right
      }

      int reward = get_reward(next_x, next_y);
      int next_action = choose_action(next_x, next_y);
      Q[x][y][action] += ALPHA * (reward + HAMMA * Q[next_x][next_y][next_action] - Q[x][y][action]);

      x = next_x;
      y = next_y;
      action = next_action;
    }

    if (episode % 10 == 0) { // Print every 10 episodes
      printf("Episode %d completed in %d steps.\n", episode, steps);
      fflush(stdout); // Ensure immediate output
    }
  }
}

int main(){
  srand(time(NULL));

  for (int i=0;i<GRID_SIZE;i++){
    for (int j=0;j<GRID_SIZE;j++){
      for (int k=0;k<NUM_ACTIONS;k++){
        Q[i][j][k] = 0.0;
      }
    }
  }

  sarsa();

  printf("Learned Q values : \n");
  for(int i = 0;i<GRID_SIZE;i++){
    for(int j=0;j<GRID_SIZE;j++){
      printf("State (%d,%d): ",i,j);
      for (int k = 0;k<NUM_ACTIONS;k++){
        printf("Action %d: %.2f",k,Q[i][j][k]);
      }
      printf("\n");
    }
  }
  return 0;
}
