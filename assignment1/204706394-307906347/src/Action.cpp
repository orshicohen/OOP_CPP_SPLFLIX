//
// Created by assorh@wincs.cs.bgu.ac.il on 02/12/2019.
//
#include "../include/User.h"
#include "../include/Action.h"
#include "../include/Session.h"
#include "../include/Watchable.h"
#include <iostream>
#include <string>

BaseAction::BaseAction():errorMsg (""), status(PENDING){
}

BaseAction::~BaseAction(){}

ActionStatus BaseAction::getStatus() const{
    return status;
}


void BaseAction::complete(){
    status = COMPLETED; //CHECK IF DO IT LIKE THAT
}


void BaseAction::error(const std::string& o){
    status = ERROR;
    errorMsg = o;       //check
    std::cout << "Error - " + errorMsg << std::endl;

}


std::string BaseAction::getErrorMsg() const{
    return errorMsg;//check refrence
}



void CreateUser::act(Session& sess){
    std::string name = sess.toString2();
    std::string rec = sess.toString3();

    bool exist = sess.userExist(name);
    if (exist){
        error("There is already a user with that name");
    }
    else {
        if (rec == "len") {
            User* created = new LengthRecommenderUser(name);
            sess.newUser(name,created);
            complete();
        }

        else if (rec == "rer") {
            User* created = new RerunRecommenderUser(name);
            sess.newUser(name,created);
            complete();
        }

        else if (rec == "gen") {
            User* created = new GenreRecommenderUser(name);
            sess.newUser(name,created);
            complete();
        }

        else
            error("The 3-letter code is invalid");
    }
}

std::string CreateUser::toString() const{
    if (getStatus() == COMPLETED)
        return "CreateUser COMPLETED";
    else
        return "CreateUser ERROR: " + getErrorMsg();
}

CreateUser *CreateUser::clone() {
    return new CreateUser(*this);
}


void ChangeActiveUser::act(Session& sess){
    bool exist = sess.NewActiveUser();
    if (exist){
        complete();
    }
    else
        error("This user doesn't exist");

}

std::string ChangeActiveUser::toString() const{
    if (getStatus() == COMPLETED)
        return "ChangeActiveUser COMPLETED";
    else
        return "ChangeActiveUser ERROR: " + getErrorMsg();
}

ChangeActiveUser *ChangeActiveUser::clone() {
    return new ChangeActiveUser(*this);
}


void DeleteUser::act(Session & sess){
    std::string exist = sess.DelVec();
    if (exist=="C"){
        sess.DelVec();
        complete();
    }
    else if (exist=="A")
        error("This user doesn't exist");
    else
        error("can't delete activeuser");

}
DeleteUser *DeleteUser::clone() {
    return new DeleteUser(*this);
}
std::string DeleteUser::toString() const{
    if (getStatus() == COMPLETED)
        return "DeleteUser COMPLETED";
    else
        return "DeleteUser ERROR: " + getErrorMsg();
}


DuplicateUser *DuplicateUser::clone() {
    return new DuplicateUser(*this);
}


void DuplicateUser::act(Session & sess){
    std::string name1 = sess.toString2();
    std::string name2 = sess.toString3();

    bool exist1 = sess.userExist(name1);
    bool exist2 = sess.userExist(name2);

    if (!exist1)
        error("Original user doesn't exist");

    else if (exist2)
        error("New user name already taken");
    else {
        User* Org = sess.FindOrg(name1);
        //User created = Org; //Copy constructor
        std::string rec = Org->getRec();
        if (rec=="len"){
            User* created = new LengthRecommenderUser(name2);
            created->setHistory(Org->get_history());
            sess.newUser(name2,created);
        }

        else if (rec=="rer")
        {
            User* created = new RerunRecommenderUser(name2);
            created->setHistory(Org->get_history());
            sess.newUser(name2,created);
        }
        else{
            User* created = new GenreRecommenderUser(name2);
            created->setHistory(Org->get_history());
            sess.newUser(name2,created);
        }
        complete();

    }
}

std::string DuplicateUser::toString() const{
    if (getStatus() == COMPLETED)
        return "DuplicateUser COMPLETED";
    else
        return "DuplicateUser ERROR: " + getErrorMsg();
}

PrintContentList *PrintContentList::clone() {
    return new PrintContentList(*this);
}

void PrintContentList::act(Session& sess){
    sess.PrintFunc("content");
    complete();
}

std::string PrintContentList::toString() const{
    return "PrintContentList COMPLETED";
}


void PrintWatchHistory::act(Session& sess){
    sess.PrintFunc("hist");
    complete();
}
std::string PrintWatchHistory::toString() const{
    return "PrintWatchHistory COMPLETED";
}

PrintWatchHistory *PrintWatchHistory::clone() {
    return new PrintWatchHistory(*this);
}

void Watch::act(Session& sess){
    sess.PrintFunc("watch");
    complete();
}

Watch *Watch::clone() {
    return new Watch(*this);
}
std::string Watch::toString() const{
    return "Watch COMPLETED";
}


void PrintActionsLog::act(Session& sess){
    sess.PrintFunc("action");
    //possible error?
    complete();
}
std::string PrintActionsLog::toString() const{
    return "PrintActionsLog COMPLETED";
    //possible error?
}

PrintActionsLog *PrintActionsLog::clone() {
    return  new PrintActionsLog(*this);
}

Exit *Exit::clone() {
    return new Exit(*this);
}

void Exit::act(Session& sess){
    complete();
}

std::string Exit::toString() const{
    return "Exit COMPLETED";
}


