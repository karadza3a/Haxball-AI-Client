//
//  main.cpp
//  HaxClient
//
//  Created by Andrej Karadzic on 11/25/15.
//  Copyright © 2015 Andrej Karadzic. All rights reserved.
//

#include "Basic/BasicBehaviour.hpp"
#include "Communicator.hpp"
#include "GlobalState.hpp"
#include "MachineLearning/ShootingAngleTraining.hpp"
#include <cmath>

int main(int argc, char *argv[]) {

  srand((unsigned int)time(0));
  string username = "Bot  ";
  username[3] = 'A' + rand() % 26;
  username[4] = '0' + rand() % 10;

  Communicator *comm = new Communicator("localhost", 16000, 15000, username);
  comm->sendlogin();
  GlobalState GS(comm);

  BasicBehaviour BB(&GS);
  BB.start();

  //  ShootingAngleTraining SAT;
  //  SAT.parseDataForMatlab();

  return 0;
}
