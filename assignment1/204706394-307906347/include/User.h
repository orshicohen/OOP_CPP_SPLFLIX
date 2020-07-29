//
// Created by assorh@wincs.cs.bgu.ac.il on 02/12/2019.
//

#ifndef SPL1_USER_H
#define SPL1_USER_H

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
class Watchable;
class Session;

class User{
public:
    User(std::string  name);
    virtual ~User();
    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;
    std::string getRec() const;
    std::vector<Watchable*> get_history() const;
    void add_to_hist(Watchable* add);
    void setHistory(const std::vector<Watchable *> &new_hist);
    void setRec(const std::string &rec);
    virtual User* clone(const Session& s)=0;
protected:
    std::vector<Watchable*> history;
private:
    std::string name;
    std::string rec;
};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual LengthRecommenderUser* clone(const Session& s);
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    Watchable* getRecommendation(Session& s);
    virtual RerunRecommenderUser* clone(const Session& s);

    int getHistoryIdx() const;

    void setHistoryIdx(int historyIdx);

private:
    int historyIDX;
};

class GenreRecommenderUser : public User {
public:
    virtual GenreRecommenderUser* clone(const Session& s);
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
};

#endif //SPL1_USER_H
