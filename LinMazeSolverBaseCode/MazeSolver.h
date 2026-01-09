#ifndef MAZE_H
#define MAZE_H

// enum creates a new type which can be used like any type
enum State {
  LINE_FOLLOWER,
  JUNCTION,
  TURN_LEFT,
  TURN_RIGHT,
  U_TURN,
  FINISHED,
  FAKE_END
};

class MazeSolver {
  private:
  State state; // value of type state

  void followLine();

  void checkIfJunction();

  void checkIfDeadEnd();

  void identifyJunction();

  void turnLeft();

  void turnRight();

  void uTurn();
  
  public:

    // constructor
    MazeSolver();

    // function to be called at every main loop
    void loop();

};

#endif

