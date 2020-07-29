//
// Created by assorh@wincs.cs.bgu.ac.il on 02/12/2019.
//
#include <iostream>
#include "../include/Session.h"
#include "../include/json.hpp"
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>
#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Watchable.h"
#include <fstream>

class User;
class Watchable;
class json;

Session::Session(const std::string &configFilePath): content(),actionsLog(),userMap(),activeUser(),
    line(""),word1 (""),word2 (""),word3 (""),yes (false)
    {
    using json=nlohmann::json ;
    std::ifstream JFile(configFilePath);
    json j;
    JFile>>j;
    long Id=0;
    for(unsigned int i=0;i<j["movies"].size();i++) // run on movies arr
    {
        Id++;
        std::string Name = j["movies"][i]["name"];
        int Length = j["movies"][i]["length"];
        std::vector<std::string> Tags = j["movies"][i]["tags"];
        Watchable* enter = new Movie(Id,Name,Length,Tags); // fill data for movie
        content.push_back(enter) ; // enter movie[i] in content vector
    }
    for(unsigned int i=0;i<j["tv_series"].size();i++) // run on tv_series section
    {
        for(unsigned int k=0;k<(j["tv_series"][0]["seasons"]).size();k++)
            for(unsigned int h=0;h<j["tv_series"][i]["seasons"][k];h++)
            {
                Id++;
                std::string SeriesName = j["tv_series"][i]["name"];
                int Length = j["tv_series"][i]["episode_length"];
                std::vector<std::string> Tags = j["tv_series"][i]["tags"];
                Watchable* enter = new Episode(Id, SeriesName, Length, k + 1, h + 1,Tags);
                content.push_back(enter); // enter episode[i][k][j] in content vector
            }
    }
    User* def = new LengthRecommenderUser("default");
    userMap.insert({"default",def});
    activeUser=def;
}




void Session::start(){
    std::cout << "SPLFLIX is now on!" << std::endl;
    bool flag= true;
    while(flag) {              //infinite loop - until exit
        if (yes) {
            BaseAction* now = new Watch();
            actionsLog.push_back(now);  //check
            now->act(*this);
        }
        else {
            int i = 1;
            word1 = "";
            word2 = "";
            word3 = "";
            std::getline(std::cin, line);
            if (line == "exit") {
                BaseAction* now = new Exit();
                actionsLog.push_back(now);  //check
                now->act(*this);
                flag= false;
            }
            for (auto x:line) {
                if (i == 1) {
                    if (x == ' ') {
                        i++;
                        continue;
                    }  else
                        word1 = word1 + x;
                }
                if (i == 2) {
                    if (x == ' ') {
                        i++;
                        continue;
                    } else
                        word2 = word2 + x;
                }
                if (i == 3) {
                    word3 = word3 + x;
                }
            }

            if (word1 == "createuser") {
                BaseAction* now = new CreateUser();
                actionsLog.push_back(now);  //check
                now->act(*this);
            } else if (word1 == "changeuser") {
                BaseAction* now = new ChangeActiveUser();
                actionsLog.push_back(now);  //check
                now->act(*this);
            } else if (word1 == "deleteuser") {
                BaseAction* now = new DeleteUser();
                actionsLog.push_back(now);  //check
                now->act(*this);
            } else if (word1 == "dupuser") {
                BaseAction* now = new DuplicateUser();
                actionsLog.push_back(now);  //check
                now->act(*this);
            } else if (word1 == "content") {
                BaseAction* now = new PrintContentList();
                actionsLog.push_back(now);  //check
                now->act(*this);
            } else if (word1 == "watchhist") {
                BaseAction* now = new PrintWatchHistory;
                actionsLog.push_back(now);  //check
                now->act(*this);
            } else if (word1 == "watch") {
                BaseAction* now = new Watch();
                actionsLog.push_back(now);  //check
                now->act(*this);
            } else if (word1 == "log") {
                BaseAction* now = new PrintActionsLog();
                actionsLog.push_back(now);  //check
                now->act(*this);
            }

        }
    }
}

std::string Session::toString2(){
    return word2;
}

std::string Session::toString3(){
    return word3;
}

bool Session::userExist(const std::string& name){
    return !(userMap.find(name) == userMap.end());
}

void Session::newUser(const std::string& name, User* created){     //check
    userMap.insert({name,created});
}

bool Session::NewActiveUser(){
    if(userMap.find(word2)==userMap.end())
        return false;
    User* changed = userMap.at(word2);
    activeUser = changed;
    return true;
}

std::string Session::DelVec(){
    if(userMap.find(word2)==userMap.end())
        return "A";
    else {
        if(activeUser->getName()==word2)
            return "B";
        User* deleted = userMap.at(word2);
        delete deleted;//distructor
        userMap.erase(word2);
        return "C";
    }
}

