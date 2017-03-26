
// How to create arrays of objects other than primitive variables? 
// For example: 
// 
// grid[2] = a object of class Grid (instead of double or float)? 
// 
// This is not possible with the C++ learned so far. It is possible with templates
//

/**
std::vector<double> a;    //dynamic array of doubles
std::vector<Grid> grids;    // array of grids

 Grid g1;
 Grid g2;
 grids.push_back(g1);
 grids.push_back(g2);

 grids[0] is of type Grid.
 &grids[1] - &grids[0]  return 1 (as usual)
 sizeof(grids[0]) = sizeof(g1) = sizeof(Grid)
*/
