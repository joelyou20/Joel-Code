README.txt

# Author: Joel Cribby
# Date: 2018-03-07

# Program Description for CSC 360 Assignment 2
## Train Crossing Problem ##

# Description: In this program, the student (myself) was requested to design a
 program that would build trains (pthreads) from stdin and simulate a train
 crossing scenerio where each train has a loading time and a crossing time.

 The program focused on utilizing pthreads and prioritization

# Successes: I managed to successfully build the trains and then load them
 into the correct station. They then were able to cross to the opposite side
 of the track and exit.

# Failures: Prioritization never was successfully implemented.

# Differences from p1a design outline:

 1) I have used 4 priority queue data structures as opposed to 2. I did this
    to implement a priority queue for west low, west high, east low, and east
    high priority queues.

 2) I did not mention the fact that all trains will load at same time in my
    algorithm sketch. That decision was made later after p1a.

 3) In my algorithm sketch (question 8 of p1a) I mentioned that the dispatcher
    would be used to load the trains before I even created the dispatcher.
    (Step 5: train is loaded by dispatcher. Step 7: dispatcher is created)

 4) In step 10 of my algorithm sketch (question 8 of p1a) I mention that the
    trains would be loaded based on a FCFS system which after testing and
    research, I had decided to implement a more stringent system of 
    prioritization where several considerations for importance would be
    taken into account.
