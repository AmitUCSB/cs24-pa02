#include <iostream>
#include <map>
#include <set>
#include <string>

using namespace std;

string prefixFinder(const map<string, double>& movies, string prefix) {

    map<double, set<string>, greater<>> orderedMovies;

    for (const auto& [title, rating] : movies) {
        if (title.substr(0, prefix.size()) == prefix) {
            orderedMovies[rating].insert(title);
        }
    }

    if (orderedMovies.empty()) {
        cout << "No movies found with prefix " << prefix << endl;
        return "";
    }

    for (const auto& [rating, titles] : orderedMovies) {
        for (const auto& title : titles) {
            cout << title << ", " << rating << endl;
        }
    }
    cout << endl;

    const auto& topMovie = *orderedMovies.begin()->second.begin();
    return topMovie;
}