// Winter'24
// Instructor: Diba Mirza
// Student name: 
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <cstring>
#include <algorithm>
#include <limits.h>
#include <iomanip>
#include <set>
#include <queue>
#include <sstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

#include "utilities.h"
#include "movies.h"

bool parseLine(string &line, string &movieName, double &movieRating);

int main(int argc, char** argv){
    if (argc < 2){
        cerr << "Not enough arguments provided (need at least 1 argument)." << endl;
        cerr << "Usage: " << argv[ 0 ] << " moviesFilename prefixFilename " << endl;
        exit(1);
    }

    ifstream movieFile (argv[1]);
 
    if (movieFile.fail()){
        cerr << "Could not open file " << argv[1];
        exit(1);
    }
  
    // Create an object of a STL data-structure to store all the movies
    map<string, double> movies;
    string line, movieName;
    double movieRating;
    // Read each file and store the name and rating
    while (getline (movieFile, line) && parseLine(line, movieName, movieRating)){
        movies[movieName] = movieRating;
    }

    movieFile.close();

    if (argc == 2){
        for (const auto& [name, rating] : movies){
            cout << name << ", " << rating << endl;
        }
        return 0;
    }

    ifstream prefixFile (argv[2]);

    if (prefixFile.fail()) {
        cerr << "Could not open file " << argv[2];
        exit(1);
    }

    vector<string> prefixes;
    while (getline (prefixFile, line)) {
        if (!line.empty()) {
            prefixes.push_back(line);
        }
    }

    auto start = high_resolution_clock::now();

    vector<string> topMovies;
    for (int i = 0; i < prefixes.size(); i++) {
        topMovies.push_back(prefixFinder(movies, prefixes.at(i)));
    }
    

    for (int i = 0; i < topMovies.size(); i++) {
        if(topMovies.at(i) != ""){
            cout << "Best movie with prefix " << prefixes.at(i) << " is: " 
            << topMovies.at(i) << " with rating " << std::fixed << std::setprecision(1) 
            << movies[topMovies.at(i)] << endl;
        }
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Prefix search time: " << duration.count() << " ms" << endl;

    return 0;
}

/* 
Time Complexity Analysis:

n = total number of movies in the dataset
m = number of prefixes provided
k = maximum number of movies matching a given prefix
l = maximum length of a movie name

prefixFinder:
Iterates over all movies and checks if the prefix matches: O(n * l)

Each matching movie is inserted into a map<double, set<string>, greater<>>:
Each insert = O(log k)
Worst case is all movies have different ratings: k
Total for k movies = O(k * log k)

Total for all m prefixes = **O(m(n * l + k * log k))**

Final Time Complexity:  
O(m*n*l + m*k*log k))

Runtime Analysis:

input_20_random.csv = 124 ms
input_100_random.csv, = 149 ms
input_1000_random.csv = 345 ms
input_76920_random.csv = 45007 ms

Space Complexity Analysis:

In prefixFinder, the worst case is every movie matches the prefix, 
so the map has up to n keys and each key holds up to n titles. O(n)
Copying the titles into the set is O(l)
Total: O(n*l) to store all titles

Tradeoffs between time/space complexity:

I designed my algorithm for low time complexity. Our algorithm stores movie titles into a sorted
map and then we call prefixFinder to find the movies that match the given prefix. Then those movies
are put into a map sorted by rating and then we print the movies out. Although, we designed 
our program for a low time complexity, we also ended up with a low space complexity of O(n*l).
This is because the maps we create are the only things affecting space complexity and they
are of maximum size n (total number of movies).

*/

bool parseLine(string &line, string &movieName, double &movieRating) {
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex+1));
    if (movieName[0] == '\"') {
        movieName = movieName.substr(1, movieName.length() - 2);
    }
    return true;
}


