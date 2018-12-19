/*
 * ActorGraph.cpp
 * Author: <Tuan Nguyen>
 * Date:   <05/31/2018>
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
#include "ActorGraph.h"

using namespace std;

ActorGraph::ActorGraph(void) {}

bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {
    // Initialize the file stream
    ifstream infile(in_filename);
    
    cerr << "Reading " << in_filename << " ...\n";

    bool have_header = false;

    // keep reading lines until the end of file is reached
    while (infile) {
        string s;

        // get the next line
        if (!getline( infile, s )) break;

        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss( s );
        vector <string> record;

        while (ss) {
            string next;

            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }

        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }

        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[2]);

        // we have an actor/movie relationship, now what?
        ActorNode * this_actor; // a pointer to the current actor
        Movie * this_movie; // a pointer to the current movie
        
        // if actor was not found before
        if (actorsList.find(actor_name) == actorsList.end()) {
          this_actor = new ActorNode(actor_name);
          actorsList.insert(pair<string, ActorNode *>
                        (actor_name, this_actor));
        }
      
        // otherwise get the pointer to this actor from the hash map
        else {
          this_actor = actorsList.find(actor_name)->second;
        }
        
        // if movie was not found before
        if (moviesList.find(movie_title + record[2]) == moviesList.end()) {
          this_movie = new Movie(movie_title, movie_year, use_weighted_edges);
          moviesList.insert(pair<string, Movie *>
                            (movie_title + record[2], this_movie));               
        }

        // otherwise get the pointer to this actor from the hash map
        else {
          this_movie = moviesList.find(movie_title+record[2])->second;
        }
        
        // add movie to the movie list of this actor
        this_actor->movies.insert(pair<string, Movie *>
                                  (movie_title + record[2], this_movie));                                 
        
        // add actor to the actor list of this movie
        this_movie->actors.insert(pair<string, ActorNode *> 
                                  (actor_name, this_actor));                       
    }

    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    
    // build the neighbors for all actors
    if (!actorsList.empty())
      buildNeighbors();
    
    
    
    
    for (auto it : moviesList) {
      cout << it.first << " " << it.second->year << endl;
    }
    
    infile.close();
    return true;
}

void ActorGraph::buildNeighbors() {
  for (auto it : actorsList) {
    ActorNode * this_actor = it.second;
    
    this_actor->getNeighbors();
  }
}

bool ActorGraph::findShortestPath (const char* in_filename, const char* out_filename) {
  // initialize instances of input and output streams
  ifstream in(in_filename);
  ofstream out(out_filename);
  bool header = false; // is there a header in input file?
  
  // write the header
    out << "(actor)--[movie#@year]-->(actor)--..." << endl;

  while (in) {
    string lineFromFile;
    
    // get the next line
    if (!getline(in, lineFromFile))
      break;
    
    // skip the header
    if (!header) {
      header = true;
      continue;
    }
    
    istringstream ss( lineFromFile );
    vector <string> record;
    
    while (ss) {
      string next;

      // get the next string before hitting a tab character and put it in 'next'
      if (!getline( ss, next, '\t' ))
        break;

      record.push_back(next);
    }

    if (record.size() != 2) {
      // we should have exactly 2 columns
      continue;
    }
    
    string actor1(record[0]);
    string actor2(record[1]);
    
    // reset all the actor nodes' dist and prev and done fields
    resetActorsMin();
    
    // set actor2's distance to 0
    if (actorsList.find(actor2) != actorsList.end())
      actorsList.find(actor2)->second->distance = 0;
    
    // get the shortest path between 2 actors
    getShortestBetween(actor2, actor1);

    ActorNode* actor1Node = actorsList.find(actor1)->second;
    ActorNode* actor2Node = actorsList.find(actor2)->second;
    
    ActorNode* curr = actor1Node;
    
    while(1) {
      out << '(' << curr->actor << ")";
      
      if (curr == actor2Node)
        break;
      
      Movie * movie;
      
      movie = getSmallestEdge(curr, curr->pre).second;
      
      out << "--[" << movie->movieName << "#@" << movie->year << "]-->";

      curr = curr->pre;
    }
    
    out << endl;
  }
  
  if (!in.eof()) {
    cerr << "Failed to read " << in_filename << "!\n";
    return false;
  }

  in.close();
  return true;
}

bool ActorGraph::findWidestPath (const char* in_filename, const char* out_filename) {
  // initialize instances of input and output streams
  ifstream in(in_filename);
  ofstream out(out_filename);
  bool header = false; // is there a header in input file?
  
  // write the header
    out << "Actor1\t" << "Actor2\t" << "Year" << endl;

  while (in) {
    string lineFromFile;
    
    // get the next line
    if (!getline(in, lineFromFile))
      break;
    
    // skip the header
    if (!header) {
      header = true;
      continue;
    }
    
    istringstream ss( lineFromFile );
    vector <string> record;
    
    while (ss) {
      string next;

      // get the next string before hitting a tab character and put it in 'next'
      if (!getline( ss, next, '\t' ))
        break;

      record.push_back(next);
    }

    if (record.size() != 2) {
      // we should have exactly 2 columns
      continue;
    }
    
    string actor1(record[0]);
    string actor2(record[1]);
    
    // reset all the actor nodes' dist and prev and done fields
    resetActorsMax();
    
    // set actor2's distance to 99999
    if (actorsList.find(actor2) != actorsList.end())
      actorsList.find(actor2)->second->distance = 99999;
    
    // get the wides path between 2 actors
    getWidestBetween(actor2, actor1);
    
    ActorNode* actor1Node = actorsList.find(actor1)->second;
    ActorNode* actor2Node = actorsList.find(actor2)->second;
    
    ActorNode* curr = actor1Node;
    
    int minWeight = 99999;
    
    while (curr != actor2Node) {
      if (curr->distance < minWeight)
        minWeight = curr->distance;
      
      curr = curr->pre;
    }
    
    out << actor1 << "\t" << actor2 << "\t" << (1 + 2015 - minWeight) << endl;
  }
  
  if (!in.eof()) {
    cerr << "Failed to read " << in_filename << "!\n";
    return false;
  }

  in.close();
  return true;
}

void ActorGraph::getShortestBetween(string actor1, string actor2) {
  priority_queue<ActorNode *, vector<ActorNode *>, ActorNodePtrComp> priQueue;
   
  // enqueue all actors
  for (auto it : actorsList) {
    ActorNode * this_actor = it.second;
    
    priQueue.push(this_actor);
  }
  
  while (!priQueue.empty()) {
    ActorNode * node = priQueue.top();
    priQueue.pop();
    
    // if this node is not done
    if (!node->done) {
      node->done = true;
      
      for (auto it : node->neighbors) {
        ActorNode * neighbor = it.second;
        
        if (!neighbor->done) {
          int distNeighbortoNode = node->distance
                                   + getSmallestEdge(node, neighbor).first;
          
          // if new distance is smaller
          if (distNeighbortoNode < neighbor->distance) {
            // change its fields
            neighbor->pre = node;
            neighbor->distance = distNeighbortoNode;

            priQueue.push(neighbor);
          }
        }
      }
    }
    
    if (node->actor == actor2)
      return;
  }
}

void ActorGraph::getWidestBetween(string actor1, string actor2) {
  priority_queue<ActorNode *, vector<ActorNode *>, ActorNodePtrCompMax> priQueue;
   
  // enqueue all actors
  for (auto it : actorsList) {
    ActorNode * this_actor = it.second;
    
    priQueue.push(this_actor);
  }
  
  while (!priQueue.empty()) {
    ActorNode * node = priQueue.top();
    priQueue.pop();
    
    // if this node is not done
    if (!node->done) {
      node->done = true;
      
      for (auto it : node->neighbors) {
        ActorNode * neighbor = it.second;
        
        if (!neighbor->done) {
          int distNeighbortoNode = getWidestEdge(node, neighbor).first;
          
          // if new width is smaller
          if (distNeighbortoNode > neighbor->distance) {
            // change its fields
            neighbor->pre = node;
            neighbor->distance = distNeighbortoNode;

            priQueue.push(neighbor);
          }
        }
      }
    }
    
    if (node->actor == actor2)
      return;
  }
}

pair<int, Movie *> ActorGraph::getSmallestEdge(ActorNode* actor1, ActorNode* actor2) {
  int result = 0;
  Movie * movieReturn;

  for (auto actor1Movies : actor1->movies) {
    Movie * movie = actor1Movies.second;
    
    // if found actor2 in movie's list
    if (movie->actors.find(actor2->actor) != movie->actors.end()) {
      if (result == 0 || result > movie->weight) {
        result = movie->weight;
        movieReturn = movie;
      }
    }
  }
  
  return pair<int, Movie *> (result, movieReturn);
}

pair<int, Movie *> ActorGraph::getWidestEdge(ActorNode* actor1, ActorNode* actor2) {
  int result = 0;
  Movie * movieReturn;

  for (auto actor1Movies : actor1->movies) {
    Movie * movie = actor1Movies.second;
    
    // if found actor2 in movie's list
    if (movie->actors.find(actor2->actor) != movie->actors.end()) {
      if (result == 0 || result < movie->weight) {
        result = movie->weight;
        movieReturn = movie;
      }
    }
  }
  
  return pair<int, Movie *> (result, movieReturn);
}

void ActorGraph::resetActorsMin() {
  for (auto it : actorsList) {
    ActorNode * this_actor = it.second;
    
    this_actor->resetNodeMin();
  }
}

void ActorGraph::resetActorsMax() {
  for (auto it : actorsList) {
    ActorNode * this_actor = it.second;
    
    this_actor->resetNodeMax();
  }
}

ActorGraph::~ActorGraph () {
  while (!actorsList.empty()) {
    unordered_map<string, ActorNode *>::const_iterator it = actorsList.begin();
    
    string actor_name = it->first;
    ActorNode * this_actor = it->second;
    
    if (this_actor) {
      delete this_actor;
      this_actor = 0;
    }
    
    actorsList.erase(actor_name); 
  }
  
  while (!moviesList.empty()) {
    unordered_map<string, Movie *>::const_iterator it = moviesList.begin();
    
    string movie_year = it->first;
    Movie * this_movie = it->second;
    
    if (this_movie) {
      delete this_movie;
      this_movie = 0;
    }
    
    moviesList.erase(movie_year);
  }
}