//
// Created by assorh@wincs.cs.bgu.ac.il on 02/12/2019.
//

#ifndef SPL1_WATCHABLE_H
#define SPL1_WATCHABLE_H

#include <string>
#include <vector>


class Session;

class Watchable{
public:
    Watchable(long id, int length, const std::vector<std::string>& tags);
    virtual ~Watchable();
    virtual Watchable* clone()=0;
    virtual std::string toString() const = 0;
    virtual std::string toString2() = 0;
    virtual Watchable* getNextWatchable(Session&) const = 0;
    std::string getId();
    long getLongId() const;
    const std::vector<std::string> &getTags() const;
    int getLength() const;


private:
    const long id;
    int length;
    std::vector<std::string> tags;
};



class Movie : public Watchable{
public:
    Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags);
    virtual std::string toString() const;
    virtual std::string toString2();
    virtual Watchable* getNextWatchable(Session&) const;
    virtual Movie* clone();
private:
    std::string name;
};


class Episode: public Watchable{
public:
    Episode(long id,const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
    virtual std::string toString() const;
    virtual std::string toString2();
    virtual Watchable* getNextWatchable(Session&) const;
    virtual Watchable* clone();
private:
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;

};

#endif //SPL1_WATCHABLE_H
