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

void MazeSolver::checkIfJunction() {
  lineSensors.readLineBlack(lineSensorValues);

  bool junction = false;

  if (lineSensorValues[0] > 950) junction = true;  // detect a line to the left
  if (lineSensorValues[1] > 950) junction = true;  // detect a line to the left
  if (lineSensorValues[3] > 950) junction = true;  // detect a line to the right
  if (lineSensorValues[4] > 950) junction = true;  // detect a line to the right
  // any other case contains one of these types

  if (junction) {
    state = JUNCTION;
    motors.setSpeeds(0, 0);
  }
}

void MazeSolver::checkIfDeadEnd() {
  lineSensors.readLineBlack(lineSensorValues);
  if (lineSensorValues[2] < 500) state = U_TURN;
}

void MazeSolver::identifyJunction() {

  // display.clear();

  delay(500);



  // move forward to identify other junctions
  motors.setSpeeds(baseSpeed, baseSpeed);
  delay(250);
  motors.setSpeeds(0, 0);
  lineSensors.readLineBlack(lineSensorValues);



  // if can sense everywhere -> FINISHED
  if (lineSensorValues[0] > 950 && lineSensorValues[1] > 950 && lineSensorValues[2] > 950 && lineSensorValues[3] > 950 && lineSensorValues[4] > 950) {
    state = FINISHED;
    return;
  }


  // if there's a left take it
  if (lineSensorValues[0] > 750) {
    state = TURN_LEFT;
    return;
  }

  if (lineSensorValues[2] > 750) {
    motors.setSpeeds(baseSpeed, baseSpeed);
    delay(100);

    state = LINE_FOLLOWER;
      path[count] = FORWARD;
      count++;
      countNumber();
    return;
  }

  // if there's a left take it
  if (lineSensorValues[4] > 750) {
    state = TURN_RIGHT;
    return;
  }


  // any other case -> keep going
  state = LINE_FOLLOWER;
}



bool first = true;

void MazeSolver::countNumber() {
  convertNumber();
  display.clear();
  display.gotoXY(0, 0);

  for (int i = 0; i < count; i++){
    if (i == 8){
      display.gotoXY(0,1);}
    display.print(path[i]);
  }
}



void MazeSolver::convertNumber(){
  if (path(count) == 0):


}



void MazeSolver::turnLeft() {

  motors.setSpeeds(baseSpeed, baseSpeed);
  delay(250);
  motors.setSpeeds(0, 0);

  motors.setSpeeds(-baseSpeed, baseSpeed);
  delay(755);
  motors.setSpeeds(0, 0);
  state = LINE_FOLLOWER;

  path[count] = LEFT;
  countNumber();
  count++;
}

void MazeSolver::turnRight() {

  motors.setSpeeds(baseSpeed, baseSpeed);
  delay(250);
  motors.setSpeeds(0, 0);

  motors.setSpeeds(baseSpeed, -baseSpeed);
  delay(755);
  motors.setSpeeds(0, 0);
  state = LINE_FOLLOWER;

  path[count] = RIGHT;
  count++;
  countNumber();
}

void MazeSolver::uTurn() {
  motors.setSpeeds(-baseSpeed, baseSpeed);
  delay(1600);
  motors.setSpeeds(0, 0);
  state = LINE_FOLLOWER;
  path[count] = BACK;
  count++;
  countNumber();
}

void MazeSolver::loop() {

  countNumber();
  // display.clear();
  // display.gotoXY(0, 0);
  //display.print(state);

  if (state == LINE_FOLLOWER) {
    followLine();
    //check if junction there's a junction and change state otherwise
    checkIfJunction();
    checkIfDeadEnd();

  }

  if (state == JUNCTION) {
    identifyJunction();
  }

  if (state == TURN_LEFT) {
    turnLeft();
  }

  if (state == TURN_RIGHT) {
    turnRight();
  }
  if (state == U_TURN) {
    uTurn();
  }
  if (state == FINISHED) {
    motors.setSpeeds(0, 0);
    return;
  }

  if (state == FAKE_END) {
    // display.clear();

    while (!buttonB.getSingleDebouncedPress()) {
      uint16_t position = lineSensors.readLineBlack(lineSensorValues);

      display.gotoXY(0, 0);
      display.print(position);
      display.print("    ");
      display.gotoXY(0, 1);
      for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        uint8_t barHeight = map(lineSensorValues[i], 0, 1000, 0, 8);

        if (barHeight > 8) { barHeight = 8; }
        const char barChars[] = { ' ', 0, 1, 2, 3, 4, 5, 6, (char)255 };
        display.print(barChars[barHeight]);
      }

      delay(50);
    }
  }
}
