#pragma once
class USER : public person // Remove Exception from login
{
private:
    bool isLoggedIn = false; // current status of user
    string email;
    string name;
    double balance;
    string type;
    string expiry; //  for silver and gold expiry date is 365 days
    string status;

protected:
    string password;

public:
    // -------------- SETTER FUNCTION ---------------------

    void set_username(string u) { username = u; }
    void set_password(string p) { password = p; }
    void set_email(string e) { email = e; }
    void set_name(string n) { name = n; }
    void set_bal(double b) { balance = b; }
    void set_expiry(string e) { expiry = e; }
    void set_status(string s) { status = s; }
    void set_memtype(string m) { type = m; }

    // -------------- GETTER FUNCTION ------------------------

    string get_username() const { return username; }
    string get_password() { return password; }
    string get_name() { return name; }
    string get_email() { return email; }
    double get_bal() { return balance; }
    string get_expiry() { return expiry; }
    string get_status() { return status; }
    string get_memtype() { return type; }

    //--------contructor----------------

    USER() : person(), password("XYZ"), isLoggedIn(false), balance(0.0), type("basic"), expiry("Lifetime"), status("Active") {}                                                       // basic subscription ke liye lifetime                                            // default
    USER(string a, string b, string e, string n, double f, string m, string ex, string s) : person(a), password(b), email(e), name(n), balance(f), type(m), expiry(ex), status(s) {}; // parametrized
    USER(const USER &u)
    {
        username = u.username;
        password = u.password;
        email = u.email;
        name = u.name;
        balance = u.balance;
        type = u.type;
        expiry = u.expiry;
        status = u.status;
    }

    // ----------------------- METHODS -----------------------

    void login()
    {
        string u, p, e, n;
        double b;

        cout << "Enter Your Username: ";
        getline(cin, u);
        while (!checkusername(u))
        {
            cout << "Invalid Username Format. Try again: ";
            getline(cin, u);
        }

        displayPasswordMenu();
        cout << "Enter Your Password: ";
        getline(cin, p);
        while (!checkpass(p))
        {
            displayPasswordMenu();
            cout << "Weak Password! Please follow the requirements: ";
            getline(cin, p);
        }

        if (u == username && p == password)
        {
            isLoggedIn = true;
            cout << "LOGIN SUCCESSFUL" << endl;
        }
        else
        {
            isLoggedIn = false;
            cout << "INVALID USERNAME OR PASSWORD" << endl;
        }
    }

    void logout()
    {
        if (isLoggedIn) // to check if user is loged in or not
        {
            isLoggedIn = false;
            cout << "LOGOUT SUCCESSFUL" << endl;
        }
        else
        {
            cout << "NO USER IS CURRENTLY LOGGED IN" << endl;
        }
    }
    void display() override
    {
        cout << "Username        : " << username << endl;
        cout << "Full Name       : " << name << endl;
        cout << "Email           : " << email << endl;
        cout << "Membership Type : " << type << endl;
        cout << "Status          : " << status << endl;
        cout << "Balance         : " << balance << endl;
        cout << "Expiry Date     : " << expiry << endl;
    }
    void showBalance()
    {
        cout << "Your Current Balance: " << balance << endl;
    }
};
