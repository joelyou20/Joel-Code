//**********************************************************************************
// Citations: stackexchange.com, thegeekstuff.com, stackoverflow.com, Jianping Pan 
// Assignment Number: 2
// Author: Joel Cribby
// Version: 4.0
// Course: CSC 360
//
// Program Name: ptrains.c
// Program Description: This program is designed to test multi-thread program
//  implementation. The program does this by simulating a train crossing structure.
//  Each train must be loaded to an appropriate station and must cross the track to
//  the opposite station one at a time.
//**********************************************************************************
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>

#define PTHREAD_BUF_SIZE 100
#define BILLION 1000000000L

//Primary Train Data Structure
struct Train {
    int train_no;
    char priority;
    char* direction;
    int loading_time;
    int crossing_time;
    int current_state;

    pthread_t train;
    pthread_cond_t allowed;
};

// Priority Queue Implementation - citation: www.geeksforgeeks.org ------------------
// Modified to function with a 'Train' structure


//PQ Node
struct Node {
    struct Train* ptrain;
    int priority;
    struct Node* next;
};

//PQ function to initialize a new node
struct Node* createNode(struct Train* ptrain, int priority) {
    struct Node* temp_node = (struct Node*)malloc(sizeof(struct Node*)*100);
    temp_node -> ptrain = ptrain;
    temp_node -> priority = priority;
    temp_node -> next = NULL;

    return temp_node;
}

//PQ function to return the highest priority node in a specific PQ
struct Train* getHead(struct Node** node) {
    return(*node) -> ptrain;
}

//Removes the highest priority node from a specific PQ
void dequeue(struct Node** node) {
    struct Node* temp_node = *node;
    (*node) = (*node) -> next;
    free(temp_node);
}

//Adds a new node to a specific queue and supplies a prioity integer
void enqueue(struct Node** head, struct Train* ptrain, int priority) {
    struct Node* tail = (*head);
    struct Node* temp_node = createNode(ptrain, priority);
    if((*head) -> priority > priority) {
        temp_node -> next = *head;
        (*head) = temp_node;
    } else {
        while(tail -> next != NULL && tail -> next -> priority < priority) {
            tail = tail -> next;
        }
        temp_node -> next = tail -> next;
        tail -> next = temp_node;
    }
}

//Returns true if the specific PQ is empty
int isEmpty(struct Node** node) {
    return (*node) == NULL;
}
//End of PQ section-------------------------------------------------------------------

/*State Legend:
 * 0 = Waiting
 * 1 = Ready
 * 2 = Loading
 * 3 = Loaded
 * 4 = Crossed
 */

//Mutex and convar initialization section
pthread_mutex_t westStation = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t eastStation = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mainTrack = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t dispatcher = PTHREAD_COND_INITIALIZER;

//Priority queue and waiting trains array defined
struct Node* west_station_low;
struct Node* west_station_high;
struct Node* east_station_low;
struct Node* east_station_high;
struct Train** waiting_trains;

//Global variables
int trains_left = 0;
int trains_ready = 0;
float accum = 0.0;

//Prototypes
void* runPtrain(void* train);
void* runDispatcher(void* count);

//Function to output formatted timestamps
void outputTime(struct timespec start, struct timespec stop) {
    float seconds;
    int hours, minutes;
    accum += (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) /BILLION;
    seconds = accum;
    while(seconds >= 60) {
        seconds -= 60;
    }
    minutes = accum / 60;
    while(minutes >= 60) {
        minutes -= 60;
    }
    while(hours >= 60) {
        hours -= 60;
    }
    printf("%02d:%02d:%04.1f ", hours, minutes, seconds);
} 

