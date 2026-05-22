#pragma once
// ============================ EXCEPTION CLASSES ===========================================//
class myexception : public exception // base exception class
{
protected:
    string message;

public:
    myexception(string a) : message(a) {}
    const char *what() const noexcept override
    {
        return message.c_str();
    }
};
class userexception : public myexception
{
public:
    userexception(string a) : myexception(a) {}
};
class adminexception : public myexception
{
public:
    adminexception(string message) : myexception(message) {}
};
class booknotfound : public myexception
{
public:
    booknotfound(string message) : myexception(message) {}
};
class limitexceed : public myexception
{
public:
    limitexceed(string message) : myexception(message) {}
};
class fineexception : public myexception
{
public:
    fineexception(string message) : myexception(message) {}
};
class membershipexception : public myexception
{
public:
    membershipexception(string m) : myexception(m) {}
};
