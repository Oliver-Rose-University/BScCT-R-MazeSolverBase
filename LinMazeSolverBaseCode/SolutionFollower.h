#ifndef SOLUTION_H
#define SOLUTION_H

class SolutionFollower {
  private:
  State state;
  void followLine();
  void checkIfJunction();
  void turnLeft();
  void turnRight();
  void identifyJunction();
  void countNumber();
  int count = 0;


  public:
    Decisions path[50];
    int totalLength;
    void loop();
};

#endif