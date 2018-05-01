//
//  workWithStrings.cpp
//  usersProject
//
//  Created by Julia Schell on 4/30/18.
//  Copyright © 2018 Julia Schell. All rights reserved.
//



#ifndef mainFuncs_h
#define mainFuncs_h

#include "workWithStrings.h"
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <set>
#include <ctime>
using namespace std;

#endif /* mainFuncs_h */

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
bool commitInterpret( string searchResult, map<string, string>& dateMap ){
    //parse data
    Json::Value data;
    Json::CharReaderBuilder builder;
    Json::CharReader * reader = builder.newCharReader();
    string errors;
    
    if (!reader->parse(searchResult.c_str(), searchResult.c_str() + searchResult.size(), &data, &errors) ){
        cout << "An error occured with commit parsing" << endl;
        return false;
    }
    else if (data["items"].size() == 0){
        cout << "This search had no results!" << endl;
        return false;
    }
    
    //go through listed commits
    int numItems = data["items"].size();
    for (int i = 0; i < numItems; i++){
        string dateUCT = data["items"][i]["commit"]["committer"]["date"].asString();
        string date = dateUCT.substr(0,10);
        string comment = data["items"][i]["commit"]["message"].asString();
        if (dateMap[date] == comment)
            continue;
        else
            dateMap.emplace(date,comment);
    }
    return true;
}

//parse and use issue data
set<int> issueInterpret( string searchResult, tm curr ){
    //parse data
    Json::Value data;
    Json::CharReaderBuilder builder;
    Json::CharReader * reader = builder.newCharReader();
    string errors;
    
    if (!reader->parse(searchResult.c_str(), searchResult.c_str() + searchResult.size(), &data, &errors) ){
        cout << "An error occured with issue parsing" << endl;
        exit;
    }
    else if (data["items"].size() == 0){
        cout << "This search had no results!" << endl;
        exit;
    }
    
    set<int> issueCount;
    

    //get date
    time_t rawtime = time(nullptr); struct tm * ptm;
    ptm = gmtime ( &rawtime );
    int currDay = ptm->tm_yday; int currYear = ptm->tm_year;
    
    //go through listed commits
    int numItems = data["items"].size();
    for (int i = 0; i < numItems; i++){
        string dateUCT = data["items"][i]["created_at"].asString();
        int y = stoi(dateUCT.substr(0,4)); int m = stoi(dateUCT.substr(5,2)); int d = stoi(dateUCT.substr(8,2));
        struct tm * date; date->tm_year = y-1990; date->tm_mon = m-1; date->tm_mday = d;
        mktime(date);
        
        //ignore if over a year old
        if (currDay>=date->tm_yday & currYear>date->tm_year)
            continue;
        
        issueCount.insert(date->tm_yday)
        
        //string title = data["items"][i]["title"].asString();
        /*if (dateMap[date] == title)
            continue;
        else
            dateMap.emplace(date,title); */
    }
    
}

/*
class MDY{
public:
    MDY(int m, int d, int y):month(m), day(d), year(y){};
    MDY operator - (MDY date);
    int getMonth(){return month;};
    int getDay(){return day;};
    int getYear(){return year;}
private:
    int month;
    int day;
    int year;
}

MDY MDY::operator-(MDY date){
    mNew = month - date.getMonth();
    dNew = day - date.getDay();
    yNew = year - date.getYear();
    return MDY(mNew, dNew, yNew);
}

chrono::year_month_day::year_month_day UTCtoymd(string utc){
    //UTC format YYYY-MM-DD
    int y = stoi(dateUTC.substr(0,4));
    int m = stoi(dateUCT.substr(5,2));
    int d = stoi(dateUCT.substr(8,2));
    year_month_day date(year(y),month(m),day(d));
    return date;
} */


