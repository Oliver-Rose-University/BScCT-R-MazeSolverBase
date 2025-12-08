
#include <Pololu3piPlus32U4.h>
#include <PololuMenu.h>

using namespace Pololu3piPlus32U4;

#include "MazeSolver.h"
#include "Shared.h"
MazeSolver::MazeSolver() {
  state = LINE_FOLLOWER;
}

void MazeSolver::followLine() {
  // get position & error
  int16_t position = lineSensors.readLineBlack(lineSensorValues);
  int16_t error = position - 2000;

  // calculate speed difference with PID formula
  int16_t speedDifference = error * (int32_t)proportional / 256 + (error - lastError) * (int32_t)derivative / 256;

  // store error
  lastError = error;

  // get new speed & constrain
  int16_t leftSpeed = (int16_t)baseSpeed + speedDifference;
  int16_t rightSpeed = (int16_t)baseSpeed - speedDifference;
  leftSpeed = constrain(leftSpeed, minSpeed, (int16_t)maxSpeed);
  rightSpeed = constrain(rightSpeed, minSpeed, (int16_t)maxSpeed);

  // update motor speed
  motors.setSpeeds(leftSpeed, rightSpeed);
}

void MazeSolver::loop() {
  if (state == LINE_FOLLOWER) {
    followLine();
    checkIfJunction();
    display.clear();
    display.print('A');
  }

  if (state == JUNCTION) {
    // call junciton identifier function
    identifyJunction();
    display.clear();
    display.print('J');
  }
  if (state == TURN_LEFT) {
    // call left turn function
    motors.setSpeeds(0, 0);
    display.clear();
    display.print('L');
    turnLeft();
  }
  if (state == U_TURN) {
    // call u turn function
    motors.setSpeeds(0, 0);
    display.clear();
    display.print('U');
    uTurn();
  }
  if (state == FINISHED) {
    motors.setSpeeds(0, 0);
    display.clear();
    display.print('F');
    return;
  }
}

void MazeSolver::checkIfJunction() {
  if (lineSensorValues[0] > 750 & lineSensorValues[2] > 750){
    state = JUNCTION;
  }
  if (lineSensorValues[2] > 750 & lineSensorValues[3] > 750){
    state = JUNCTION;
  }
  if (lineSensorValues[0] > 750 & lineSensorValues[2] > 750 & lineSensorValues[4] > 750){
    state = JUNCTION;
  }
   if (lineSensorValues[0] > 750 & lineSensorValues[4] > 750){
    state = JUNCTION;
  }
}

void MazeSolver::identifyJunction(){
  motors.setSpeeds(baseSpeed, baseSpeed);
  delay(50);
  lineSensors.readLineBlack(lineSensorValues);
  motors.setSpeeds(0, 0);
  if (lineSensorValues[0] > 750 & lineSensorValues[1] > 750 & lineSensorValues[2] > 750 & lineSensorValues[3] > 750 & lineSensorValues[4] > 750){
    state = FINISHED;
  }else if (lineSensorValues[0] > 750){
    state = TURN_LEFT;
  }else if (lineSensorValues[2] > 750){
    state = LINE_FOLLOWER;
  }else if (lineSensorValues[0] > 750 & lineSensorValues[2] > 750 & lineSensorValues[4] > 750){
    state = TURN_LEFT;
  }else if (lineSensorValues[0] > 750 & lineSensorValues[4] > 750){
    state = TURN_LEFT;
  }else{
    checkIfDeadEnd();
  }
}


void MazeSolver::turnLeft(){
  motors.setSpeeds(100,100);
  delay(100);
  motors.setSpeeds(-200, 200);
  delay(500);
  motors.setSpeeds(200,200);
  delay(100);

  motors.setSpeeds(0, 0);
  state = LINE_FOLLOWER;
}

void MazeSolver::uTurn(){
  while (lineSensorValues[2] < 750){
    lineSensors.readLineBlack(lineSensorValues);
    motors.setSpeeds(200, -200);
  }
  state = LINE_FOLLOWER;
}

void MazeSolver::checkIfDeadEnd(){
  if (lineSensorValues[0] < 50 & lineSensorValues[1] < 50 & lineSensorValues[2] < 50 & lineSensorValues[3] < 50 & lineSensorValues[4] < 50){
    state = U_TURN;
    motors.setSpeeds(0, 0);
  }
}