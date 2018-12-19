/*
 * pathfinder.cpp
 * Author: <Tuan Nguyen>
 * Date:   <05/31/2018>
 * Main driver for the program to find the shortest path between actors
 */
 
 #include <iostream>
 #include <fstream>
 #include "ActorGraph.h"
 
 using namespace std;
 
 int main(int argc, const char * * argv) {
   bool useWeightedEdge = false;
   
   // if number of command line args is not correct
   if (argc != 5) {
     cerr << argv[0] << " called with incorrect arguments.\n"
          << "Usage: " << argv[0]
          << " movie_cast_tsv_file u/w pairs_tsv_file output_paths_tsv_file\n";
     return 0;
   }
   
   // if command line arg specifies weighted edge
   if (*argv[2] == 'w')
     useWeightedEdge = true;
   
   // create an instance of the ActorGraph and read from input file to create
   // graph
   ActorGraph myGraph;
   myGraph.loadFromFile(argv[1], useWeightedEdge);
   
   myGraph.findShortestPath(argv[3], argv[4]);
 }