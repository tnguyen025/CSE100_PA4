/*
 * ActorGraph.h
 * Author: <Tuan Nguyen>
 * Date:   <05/31/2018>
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#ifndef ACTORGRAPH_H
#define ACTORGRAPH_H

#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>
#include "ActorNode.h"
#include "Movie.h"

using namespace std;


class ActorNodePtrComp {
public:
    bool operator()(ActorNode*& lhs, ActorNode*& rhs) const {
      if (lhs == rhs)
        return true;
      
      return *lhs < *rhs;
    }
};

class ActorNodePtrCompMax {
public:
    bool operator()(ActorNode*& lhs, ActorNode*& rhs) const {
      if (lhs == rhs)
        return true;
      
      return *lhs > *rhs;
    }
};


class ActorGraph {
protected:
  unordered_map<string, ActorNode *> actorsList;
  unordered_map<string, Movie *> moviesList;
  
public:
  ActorGraph(void);

  // Maybe add some more methods here

  /** You can modify this method definition as you wish
   *
   * Load the graph from a tab-delimited file of actor->movie relationships.
   *
   * in_filename - input filename
   * use_weighted_edges - if true, compute edge weights as 1 + (2015 - movie_year), otherwise all edge weights will be 1
   *
   * return true if file was loaded sucessfully, false otherwise
   */
  bool loadFromFile(const char*, bool);
  
  void buildNeighbors();
  
  bool findShortestPath (const char*, const char*);
  
  bool findWidestPath (const char*, const char*);
  
  void getShortestBetween(string, string);
  
  void getWidestBetween(string, string);
  
  pair<int, Movie *> getSmallestEdge(ActorNode*, ActorNode*);
  
  pair<int, Movie *> getWidestEdge(ActorNode*, ActorNode*);
  
  void resetActorsMin();
  
  void resetActorsMax();

  ~ActorGraph ();
};


#endif // ACTORGRAPH_H
