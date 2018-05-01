//
//  main.cpp
//  usersProject
//
//  Created by Julia Schell on 4/29/18.
//  Copyright © 2018 Julia Schell. All rights reserved.
//

#include "json/json.h"
#include "workWithStrings.h"
#include <iostream>
#include <curl/curl.h>
#include <string>
using namespace std;


int main(){
    auto curl = curl_easy_init();
    if (curl) {
        
        string username = "Dogild";
        string commitURL = "https://api.github.com/search/commits?q=author:"+username+"&sort=created";
        string commitHeader = "Accept: application/vnd.github.cloak-preview";
        string issueURL = "https://api.github.com/search/issues?q=author:"+username+"&sort=created";
        
        //GET commit data from github
        string commitSearchString = getSearch(commitURL, commitHeader);
        if (commitSearchString.empty()){
            cout << "Error: get commit search results" << endl;
        }
        
        //GET issue data from github
        string issueSearchString = getSearch(issueURL,"");
        if (commitSearchString.empty()){
            cout << "Error: get commit search results" << endl;
        }
        
        //Analyze commit data
        map<string, string> commitDateMap;
        if (!commitInterpret(commitSearchString, commitDateMap)){
            // what to do if not successful
        }
        
        
        
        
        
    }
}


