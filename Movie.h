/*
 * Movie.h
 * Author: <Tuan Nguyen>
 * Date:   <05/31/2018>
 */

#ifndef MOVIE_H
#define MOVIE_H

#include <unordered_map>
#include "ActorNode.h"

using namespace std;

class ActorNode;

/**
 *  A movie with name, year, and weight
 */
class Movie {
public:
  string movieName;
  int year;
  int weight;
  unordered_map<string, ActorNode *> actors;
  
  Movie(string name, int yr, bool weighted) : movieName (name), year (yr) {
    if (weighted) {
      weight = 1 + 2015 - year;
    }
    
    else {
      weight = 1;
    }
  }
};

#endif