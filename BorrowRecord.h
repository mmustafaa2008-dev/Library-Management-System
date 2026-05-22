#pragma once
class BORROWRECORD // no needed exception here
{
private:
    string username;
    string id;
    string author;
    string title;
    string issuedate;
    string returndate;
    string duedate;
    double fine;
    bool paidfine;

public:
    //----------------setter and getter--------------
    void set_id(string i) { id = i; }
    void set_issuedate(string s) { issuedate = s; }
    void set_returndate(string r) { returndate = r; }
    void set_duedate(string d) { duedate = d; }
    void set_fine(double f) { fine = f; }
    void set_username(string b) { username = b; }
    void set_paidfine(bool f) { paidfine = f; }
    void set_author(string a) { author = a; }
    void set_title(string b) { title = b; }

    string get_issuedate() { return issuedate; }
    string get_returndate() { return returndate; }
    string get_duedate() { return duedate; }
    string get_id() { return id; }
    string get_username() const { return username; }
    string get_author() { return author; }
    string get_title() { return title; }
    double get_fine() { return fine; }
    bool get_paidfine() { return paidfine; }

    //---------------constructor---------------

    BORROWRECORD() // default
    {
        id = "";
        username = "";
        issuedate = "";
        returndate = "";
        duedate = "";
        fine = 0;
        paidfine = false;
    }

    BORROWRECORD(string a, string b, string e, string f) // parametrized
    {
        id = a;
        username = b;
        issuedate = e;
        returndate = "";
        duedate = f;
        fine = 0;
        paidfine = false;
    }
    BORROWRECORD(const BORROWRECORD &b) // copy
    {
        id = b.id;
        username = b.username;
        issuedate = b.issuedate;
        returndate = b.returndate;
        duedate = b.duedate;
        fine = b.fine;
        paidfine = b.paidfine;
    }
    //--------------------------methods--------------------------
    double calculatefine(string membership)
    {
        struct tm due_tm = {0};    // 20
        struct tm return_tm = {0}; // 30

        // for storing the integer indivisually
        int d1, d2, m1, m2, y1, y2;

        if (sscanf(duedate.c_str(), "%d-%d-%d", &d1, &m1, &y1) != 3 ||
            sscanf(returndate.c_str(), "%d-%d-%d", &d2, &m2, &y2) != 3)
        {
            return 0.0; // Return 0 fine if dates are malformed
        }

        // for reading the date from the string
        sscanf(duedate.c_str(), "%d-%d-%d", &d1, &m1, &y1);
        sscanf(returndate.c_str(), "%d-%d-%d", &d2, &m2, &y2);

        // for due date
        due_tm.tm_mday = d1;
        due_tm.tm_mon = m1 - 1;
        due_tm.tm_year = y1 - 1900;

        // for return date
        return_tm.tm_mday = d2;
        return_tm.tm_mon = m2 - 1;
        return_tm.tm_year = y2 - 1900;

        time_t time1 = mktime(&due_tm);
        time_t time2 = mktime(&return_tm);                     //  time2 should be greater than time1
        double diff = difftime(time2, time1) / (60 * 60 * 24); // for converting the date into seconds

        if (diff <= 0)
        {
            return 0; // on time
        }
        double totalfine = diff * 10;
        if (membership == "Gold")
        {
            totalfine *= 0.50;
        }
        else if (membership == "Silver")
        {
            totalfine *= 0.80;
        }
        else if (membership == "Basic")
        {
            if (diff > 10)
            {
                totalfine *= 2;
            }
        }
        return totalfine;
    }

    void
    display_detail()
    {
        cout << "Student Name  : " << username << endl;
        cout << "Resource ID   : " << id << endl;
        cout << "ISSUE DATE    : " << issuedate << endl;
        cout << "DUE DATE      : " << duedate << endl;
        cout << "RETURN DATE   : " << returndate << endl;
        if (fine == 0)
        {
            cout << "Fine          : " << fine << endl;
        }
        else
        {
            cout << "Fine          : " << fine << endl;
        }
        cout << endl;
    }
};
