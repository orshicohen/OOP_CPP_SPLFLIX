//
// Created by assorh@wincs.cs.bgu.ac.il on 02/12/2019.
//

#include "../include/User.h"
#include "../include/Watchable.h"
#include "../include/Session.h"
#include <cmath>
#include <utility>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
using namespace std;
class Watchable;
class Session;


User::User(std::string Name):history(),name(Name),rec(){}
User::~User() {}
std::string User::getName() const {
    return name;
}

std::vector<Watchable*> User::get_history() const {
    return history;
}
std::string User::getRec() const{
    return rec;
}
void User::add_to_hist(Watchable* add){
    history.push_back(add);
}

void User::setHistory(const std::vector<Watchable *> &new_hist) {
    history = new_hist;
}

void User::setRec(const string &Rec) {
    rec = Rec;
}


LengthRecommenderUser::LengthRecommenderUser(const std::string& name):User(name){
    setRec("len");
}
Watchable* LengthRecommenderUser::getRecommendation(Session& s){
    int sum=0;
    float avg = 0;
    for (auto & i : history){
        sum += i->getLength();
    }
    avg=(float)sum/(float)(history.size());
    float min_distance=-1;
    Watchable* watch=s.getContent(1);
    Watchable* closest= nullptr;
    float this_distance;
    int i=1;
    while (watch != nullptr){
        auto length= (float) watch->getLength();
        this_distance = std::abs(avg-length);
        if(this_distance<min_distance || min_distance==-1){
            bool flag = false;
            for (auto & j : history){
                if(watch==j)
                    flag= true;
            }
            if (!flag){
                min_distance=this_distance;
                closest=watch;
            }
        }
        i++;
        watch=s.getContent(i);

    }
    return closest;

}

LengthRecommenderUser *LengthRecommenderUser::clone(const Session& s) {
    LengthRecommenderUser* newUser=new LengthRecommenderUser(getName());
    for(unsigned int i=0;i<history.size();i++){
        Watchable* w =s.getContent(history[i]->getLongId());
        newUser->add_to_hist(w);
    }
    return newUser ;
}




RerunRecommenderUser::RerunRecommenderUser(const std::string& name):User(name), historyIDX(0){
    setRec("rer");
}

Watchable* RerunRecommenderUser::getRecommendation(Session& s) {
    Watchable* toReturn = s.getContent(history[historyIDX]->getLongId());
    historyIDX += 1;
    return toReturn;
}

int RerunRecommenderUser::getHistoryIdx() const {
    return historyIDX;
}

RerunRecommenderUser *RerunRecommenderUser::clone(const Session& s) {
    RerunRecommenderUser* newUser=new RerunRecommenderUser(getName());
    for(unsigned int i=0;i<history.size();i++){
        Watchable* w =s.getContent(history[i]->getLongId());
        newUser->add_to_hist(w);
    }
    newUser->setHistoryIdx(historyIDX);
    return newUser ;
}

void RerunRecommenderUser::setHistoryIdx(int historyIdx2) {
    historyIDX = historyIdx2;
};

GenreRecommenderUser *GenreRecommenderUser::clone(const Session& s) {
    GenreRecommenderUser* newUser=new GenreRecommenderUser(getName());
    for(unsigned int i=0;i<history.size();i++){
        Watchable* w =s.getContent(history[i]->getLongId());
        newUser->add_to_hist(w);
    }
    return newUser ;
}



GenreRecommenderUser::GenreRecommenderUser(const std::string& name):User(name){
    setRec("gen");
}
Watchable* GenreRecommenderUser::getRecommendation(Session& s){

    std::unordered_map<std::string,int> TagsMap;
    for (auto & i : history){
        const std::vector<std::string>& tags = i->getTags();
        for (const auto & tag : tags) {
            if (TagsMap.find(tag) == TagsMap.end())
                TagsMap.insert({tag, 1});
            else
                TagsMap.at(tag) += 1;
        }
    }
    //bool finish =flase;
    while(true) {
        int maxValue = 0;
        std::string maxGenreStr;
        for (auto const &x:TagsMap) {
            if (x.second > maxValue) {
                maxValue = x.second;
                maxGenreStr = x.first;
            }
            if (x.second == maxValue) {
                if (maxGenreStr > x.first)//tocheck
                    maxGenreStr = x.first;
            }
        }
        bool condition=true;
        for (auto & j : s.getContent1()) {
            const std::vector <std::string> &tags = j->getTags();
            for (const auto & tag : tags) {
                if (tag == maxGenreStr) {
                    for (auto &x : history) {
                        if (x == j){
                             condition=false;
                        break;
                    }
                    }
                    if(condition)
                    return j;
                    condition=true;
                }
            }

        }
        TagsMap.erase(maxGenreStr);
    }

}


