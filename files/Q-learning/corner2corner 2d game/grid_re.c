//first we import dependencies

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//define some variables

#define GRID_SIZE 5
#define NUM_ACTIONS 4
#define NUM_EPISODES 1000
#define MAX_STEPS 50

//define hyperparameters

double alpha = 0.1;
double hamma = 0.9;
double epsilon = 1.0;
double epsilon_decay = 0.99;
double min_epsilon = 0.1;

//define Q table

double q_table[GRID_SIZE][GRID_SIZE][NUM_ACTIONS] = {0};

//define the function to choose using epsilon-greedy policy

int get_next_action(int x,int y){

   if ((double)rand()/RAND_MAX <epsilon){
     return rand()%NUM_ACTIONS;
   }else {
    int best_action = 0;
    for (int a = 1; a< NUM_ACTIONS ;a++){
     if (q_table[x][y][a] > q_table[x][y][best_action]){
      best_action=a;
     }
    }
    return best_action;
   }
}

//define the function  to take a step in the environment based on the action chosen
void step(int* x, int* y, int action){

   switch(action){
    case 0 :
	 if (*x>0) (*x)--;
	 break;
    case 1 :
	 if (*x<GRID_SIZE-1)(*x)++;
	 break;
    case 2 :
	 if (*y>0) (*y)--;
	 break;
    case 3 :
	 if (*y<GRID_SIZE-1)(*y)++;
	 break;
   }
}

int num_of_steps = 0;

int main(){

  srand(time(NULL));

  for (int episode = 0; episode <NUM_EPISODES;episode++){
    int x=0;
    int y=0;
    for (int step_count=0;step_count < MAX_STEPS;step_count++){

      double reward;

      int action = get_next_action(x,y);
      int next_x = x;
      int next_y = y;
      step(&next_x,&next_y,action);

      if (next_x==GRID_SIZE-1 && next_y==GRID_SIZE-1){
	   reward += 1;
      }else{
	   reward -= 0.01;
      }
      
      num_of_steps += 1;

      q_table[x][y][action] += alpha * (reward + hamma * q_table[next_x][next_y][get_next_action(next_x,next_y)]-q_table[x][y][action]);
      x = next_x;
      y = next_y;
      if (next_x == GRID_SIZE-1 && next_y == GRID_SIZE-1){
	      printf("Step number :: %d\n", num_of_steps);
	      break;
      }}
      epsilon = fmax(min_epsilon,epsilon*epsilon_decay);
   }

   printf("MAmmaa OOOOooo completed \n");
   printf("Q-table:: \n");
   

   for (int i = 0; i < GRID_SIZE; i++) {
       for (int j = 0; j < GRID_SIZE; j++) {
            printf("State (%d,%d): ", i, j);
            for (int a = 0; a < NUM_ACTIONS; a++) {
                printf("Action %d: %.2f ", a, q_table[i][j][a]);
            }
            printf("\n");
        }
  }
}

