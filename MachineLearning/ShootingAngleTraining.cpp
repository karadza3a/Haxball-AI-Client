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
      //      double pvx = params[k][4];
      //      double pvy = params[k][5];
      double bvx = params[k][6];
      double bvy = params[k][7];
      double shx = bx - px, shy = by - py;

      Point p11(points[k][0].first, points[k][0].second);
      Point p12(points[k][10].first, points[k][10].second);
      Vector recordedLine1(p11, p12);

      Point p21(points[k][m - 11].first, points[k][m - 11].second);
      Point p22(points[k][m - 3].first, points[k][m - 3].second);
      Vector recordedLine2(p21, p22);

      Line line1(p11, p12);
      Line line2(p21, p22);

      auto result = intersection(line1, line2);
      Point inters = boost::get<Point>(result.get());

      std::cerr << inters.y() << std::endl;

      std::cout << shx << ", "     //
                << shy << ", "     //
                << bvx << ", "     //
                << bvy << ",    "; //

      std::cout << atan2(recordedLine1.y(), recordedLine1.x()) << ",    "
                << atan2(recordedLine2.y(), recordedLine2.x()) << std::endl;
    }
  }
}