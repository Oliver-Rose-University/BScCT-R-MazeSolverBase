#ifndef MAZE_H
#define MAZE_H

// enum creates a new type which can be used like any type
enum State {
  LINE_FOLLOWER,
  JUNCTION,
  TURN_LEFT,
  U_TURN,
  FINISHED
};

class MazeSolver {
  private:
  State state; // value of type state

  void followLine();
  
  public:

    // constructor
    MazeSolver();

    // function to be called at every main loop
    void loop();

};

#endif

