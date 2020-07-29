//
// Created by assorh@wincs.cs.bgu.ac.il on 02/12/2019.
//

#ifndef SPL1_SESSION_H
#define SPL1_SESSION_H

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"

class User;
class Watchable;

class Session{
public:
    Session(const std::string &configFilePath);     //constructor
    ~Session();     //disstructor
    Session(const Session &other); //copy constructor
    Session& operator=(const Session &other); //copy assignment
    Session(Session &&other);
    Session &operator=(Session&& other);
    void start();
    std::string toString2();    //return word2
    std::string toString3();    //return word3
    bool userExist(const std::string& name);    //return if there is already a user with that name
    void newUser(const std::string& name,User* created);
    bool NewActiveUser();
    std::string DelVec();
    User* FindOrg(const std::string& name);
    void PrintFunc(const std::string& what);
    Watchable* getContent (long id) const;
    User* getActive() const;
    void clear();
    void Copy(const Session &other);
    void Move(Session &other);
    const std::vector<BaseAction *> &getActionsLog() const;
    const std::unordered_map<std::string, User *> &getUserMap() const;
    const std::vector<Watchable *> &getContent1() const;
//    void DelContent(int i);


private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    std::string line;
    std::string word1;
    std::string word2;
    std::string word3;
    bool yes;
};

#endif //SPL1_SESSION_H
