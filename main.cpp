//
//  main.cpp
//  usersProject
//
//  Created by Julia Schell on 4/29/18.
//  Copyright © 2018 Julia Schell. All rights reserved.
//

#include "json/json.h"
#include "userContributions.h"
#include <iostream>
#include <curl/curl.h>
#include <string>
using namespace std;

class userContributionHistory;

int main(){
    
    string username;
    cout << "Please enter the username you'd like to see contributions for (case sensitive!)" << endl;
    cin >> username;
    
    userContributionHistory thisUser(username);
    addCommits(&thisUser);
    addIssues(&thisUser);
    
    int contributionArray[366];
    printContributions(&thisUser, contributionArray);
    cout << "all done!" << endl;
}



