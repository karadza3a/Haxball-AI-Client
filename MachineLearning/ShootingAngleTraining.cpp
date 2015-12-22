//
//  ShootingAngleTraining.cpp
//  HaxClient
//
//  Created by Andrej Karadzic on 11/26/15.
//  Copyright © 2015 Andrej Karadzic. All rights reserved.
//

#include "ShootingAngleTraining.hpp"

/*
 * Poorly written, but only needed for prototyping
 */
void ShootingAngleTraining::parseDataForMatlab() {
  const int maxLines = 1000;
  std::vector<std::pair<double, double> //
              >
      points[maxLines];
  std::vector<double> params[maxLines];

  char s[10000];
  int n = -1, j = 0;
  freopen("/Users/karadza3a/Desktop/outAll.txt", "r", stdin);
  freopen("/Users/karadza3a/Desktop/data.txt", "w", stdout);
  while (std::cin >> s) {
    if (s[1] == '-') {

      double px, py, bx, by, pvx, pvy, bvx, bvy;
      sscanf(s, "--,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf", &px, &py, &bx, &by, &pvx,
             &pvy, &bvx, &bvy);
      n++;
      params[n].push_back(px);
      params[n].push_back(py);
      params[n].push_back(bx);
      params[n].push_back(by);
      params[n].push_back(pvx);
      params[n].push_back(pvy);
      params[n].push_back(bvx);
      params[n].push_back(bvy);
      j = 0;
    } else {
      double x, y;
      sscanf(s, "%lf,%lf", &x, &y);
      points[n].push_back(std::make_pair(x, y));
      j++;
    }
  }
  for (int k = 0; k <= n; k++) {

    int m = points[k].size();
    if (m > 10) {
      double px = params[k][0];
      double py = params[k][1];
      double bx = params[k][2];
      double by = params[k][3];
      double pvx = params[k][4];
      double pvy = params[k][5];
      double bvx = params[k][6];
      double bvy = params[k][7];
      double shx = bx - px, shy = by - py;

      Point p1(points[k][0].first, points[k][0].second);
      Point p2(points[k][10].first, points[k][10].second);
      Vector recordedLine1(p1, p2);

      //      Point p11(points[k][m - 8].first, points[k][m - 8].second);
      //      Point p12(points[k][m - 2].first, points[k][m - 2].second);
      //      Ray recordedLine2(p11, p12);

      // Shot vector
      Vector shv(*new Point(px, py), *new Point(bx, by));

      //      double a = atan2(shv.y(), shv.x()) * 180 / M_PI;

      // Extreme shot angles or too low or too high recording resolution
      // (dependant on shot speed) isn't giving precise intersection when
      // recreating it
      //      int x = 35;
      //      if ((a > 45 - x && a < 45 + x) || (-a > 45 - x && -a < 45 + x) ||
      //          (a > 135 - x && a < 135 + x) || (-a > 135 - x && -a < 135 +
      //          x))
      //        a++;
      //      else
      //        continue;
      //
      //      double sd = sqrt(squared_distance(p1, p2));
      //      if (sd < 2.1 || sd > 2.9)
      //        continue;
      // end filtering
      //
      //      Point p3(3, 20 * signof(shy));
      //      Point p4(4, 20 * signof(shy));
      //      Line outline(p3, p4);
      //
      //      Point pg3(43 * signof(shx), 3);
      //      Point pg4(43 * signof(shx), 4);
      //      Line goalline(pg3, pg4);

      //      Point p5(bx, by);
      //      Point p6(bx + shx, by + shy);
      //      Ray idealRay(p5, p6);
      //
      //      auto result = intersection(idealRay, outline);
      //      Point idealInters = boost::get<Point>(result.get());

      //      auto result = intersection(recordedLine1, outline);
      //      Point realInters = boost::get<Point>(result.get());
      //
      //      result = intersection(recordedLine2, goalline);
      //      Point realGoalInters = boost::get<Point>(result.get());

      std::cout << shx << ", "     //
                << shy << ", "     //
                << bvx << ", "     //
                << bvy << ",    "; //

      std::cout << atan2(recordedLine1.y(), recordedLine1.x()) << std::endl;
    }
  }
}