//Train function run at creation of each train pthread
void *runPtrain(void* ptrain) {
    struct Train *new_ptrain = (struct Train*)ptrain;
    struct timespec start, stop;

    pthread_cond_init(&new_ptrain -> allowed, NULL); 
    new_ptrain -> current_state = 0;
    pthread_cond_signal(&dispatcher);

    if(clock_gettime(CLOCK_REALTIME, &start) == -1) {
        perror("clock gettime");
        exit(EXIT_FAILURE);
    }
    trains_ready++;
    
    //Let dispatcher know that train has been built and wait to load

    pthread_cond_signal(&dispatcher);
    pthread_mutex_lock(&mainTrack);    
    pthread_cond_wait(&dispatcher, &mainTrack);

    //Run loading time wait
    usleep(new_ptrain -> loading_time * 100000);

    //Enqueue train into appropriate station
    if(strcmp(new_ptrain -> direction, "East") == 0) {
        pthread_mutex_lock(&eastStation);
        if(new_ptrain -> priority == 'H') {
            if(isEmpty(&east_station_high)) {
                east_station_high = createNode(new_ptrain, 1);
            } else {
                enqueue(&east_station_high, new_ptrain, 1);
            }
        } else {
            if(isEmpty(&east_station_low)) {
                east_station_low = createNode(new_ptrain, 1);
            } else {
                enqueue(&east_station_low, new_ptrain, 1);
            }
        }
        pthread_mutex_unlock(&eastStation);
    } else if (strcmp(new_ptrain -> direction, "West") == 0){
        pthread_mutex_lock(&westStation);
        if(new_ptrain -> priority == 'H') {
            if(isEmpty(&west_station_high)) {
                west_station_high = createNode(new_ptrain, 1);
            } else {
                enqueue(&west_station_high, new_ptrain, 1);
            }
        } else {
            if(isEmpty(&west_station_low)) {
                west_station_low = createNode(new_ptrain, 1);
            } else {
                enqueue(&west_station_low, new_ptrain, 1);
            }
        }
        pthread_mutex_unlock(&westStation);
    }

    //Signal dispatcher that train has finished loading
    new_ptrain -> current_state = 2;
    if(clock_gettime(CLOCK_REALTIME, &stop) == -1) {
        perror("clock gettime");
        exit(EXIT_FAILURE);
    }
    outputTime(start, stop);
    printf("Train  %2d is ready to go %4s\n", new_ptrain -> train_no, new_ptrain -> direction);

    if(clock_gettime(CLOCK_REALTIME, &start) == -1) {
        perror("clock gettime");
        exit(EXIT_FAILURE);
    }

    //Wait for dispatcher to allow train onto main track
    pthread_cond_signal(&dispatcher);
    pthread_cond_wait(&(new_ptrain -> allowed), &mainTrack);

    if(clock_gettime(CLOCK_REALTIME, &stop) == -1) {
        perror("clock gettime");
        exit(EXIT_FAILURE);
    }
    outputTime(start, stop);
    printf("Train  %2d is ON the main track going %4s\n", new_ptrain -> train_no, new_ptrain -> direction);
    if(clock_gettime(CLOCK_REALTIME, &start) == -1) {
        perror("clock gettime");
        exit(EXIT_FAILURE);
    }

    //Run crossing time wait
    usleep(new_ptrain -> crossing_time * 100000); 
    if(clock_gettime(CLOCK_REALTIME, &stop) == -1) {
        perror("clock gettime");
        exit(EXIT_FAILURE);
    }
    outputTime(start, stop);
    printf("Train  %2d is OFF the main track after going %4s\n", new_ptrain -> train_no, new_ptrain -> direction);
    
    //Exit main track and decrement number of trains left. System exit if there
    //are no trains left to cross, otherwise signal dispatcher and detach thread
    trains_left--;
    if(trains_left < 0) exit(0);
    pthread_mutex_unlock(&mainTrack);
    pthread_cond_signal(&dispatcher);
    pthread_exit(0);
}

