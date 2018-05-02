//
//  workWithStrings.hpp
//  usersProject
//
//  Created by Julia Schell on 4/30/18.
//  Copyright © 2018 Julia Schell. All rights reserved.
//

#ifndef userContributions_h
#define userContributions_h

#include <iostream>
#include <stdio.h>
#include <string>
#include <map>
#include <set>
#include "json/json.h"
#include "contributionClass.h"
#include <chrono>
#include <curl/curl.h>
using namespace std;
using namespace chrono;

class userContributionHistory;

void addCommits(userContributionHistory * thisUser);
void addIssues(userContributionHistory * thisUser);

//make github search, return string results
string getSearch(string username, string date);
size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data);

//takes in string from search, parses, updates date:number actions map, return parsed Value object
multimap<int,tm> commitInterpret( string searchResult, tm curr );
multimap<int,tm> issueInterpret( string searchResult, tm curr );

tm getCurrentTime();
string dateFromyday(int yday);

void printContributions(userContributionHistory * thisUser, int * commitArray);

//fetch and add data to new user
void addCommits(userContributionHistory * thisUser){
    string username = thisUser->getUsername();
    string commitURL = "https://api.github.com/search/commits?q=author:"+username+"&sort=created";
    string commitHeader = "Accept: application/vnd.github.cloak-preview";
    string commitSearchString = getSearch(commitURL, commitHeader);
    if (commitSearchString.empty()){
        cout << "Error: get commit search results" << endl;
    }
    thisUser->update(0,commitSearchString);
}

void addIssues(userContributionHistory * thisUser){
    string username = thisUser->getUsername();
    string issueURL = "https://api.github.com/search/issues?q=author:"+username+"&sort=created";
    string issueSearchString = getSearch(issueURL,"");
    if (issueSearchString.empty()){
        cout << "Error: get commit search results" << endl;
    }
    
    thisUser->update(1,issueSearchString);
}

//for GET search
size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}


//GET search api results
string getSearch(string url, string header){
    auto curl = curl_easy_init();
    if (curl) {
        
        //set options
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        if (!header.empty()){
            struct curl_slist *list = NULL;
            list = curl_slist_append(list, header.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        }
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "juliaschell");
        curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
        curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
        
        string response_string=""; string header_string="";
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);
        
        //get data
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl = NULL;
        
        return response_string;
    }
    
    return "";
}

//parse and use commit data
multimap<int,tm> commitInterpret( string searchResult, tm curr ){
    //parse data
    Json::Value data;
    Json::CharReaderBuilder builder;
    Json::CharReader * reader = builder.newCharReader();
    string errors;
    
    if (!reader->parse(searchResult.c_str(), searchResult.c_str() + searchResult.size(), &data, &errors) ){
        cout << "An error occured with commit parsing" << endl;
        exit;
    }
    else if (data["items"].size() == 0){
        cout << "This search had no results!" << endl;
        exit;
    }
    
    //set to return: add date of each issue
    multimap<int,tm> commitCount;
    
    //go through listed commits
    int numItems = data["items"].size();
    for (int i = 0; i < numItems; i++){
        string returnedDate = data["items"][i]["commit"]["committer"]["date"].asString();
        //parse UTC YYYY-MM-DD format
        int y = stoi(returnedDate.substr(0,4)); int m = stoi(returnedDate.substr(5,2)); int d = stoi(returnedDate.substr(8,2));
        //make tm object to compare to current date
        struct tm date;
        date.tm_year = y-1900; date.tm_mon = m-1; date.tm_mday = d;
        mktime(&date);
        //ignore if over a year old
        //if (curr.tm_yday>=date->tm_yday & curr.tm_year>date->tm_year)
           // continue;
        //otherwise (within a year) add to map
        commitCount.insert(pair<int,tm>(date.tm_yday, date));
        
        //insert message
        /*string comment = data["items"][i]["commit"]["message"].asString();
         if (dateMap[date] == comment)
         continue;
         else
         dateMap.emplace(date,comment);*/
    }
    return commitCount;
}

//parse and use issue data
multimap<int,tm> issueInterpret( string searchResult, tm curr ){
    //parse data
    Json::Value data;
    Json::CharReaderBuilder builder;
    Json::CharReader * reader = builder.newCharReader();
    string errors;
    
    //parse, check for errors
    if (!reader->parse(searchResult.c_str(), searchResult.c_str() + searchResult.size(), &data, &errors) ){
        cout << "An error occured with issue parsing" << endl;
        exit;
    }
    else if (data["items"].size() == 0){
        cout << "This search had no results!" << endl;
        exit;
    }
    
    //set to return: add date of each issue
    multimap<int,tm> issueCount;
    
    
    //iterate through returned issues
    int numItems = data["items"].size();
    for (int i = 0; i < numItems; i++){
        string returnedDate = data["items"][i]["created_at"].asString();
        int y = stoi(returnedDate.substr(0,4)); int m = stoi(returnedDate.substr(5,2)); int d = stoi(returnedDate.substr(8,2));
        struct tm date;
        date.tm_year = y-1900; date.tm_mon = m-1; date.tm_mday = d;
        mktime(&date);
        
        //if (curr.tm_yday>=date.tm_yday & curr.tm_year!=date.tm_year)
            //continue;
        
        issueCount.insert(pair<int,tm>(date.tm_yday,date));
        
        //add title info to map
        /*string title = data["items"][i]["title"].asString();
         if (dateMap[date] == title)
         continue;
         else
         dateMap.emplace(date,title); */
    }
    return issueCount;
}

tm getCurrentTime(){
    time_t t = time(nullptr);
    struct tm * tmCurr = gmtime(&t);
    return *tmCurr;
}

//print contribution data from 365 days ago to current day
void printContributions(userContributionHistory * thisUser, int * commitArray){
    tm dCurr = getCurrentTime(); tm searchDate = dCurr;
    cout << "Contributions for " << thisUser->getUsername() << ", by date" << endl;
    
    //set to 365 days ago
    searchDate.tm_year--;
    int i = 0;
    for (searchDate.tm_yday++; searchDate.tm_yday <= 365; searchDate.tm_yday++, i++){
        string dateUTC = dateFromyday(searchDate.tm_yday);
        int c = thisUser->getTotalCount(searchDate);
        commitArray[i] = c;
        cout << dateUTC << "/" << searchDate.tm_year + 1900 << ": " << c << endl;
    }
    //print this year's contributions
    searchDate.tm_year++;
    for (searchDate.tm_yday = 0; searchDate.tm_yday<=dCurr.tm_yday; searchDate.tm_yday++, i++){
        string dateUTC = dateFromyday(searchDate.tm_yday);
        int c = thisUser->getTotalCount(searchDate);
        commitArray[i] = c;
        cout << dateUTC << "/" << searchDate.tm_year + 1900 <<  ": " << thisUser->getTotalCount(searchDate) << endl;
    }
}


string dateFromyday(int yday){
    tm t = getCurrentTime();
    t.tm_mday = 1;
    t.tm_mon = 0;
    time_t day = mktime(&t) + (yday - 1) * 86400;
    gmtime_r(&day, &t);
    return to_string(t.tm_mon+1) + "/" + to_string(t.tm_mday);
}

#endif

