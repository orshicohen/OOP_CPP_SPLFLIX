//
// Created by assorh@wincs.cs.bgu.ac.il on 02/12/2019.
//

#include "../include/Watchable.h"
#include <string>
#include <vector>
#include "../include/Session.h"
#include "../include/User.h"


Watchable::Watchable(long Id, int Length, const std::vector<std::string>& Tags):id(Id),length(Length),tags(Tags){}

Watchable::~Watchable() {}

std::string Watchable::getId(){

    return std::to_string(id);
}

long Watchable::getLongId() const {
    return id;
}


int Watchable::getLength() const {
    return length;
}

const std::vector<std::string> &Watchable::getTags() const {
    return tags;
}

Movie::Movie(long Id, const std::string& Name, int Length, const std::vector<std::string>& Tags)
        :Watchable(Id,Length,Tags),name(Name){}

std::string Movie::toString() const {
    return name;
}

std::string Movie::toString2() {
    std::string str;
    const std::vector<std::string> & tags=getTags();
    int length = getLength();

    int x = tags.size();
    for(const auto& i : tags){
        if(i == tags [x-1] )
            str += i ;
        else
            str += i + ", ";
    }

    return " " + std::to_string(length) + " minutes" + " " + "[" + str+ "]";///to check

}
Watchable* Movie :: getNextWatchable(Session& s) const{
    return s.getActive()->getRecommendation(s);

}

Movie *Movie::clone() {
    return (new Movie(*this));
}


Episode::Episode(long Id,const std::string& SeriesName,int Length, int Season, int Episode ,
        const std::vector<std::string>& Tags):Watchable(Id,Length,Tags), seriesName(SeriesName),season(Season),
        episode(Episode),nextEpisodeId(Id+1){}

std::string Episode::toString() const {
    return seriesName + " S" + std::to_string(season) + "E" + std::to_string(episode);
}


std::string Episode::toString2() {
    std::string str;
    str = "";
    const std::vector<std::string> & tags=getTags();
    int length = getLength();
    int x = tags.size();
    for(const auto& i : tags){
        if(i == tags [x-1] )
            str += i ;
        else
            str += i + ", ";
    }
    return " " + std::to_string(length) + " minutes  [" + str + "]";
}
Watchable* Episode:: getNextWatchable(Session& s) const{
    if(nextEpisodeId!=0)///to check
        return s.getContent(nextEpisodeId);
    return s.getActive()->getRecommendation(s);

}

Watchable *Episode::clone() {
    return new Episode(*this);
}