//Train function run at creation of dispatcher pthread
void* runDispatcher(void* count) {
    int i, last_direction = 0;
    int num_trains = *((int*)count);
    trains_left = num_trains;
    struct Train* next_ptrain;
    
    //Loop waits for all trains to signal they are ready to load
    while(trains_ready - 1 != num_trains) {
        pthread_cond_wait(&dispatcher, &mainTrack);
    }

    //Signals all trains to load
    pthread_cond_broadcast(&dispatcher);
    
    //Main running while loop. Waits until all trains have crossed main track
    while(trains_left >= 0) {
        pthread_mutex_lock(&mainTrack);
        for(i = 0; i <= num_trains; i++) {

            //Section which selects next train of highest priority to cross
            if (waiting_trains[i] -> current_state == 2) { 
                if(last_direction == 0) {
                    if(!isEmpty(&east_station_high)) {
                        next_ptrain = getHead(&east_station_high);
                        dequeue(&east_station_high);
                        last_direction = 0;
                    } else if(!isEmpty(&west_station_high)) {
                        next_ptrain = getHead(&west_station_high);
                        dequeue(&west_station_high);
                        last_direction = 1;
                    } else if(!isEmpty(&east_station_low)) {
                        next_ptrain = getHead(&east_station_low);
                        dequeue(&east_station_low);
                        last_direction = 0;
                    } else if(!isEmpty(&west_station_low)) {
                        next_ptrain = getHead(&west_station_low);
                        dequeue(&west_station_low);
                        last_direction = 1;
                    }
                } else {
                    if(!isEmpty(&west_station_high)) {
                        next_ptrain = getHead(&west_station_high);
                        dequeue(&west_station_high);
                        last_direction = 1;
                    } else if(!isEmpty(&east_station_high)) {
                        next_ptrain = getHead(&east_station_high);
                        dequeue(&east_station_high);
                        last_direction = 0;
                    } else if(!isEmpty(&west_station_low)) {
                        next_ptrain = getHead(&west_station_low);
                        dequeue(&west_station_low);
                        last_direction = 1;
                    } else if(!isEmpty(&east_station_low)) {
                        next_ptrain = getHead(&east_station_low);
                        dequeue(&east_station_low);
                        last_direction = 0;
                    }
                }

                //Signal selected train to cross and wait for signal that the
                //crossing has been completed successfully
                pthread_cond_signal(&next_ptrain -> allowed);
                pthread_cond_wait(&dispatcher, &mainTrack);
            }   
        } 
        pthread_mutex_unlock(&mainTrack);
    }
    return count;
}

//Main function
int main(int argc, char** argv) {
    FILE* file;
    int i = 0, fp;
    char* buf;
    pthread_t pthreads[100];
    pthread_t dthread;
    int train_no = 0;
    char priority;
    char char_direction;
    char* direction;
    int loading_time = 0;
    int crossing_time = 0;
    struct Train* ptrain;
    if ((file = fopen(argv[1], "r")) == NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    
    //Initialize waiting trains array to hold trains before entering queue
    waiting_trains = (struct Train**)malloc(sizeof(struct Train**) * 100);

    //Loop which reads from text file and builds each train
    while (fp != EOF) {
        fp = fscanf(file, "%c %d %d", &char_direction, &loading_time, &crossing_time);
        if(fp == EOF) break;
        train_no = i;

        if(char_direction == toupper(char_direction)) {
            priority = 'H';
        } else {
            priority = 'L';
        }

        if(tolower(char_direction) == 'w') {
            direction = malloc(sizeof(char*) * 100);
            strcpy(direction, "West");
        } else {
            direction = malloc(sizeof(char*) * 100);
            strcpy(direction, "East");
        }
        
        //Assign train data
        ptrain = (struct Train*)malloc(sizeof(struct Train*));
        ptrain -> train_no = train_no;
        ptrain -> priority = priority;
        ptrain -> direction = direction;
        ptrain -> loading_time = loading_time;
        ptrain -> crossing_time = crossing_time;        
        
        //Create train and start runPtrain function for that train
        if(pthread_create(&pthreads[i], NULL, &runPtrain, (void*)ptrain)) {
            fprintf(stderr, "Error creating pthread");
            exit(0);
        }       

        waiting_trains[i] = ptrain;
        
        i++;
        
        //Catch endline character
        buf = (char*)malloc(sizeof(char*));
        fgets(buf, sizeof(buf), file);
    }
    fclose(file);

    //Create dispatcher thread
    if(pthread_create(&dthread, NULL, &runDispatcher, (void*)&train_no)) {
        fprintf(stderr, "Error creating pthread");
        exit(0); 
    }

    //Free all memory and join all threads
    pthread_join(dthread, NULL);
    free(buf);
    for(int x = 0; x < train_no; x++) {
        pthread_join(pthreads[i], NULL);
        free(waiting_trains[i]);
    } 
    return(1);
}