Watchable* Session::getContent (long id) const{
    if ((unsigned)id < content.size())
        return content[id-1];
    else
        return nullptr;
}

User* Session::getActive() const {
    return activeUser;

}



User* Session::FindOrg(const std::string& name) {
    User* Org = userMap.at(name);
    return Org;
}

void Session::PrintFunc(const std::string& what){
    if (what == "content"){
        int i = 1;
        for (auto now : content){
            std::cout << std::to_string(i) << ". " << now->toString() + now->toString2() << std::endl;
            i++;
        }
    }

    else if (what == "hist") {
        std::string name = activeUser->getName();
        std::cout << "Watch history for " + name << std::endl;
        std::vector<Watchable*> history = activeUser->get_history();
        int i = 1;
        for (auto now : history){
      //      std::string what = now->toString();
            std::cout << std::to_string(i) << ". " << now->toString() << std::endl;
            i++;
        }
    }

    else if (what == "action"){
        for (auto now : actionsLog){
            std::string act = now->toString();
            std::cout << act << std::endl;

        }
    }
    else if (what == "watch"){
        std::stringstream con(word2);
        int i = 0;
        con >> i;
        Watchable* now = content[i-1];
        std::string watch = now->toString();        //possible without
        std::cout << "Watching " + watch << std::endl;
        activeUser->add_to_hist(now);
        Watchable* next = now->getNextWatchable(*this);
        std::string next_watch = next->toString();
        std::cout << "We recommend watching " + next_watch + " continue watching? [y/n]" << std::endl;
        std::string YES;
        std::getline(std::cin, YES);
        if (YES=="y") {
            yes = true;
            word2 = next->getId();
        }
        else
            yes = false;
    }
}

Session::~Session() {
    clear();



}
void Session::clear() {
    for(unsigned int i=0;i<content.size();i++)
        delete content[i];
    content.erase(content.begin(),content.end());
    for(unsigned int i=0;i<actionsLog.size();i++)
        delete actionsLog[i];
    actionsLog.erase(actionsLog.begin(),actionsLog.end());

    activeUser= nullptr;
    for (auto const &x:userMap) {
        delete x.second;
    }
    userMap.erase(userMap.begin(),userMap.end());


}

Session::Session(const Session &other): content(),actionsLog(),userMap(),activeUser(),
    line(""),word1 (""),word2 (""),word3 (""),yes (false) {
    Copy(other);
}

void Session::Copy(const Session &other){
    for(unsigned int i=0; i<other.content.size();i++) {
        Watchable *toPush = other.content[i]->clone();
        content.push_back(toPush);
    }

    for (auto x : other.actionsLog){
        actionsLog.push_back(x->clone());
    }

    for (auto x : other.userMap){
        std::string Name = x.first;
        User* toPush = x.second->clone(*this);
        userMap.insert({Name,toPush});
    }

    std::string ActiveName = other.activeUser->getName();
    activeUser = userMap.at(ActiveName);

    line = other.line;
    word1 = other.word1;
    word2 = other.word2;
    word3 = other.word3;
    yes = other.yes;
}

Session& Session::operator=(const Session &other){
    if(&other != this) {
        clear();
        Copy(other);
    }
    return *this;
}

Session::Session(Session &&other): content(),actionsLog(),userMap(),activeUser(),
    line(""),word1 (""),word2 (""),word3 (""),yes (false) {
    Move(other);
}

Session &Session::operator=(Session&& other) {
    if (this == &other)
        return *this;
    clear();
    line = other.line;
    word1 = other.word1;
    word2 = other.word2;
    word3 = other.word3;
    yes = other.yes;
    Move(other);
    return *this;
}

void Session::Move(Session &other) {

    this->content = other.content;
//    for (auto x : other.content) {
//        x = nullptr;
//    }
    other.content.clear();

    this->actionsLog = other.actionsLog;
//    for (auto x : other.actionsLog)
//        x = nullptr;
    other.actionsLog.clear();

   // this->userMap = other.userMap;
    for (auto x : other.userMap){
        this->userMap.insert({x.first,x.second});
        x.second = nullptr;
    }
    other.userMap.erase(other.userMap.begin(),other.userMap.end());


    this->activeUser = other.activeUser;
    other.activeUser = nullptr;

}


const std::vector<Watchable *> &Session::getContent1() const {
    return content;
}

const std::unordered_map<std::string, User *> &Session::getUserMap() const {
    return userMap;
}

const std::vector<BaseAction *> &Session::getActionsLog() const {
    return actionsLog;
}




