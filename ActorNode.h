/*
 * ActorNode.h
 * Author: <Tuan Nguyen>
 * Date:   <05/31/2018>
 */

#ifndef ACTORNODE_H
#define ACTORNODE_H

#include <vector>
#include <unordered_map>
#include "Movie.h"

using namespace std;

class ActorNode {
public:
  string actor;
  int distance; // distance from the start vertex
  ActorNode * pre; // previous vertex
  bool done; // is the distance finalized?
  unordered_map<string, Movie *> movies;
  unordered_map<string, ActorNode *> neighbors;
  
  /** Constructor
   *  Initialize the actor's name, distance, and prev vertex
   */
  ActorNode(string name) : actor (name), distance (0), pre (0),
                           done (false) {}
  
  ActorNode (ActorNode * node) {
    actor = node->actor;
    distance = node->distance;
    pre = node->pre;
    done = node->done;
    movies = node->movies;
    neighbors = node->neighbors;
  }
  
  void resetNodeMin() {
    distance = 99999;
    pre = 0;
    done = false;
  }
  
  void resetNodeMax() {
    distance = 0;
    pre = 0;
    done = false;
  }
  
  void getNeighbors() {
    for (auto it : movies) {
      string movie_year = it.first;
      Movie * movie = it.second;
      
      for (auto movieIt : movie->actors) {
        const string actorName = movieIt.first;
        ActorNode * actor = movieIt.second;
        neighbors.insert(pair<string, ActorNode *> (movie_year + actorName, actor));
      }
    }
  }
  
  bool operator<(const ActorNode& other) {
    if (this->distance > other.distance)
    return true;
  
    else if (this->distance == other.distance) {
      if (this->actor > other.actor)
        return true;      
    }
    
    return false;
  }
  
  bool operator>(const ActorNode& other) {
    if (this->distance < other.distance)
    return true;
  
    else if (this->distance == other.distance) {
      if (this->actor < other.actor)
        return true;      
    }
    
    return false;
  }
  
};

#endif