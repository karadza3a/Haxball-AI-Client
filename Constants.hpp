//
//  Constants.hpp
//  HaxClient
//
//  Created by Andrej Karadzic on 11/29/15.
//  Copyright © 2015 Andrej Karadzic. All rights reserved.
//

#ifndef Constants_h
#define Constants_h

#define KEY_UP 1
#define KEY_RIGHT 2
#define KEY_DOWN 4
#define KEY_LEFT 8
#define KEY_SHOOT 16

#define HOME 0
#define AWAY 1

#include <CGAL/Simple_cartesian.h>
#include <iostream>
#include <cstdlib>
#include <map>

using std::string;

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point;
typedef Kernel::Vector_2 Vector;
typedef Kernel::Segment_2 Segment;
typedef Kernel::Line_2 Line;
typedef Kernel::Ray_2 Ray;
typedef Kernel::Circle_2 Circle;

struct player {
  int id;
  Point pos;
  Vector vel;

  bool const operator==(const player &that) { return (id == that.id); }
};

struct goal {
  double x, yMin, yMax;
};

#endif /* Constants_h */
