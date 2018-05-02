//
//  contributionClass.h
//  usersProject
//
//  Created by Julia Schell on 5/1/18.
//  Copyright © 2018 Julia Schell. All rights reserved.
//

#ifndef contributionClass_h
#define contributionClass_h

#include <stdio.h>
#include <string>
#include <map>
#include "userContributions.h"
enum {COMMIT, ISSUE};

using namespace std;

tm getCurrentTime();
multimap<int,tm> commitInterpret( string searchResult, tm curr );
multimap<int,tm> issueInterpret( string searchResult, tm curr );

class userContributionHistory{
public:
    //constructor
    userContributionHistory(string username): name(username){};
    
    //update member array with new map
    bool update(int actionType, string jsonGET);
    
    string getUsername(){
        return name;}
    int getCommitCount(tm date){
        return commits.count(date.tm_yday);}
    int getIssuesCount(tm date){
        return issues.count(date.tm_yday);}
    int getTotalCount(tm date){
        return getCommitCount(date)+getIssuesCount(date);}
    
    string getDateUTC(int yday);
    /*
     void displayPastWeek();
     void displayMonth(int month);
     void displayYear();
     */
    
private:
    string name;
    multimap<int,tm> commits;
    multimap<int,tm> issues;
};

bool userContributionHistory::update(int actionType, string jsonGET){
    tm tmCurr = getCurrentTime();
    if (jsonGET=="")
        return false;
    
    switch (actionType) {
        case COMMIT:
            commits = commitInterpret(jsonGET, tmCurr);
            return true;
        case ISSUE:
            issues = issueInterpret(jsonGET, tmCurr);
            return true;
        default:
            return false;
    }
}

/*
string userContributionHistory::getDateUTC(int yday){
    tm d;
    if (commits.find(yday)!=commits.end())
        d = commits.find(yday)->second;
    else if (issues.find(yday)!=issues.end())
        d = issues.find(yday)->second;
    else{
        return "";}
    
    string MM = to_string(d.tm_mon+1); string DD = to_string(d.tm_mday);
    string dateUTC = to_string(d.tm_mon+1) + "/" + to_string(d.tm_mday);
    return dateUTC;
}*/

#endif /* contributionClass_h */
