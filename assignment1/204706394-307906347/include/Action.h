//
// Created by assorh@wincs.cs.bgu.ac.il on 02/12/2019.
//

#ifndef SPL1_ACTION_H
#define SPL1_ACTION_H

#include <string>
#include <iostream>

class Session;

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};


class BaseAction{
public:
    BaseAction();
    virtual ~BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Session& sess)=0;
     virtual BaseAction* clone()=0;
    virtual std::string toString() const=0;
protected:
    void complete();
    void error(const std::string& errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
};

class CreateUser  : public BaseAction {
public:

    virtual CreateUser* clone();
    virtual void act(Session& sess);
    virtual std::string toString() const;
};

class ChangeActiveUser : public BaseAction {
public:
    virtual void act(Session& sess);
    virtual ChangeActiveUser* clone();
    virtual std::string toString() const;
};

class DeleteUser : public BaseAction {
public:
    virtual DeleteUser* clone();
    virtual void act(Session & sess);
    virtual std::string toString() const;
};


class DuplicateUser : public BaseAction {
public:
    virtual DuplicateUser* clone();
    virtual void act(Session & sess);
    virtual std::string toString() const;
};

class PrintContentList : public BaseAction {
public:
    virtual PrintContentList* clone();

    virtual void act (Session& sess);
    virtual std::string toString() const;
};

class PrintWatchHistory : public BaseAction {
public:
    virtual PrintWatchHistory* clone();
    virtual void act (Session& sess);
    virtual std::string toString() const;
};


class Watch : public BaseAction {
public:
    virtual Watch* clone();
    virtual void act(Session& sess);
    virtual std::string toString() const;
};


class PrintActionsLog : public BaseAction {
public:
    virtual PrintActionsLog* clone();
    virtual void act(Session& sess);
    virtual std::string toString() const;
};

class Exit : public BaseAction {
public:
    virtual Exit* clone();
    virtual void act(Session& sess);
    virtual std::string toString() const;
};
#endif //SPL1_ACTION_H
