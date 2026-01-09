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

enum Decisions {
  RIGHT,
  LEFT,
  BACK,
  FORWARD
};
// Right = 0, Left = 1, Back = 2, Forwards = 3




class MazeSolver {
  private:
  State state; // value of type state

  
  int count = 0;
  int path[256];
  int pathText[256];

  void followLine();

  void checkIfJunction();

  void checkIfDeadEnd();

  void identifyJunction();

  void turnLeft();

  void turnRight();

  void uTurn();

  void countNumber();

  void convertNumber();
  
  public:

    // constructor
    MazeSolver();

    // function to be called at every main loop
    void loop();

};

#endif

