//
//  GlobalState.cpp
//  HaxClient
//
//  Created by Andrej Karadzic on 11/27/15.
//  Copyright © 2015 Andrej Karadzic. All rights reserved.
//

#include "GlobalState.hpp"
#include <vector>

bool operator<(const player &lhs, const player &rhs) {
  return (lhs.id < rhs.id);
}

extern GlobalState *GS;

/**
 * This will block until player list is received
 */
GlobalState::GlobalState(Communicator *communicator) : comm(communicator) {
  char *message = comm->receiveRaw();
  if (message[0] != 'k') {
    throw new std::runtime_error("Wrong message received.");
  }
  // "k;...."
  message += 2;

  message = parseConstants(message);

  int offset, team, id, myId = -1;
  char teamString[5], name[50];

  std::vector<player *> players[2];

  while (sscanf(message, ";%[^,],%d,%[^;]%n", teamString, &id, name, &offset) ==
         3) {
    message += offset;

    team = (teamString[0] == 'H') ? HOME : AWAY;

    if (name == comm->username) {
      myId = id;
      myTeam = team;
      initGoals();
    }

    players[team].push_back(new player);
    players[team].back()->id = id;
  }

  if (myId == -1) {
    std::cerr << "Current user does not exist on the server" << std::endl;
    exit(EXIT_FAILURE);
  }

  for (player *p : players[myTeam]) {
    if (myId != p->id)
      myPlayers[id] = *p;
    else
      myPlayer = *p;
  }

  for (player *p : players[1 - myTeam]) {
    oppPlayers[id] = *p;
  }
}

void GlobalState::initGoals() {
  myGoal.x = (myTeam == HOME) ? -constMaxX : constMaxX;
  myGoal.yMin = -constPostY;
  myGoal.yMax = constPostY;
  oppGoal.x = (myTeam == AWAY) ? -constMaxX : constMaxX;
  oppGoal.yMin = -constPostY;
  oppGoal.yMax = constPostY;
}

void GlobalState::updateState() {

  char *message = comm->receiveRaw();

  if (message[0] == 'g') {
    // "g;...."
    message += 2;
    message = parseScore(message);
    message++;
  } else if (message[0] == 'p') {
    // "p;...."
    message += 2;
  } else if (message[0] == 'k') {
    // "user reset"
    return;
  }

  message = parseBall(message);

  int id, offset;

  while (sscanf(message, ";%d,%n", &id, &offset) == 1) {
    message += offset;

    if (id == myPlayer.id)
      message = parsePlayer(message, &myPlayer);
    else {
      player *p;

      p = &myPlayers[id];
      if (!p)
        p = &oppPlayers[id];

      if (!p) {
        std::cerr << "Player not found." << std::endl;
        return;
      }

      message = parsePlayer(message, p);
    }
  }
}

char *GlobalState::parseConstants(char *message) {
  int offset;

  int k = sscanf(message, "%lf,%lf,%lf,%lf,%lf,%lf,%lf%n", &constMaxX,
                 &constMaxY, &constPostY, &constPostRadius, &constBallRadius,
                 &constPlayerRadius, &constKickerRadius, &offset);
  if (k != 7)
    std::cerr << "Error parsing: " << message << std::endl;
  return message + offset;
}
char *GlobalState::parseScore(char *message) {
  int offset;
  int k = sscanf(message, "%d:%d%n", &score[HOME], &score[AWAY], &offset);
  if (k != 2)
    std::cerr << "Error parsing: " << message << std::endl;
  return message + offset;
}

char *GlobalState::parseBall(char *message) {
  int offset;
  double x, y, vx, vy;
  int k = sscanf(message, "%lf,%lf,%lf,%lf%n", &x, &y, &vx, &vy, &offset);
  if (k != 4)
    std::cerr << "Error parsing: " << message << std::endl;
  ballPos = Point(x, y);
  ballVel = Vector(vx, vy);
  return message + offset;
}

char *GlobalState::parsePlayer(char *message, player *p) {
  int offset;
  double x, y, vx, vy;
  int k = sscanf(message, "%lf,%lf,%lf,%lf%n", &x, &y, &vx, &vy, &offset);
  if (k != 4)
    std::cerr << "Error parsing: " << message << std::endl;
  p->pos = Point(x, y);
  p->vel = Vector(vx, vy);
  return message + offset;
}

int GlobalState::myScore() { return score[myTeam]; }
int GlobalState::oppScore() { return score[1 - myTeam]; }
