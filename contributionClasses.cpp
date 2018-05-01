//
//  contributionClasses.cpp
//  usersProject
//
//  Created by Julia Schell on 4/30/18.
//  Copyright © 2018 Julia Schell. All rights reserved.
//

#include <stdio.h>
#include <string>
#include <map>
using namespace std;
enum {COMMIT, ISSUE};

class userContributionHistory{
public:
    //constructor
    userContributionHistory(string username): name(username){};
    
    //update member array with new map
    bool update(int actionType, string jsonGET);
    
    int getCommitCount(string date);
    int getIssuesCount(string date);
    int getPullCount(string date);
    int getTotalCount(string date);
    
    /*
     void displayPastWeek();
     void displayMonth(int month);
     void displayYear();
     */
    
private:
    string name;
    int commits[365];
    int issuesOpened[365];
    int pullRequests[365];
};

bool userContributionHistory::update(int actionType, string jsonGET){
    switch (actionType) {
        case COMMIT:
            
            
    }
    
}
