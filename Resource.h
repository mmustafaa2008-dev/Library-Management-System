#pragma once
class RESOURCE // no exception needed here
{
private:
    string id;
    string title;
    string author;
    string category;
    bool isavailable;

public:
    // -----------------  CONSTRUCTOR -------------------------
    RESOURCE() // DEFAULT
    {
        id = "";
        title = "";
        author = "";
        category = "";
        isavailable = true;
    }

    RESOURCE(string a, string b, string c, string d) // PARAMETRIZED CONSTRUCTOR
    {
        id = a;
        title = b;
        author = c;
        category = d;
        isavailable = true;
    }

    RESOURCE(const RESOURCE &r) // COPY CONSTRUCTOR
    {
        id = r.id;
        title = r.title;
        author = r.author;
        category = r.category;
        isavailable = r.isavailable;
    }

    // -------------------    OPERATION   ----------------------
    void check_Availability()
    {

        if (isavailable)
        {
            cout << "" << title << " is available" << endl;
        }
        else
        {
            cout << "" << title << " is not available" << endl;
        }
    }
    void mark_issued()
    {

        if (isavailable)
        {
            isavailable = false;
        }
        else
        {
            cout << title << " is already being issued" << endl;
        }
    }
    void mark_returned()
    {
        if (!isavailable)
        {
            isavailable = true;
        }
        else
        {
            cout << title << " is already being returned" << endl;
        }
    }

    void display_resource()
    {
        cout << "Resource ID   : " << id << endl;
        cout << "Title Name    : " << title << endl;
        cout << "Author Name   : " << author << endl;
        cout << "Category Type : " << category << endl;
        cout << "Availability  : " << isavailable << endl;
    }
    // ------------------- SETTER FUNCTION ----------------------
    void set_id(string a)
    {
        id = a;
    }
    void set_title(string b)
    {
        title = b;
    }
    void set_author(string c)
    {
        author = c;
    }
    void set_category(string d)
    {
        category = d;
    }
    void set_availability(bool e)
    {
        isavailable = e;
    }
    // ------------------- GETTER FUNCTION ----------------------
    string get_id()
    {
        return id;
    }
    string get_title()
    {
        return title;
    }
    string get_author()
    {
        return author;
    }
    string get_category()
    {
        return category;
    }
    bool get_isavailable()
    {
        return isavailable;
    }

    ~RESOURCE() = default;
};
