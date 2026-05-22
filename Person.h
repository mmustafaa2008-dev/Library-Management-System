#pragma once
class person // abstract class
{
public:
    string username;

public:
    person() : username("") {}
    person(string a)
    {
        username = a;
    }
    virtual void display() = 0; // virtual function
};
