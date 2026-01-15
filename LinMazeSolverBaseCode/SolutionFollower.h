#ifndef SOLUTION_H
#define SOLUTION_H
// This simply allows SolutionFollower to access the following functions, making use of the variable "count" additionally. It ensures that the prior Path is allowed to be accessed in the form of "Decisions".
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
