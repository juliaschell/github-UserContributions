//
//  workWithStrings.hpp
//  usersProject
//
//  Created by Julia Schell on 4/30/18.
//  Copyright © 2018 Julia Schell. All rights reserved.
//

#ifndef workWithStrings_hpp
#define workWithStrings_hpp

#include <stdio.h>
#include <string>
#include <map>
#include <set>
#include "json/json.h"
#include <chrono>
using namespace std;
using namespace chrono;

#endif /* workWithStrings_hpp */


//make github search, return string results
string getSearch(string username, string date);
size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data);

//takes in string from search, parses, updates date:number actions map, return parsed Value object
set<int> commitInterpret( string searchResult, string todaysDate);
string getCurrentTime();

