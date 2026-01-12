#include <Pololu3piPlus32U4.h>
#include <PololuMenu.h>

using namespace Pololu3piPlus32U4;

#include "MazeSolver.h"
#include "Shared.h"
#include "SolutionFollower.h"

void SolutionFollower::followLine() {
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

void SolutionFollower::turnLeft() {
  motors.setSpeeds(baseSpeed, baseSpeed);
  delay(250);
  motors.setSpeeds(0, 0);
 
  motors.setSpeeds(-baseSpeed, baseSpeed);
  delay(755);
  motors.setSpeeds(0, 0);
  state = LINE_FOLLOWER;


}

void SolutionFollower::turnRight() {

  motors.setSpeeds(baseSpeed, baseSpeed);
  delay(250);
  motors.setSpeeds(0, 0);

  motors.setSpeeds(baseSpeed, -baseSpeed);
  delay(755);
  motors.setSpeeds(0, 0);
  state = LINE_FOLLOWER;
}