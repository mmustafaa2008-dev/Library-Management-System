#include <iostream>
#include <string>
#include <cctype>
#include <fstream> // for file handling
#include <ctime>   // for comparing the time
#include <cstdio>  // for sscanf
#include <vector>  // for vector
#include <limits>  
#include <stdexcept> //  for builtin exceptional handling
#include <algorithm>

using namespace std;

string formatDate(int d, int m, int y)
{
    string day = (d < 10 ? "0" : "") + to_string(d);
    string month = (m < 10 ? "0" : "") + to_string(m);

    return day + "-" + month + "-" + to_string(y);
}

string getTodayDate()
{
    time_t now = time(0); 
    tm *ltm = localtime(&now);

    return formatDate(
        ltm->tm_mday,
        ltm->tm_mon + 1,
        ltm->tm_year + 1900);
}

string getFutureDate(int days) 
{
    time_t now = time(0);
    now += days * 24 * 60 * 60;

    tm *ltm = localtime(&now);

    return formatDate(
        ltm->tm_mday,
        ltm->tm_mon + 1,
        ltm->tm_year + 1900);
}
void menu() // main body
{
    cout << "----------------Library Management System---------------" << endl;
    cout << "1- USER DOMAIN" << endl;
    cout << "2- ADMIN DOMAIN" << endl;
    cout << "3- EXIT" << endl;
    cout << "Please Select from the above option : ";
}

// ADMIN CREDENTIALS
int adminid = 786;
string aduser = "admin";
string adpass = "Admin@123";

#include "Exceptions.h"

// ======================================== Helper Function ========================================//
bool checkAvailability()
{
    int value;

    while (true)
    {
        cin >> value;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Invalid Input! Enter 1 or 0: ";
            continue;
        }

        if (value == 1)
        {
            return true;
        }
        else if (value == 0)
        {
            return false;
        }
        else
        {
            cout << "Please Enter Only 1 or 0: ";
        }
    }
}
double checkDouble() 
{
    string input;

    while (true)
    {
        cin >> input;

        bool valid = true;
        bool decimalPoint = false;

        for (int i = 0; i < input.size(); i++)
        {
            if (input[i] == '.')
            {
                if (decimalPoint)
                {
                    valid = false;
                    break;
                }
                decimalPoint = true;
            }
            else if (!isdigit(input[i]))
            {
                valid = false;
                break;
            }
        }

        if (valid)
            return stod(input); 

        cout << "Invalid input! Enter numeric value (e.g. 100 or 99.5): ";
    }
}
int checkdigit()
{
    string input;

    while (true)
    {
        cin >> input;

        bool valid = true;

        for (int i = 0; i < input.size(); i++)
        {
            if (!isdigit(input[i]))
            {
                valid = false;
                break;
            }
        }

        if (valid)
            return stoi(input);

        cout << "Invalid input! Enter numbers only: ";
    }
}
bool checkusername(const string &a)
{
    if (a.length() < 3)
    {
        return false;
    }
    for (int i = 0; i < a.length(); i++)
    {
        if (a[i] == ' ')
        {
            return false;
        }
        if (!isalnum(a[i]))
        {
            return false;
        }
    }
    return true;
}
bool checkfullname(string &name) // validation for FULL NAME
{
    int words = 0;
    string word = "";

    if (name.empty())
        return false;

    for (int i = 0; i < name.size(); i++)
    {
        if (!isalpha(name[i]) && name[i] != ' ')
        {
            return false;
        }

        if (name[i] != ' ')
        {
            word += name[i];
        }
        else
        {
            if (word.length() > 0)
            {
                if (word.length() < 2)
                    return false;

                words++;
                word = "";
            }
        }
    }

    if (word.length() > 0)
    {
        if (word.length() < 2)
            return false;

        words++;
    }

    if (words < 2)
        return false;

    return true;
}
bool checkemail(string &email)
{
    int at = email.find('@');
    int dot = email.find('.', at);

    if (at == string::npos || dot == string::npos)
        return false;

    if (at == 0 || at == email.length() - 1) 
        return false;

    if (dot <= at + 1 || dot == email.length() - 1)
        return false;

    if (email.find(' ') != string::npos) 
        return false;

    if (email.find('@', at + 1) != string::npos)
        return false;

    return true;
}
bool checkpass(string a)
{
    bool hasupper = false;
    bool hasdigit = false;
    bool haslower = false;
    bool hasspecial = false;

    if (a.length() < 8)
        return false;

    for (int i = 0; i < a.length(); i++)
    {
        if (isupper(a[i]))
        {
            hasupper = true;
        }
        else if (isdigit(a[i]))
        {
            hasdigit = true;
        }
        else if (islower(a[i]))
        {
            haslower = true;
        }
        else if (ispunct(a[i]))
        {
            hasspecial = true;
        }
    }
    return (hasupper && haslower && hasdigit && hasspecial);
}
void displayPasswordMenu()
{
    cout << "+-------------------------------------------+" << endl;
    cout << "|        PASSWORD REQUIREMENTS MENU         |" << endl;
    cout << "+-------------------------------------------+" << endl;
    cout << "| 1. Minimum 8 Characters                   |" << endl;
    cout << "| 2. One Uppercase Letter (A-Z)             |" << endl;
    cout << "| 3. One Lowercase Letter (a-z)             |" << endl;
    cout << "| 4. One Digit (0-9)                        |" << endl;
    cout << "| 5. One Special Character (@, #, $, etc.)  |" << endl;
    cout << "+-------------------------------------------+" << endl;
}

// ============================ CLASSES OF THE PROJECTS ======================================//
#include "Person.h"
#include "User.h"
#include "Resource.h"
#include "BorrowRecord.h"
#include "Admin.h"
#include "Reservation.h"
#include "Library.h"

void mmenu() // for book
{
    cout << "1.  Issue Resource" << endl;
    cout << "2.  Return Resource" << endl;
    cout << "3.  Search Resource" << endl;
    cout << "4.  Search Online + Request API" << endl;
    cout << "5.  Pay Fine" << endl;
    cout << "6.  View My Books" << endl;
    cout << "7.  Update Account Credential" << endl;
    cout << "8.  View All Available Resource" << endl;
    cout << "9.  View Balance" << endl;
    cout << "10. Add Balance" << endl;
    cout << "11. Reserve Book" << endl;
    cout << "12. View My Reservations" << endl;
    cout << "13. Cancel Reservation" << endl;
    cout << "14- Renew Membership" << endl;
    cout << "15- Cancel Membership" << endl;
    cout << "16. Exit" << endl;
    cout << "Enter your choice : ";
}

void user(LIBRARY &l)
{
    int choice;
    while (true)
    {
        cout << "1- Sign In" << endl;
        cout << "2- Sign Up" << endl;
        cout << "3- Exit" << endl;
        cout << "Select The Above Option : ";
        choice = checkdigit();

        if (choice == 1) 
        {
            if (l.authenticUser() == 0) 
            {
                int innerchoice;
                do
                {
                    mmenu();
                    innerchoice = checkdigit();
                    try
                    {

                        switch (innerchoice)
                        {
                        case 1:
                            l.issueResource();
                            l.saveBooks();
                            l.saveRecords();
                            break;

                        case 2:
                            l.returnResource();
                            l.saveBooks();
                            l.saveRecords();
                            break;

                        case 3:
                            l.searchResource();
                            break;

                        case 4:
                            l.searchandRequestBook();
                            break;

                        case 5:
                            l.payfine();
                            l.saveRecords();
                            break;

                        case 6:
                            l.mybook();
                            break;

                        case 7:
                            l.updateuser();
                            l.saveUsers();
                            break;

                        case 8:
                            l.availableresource();
                            break;

                        case 9:
                            l.viewbalance();
                            break;
                        case 10:
                            l.addBalance();
                            break;
                        case 11:
                            l.reserveResource();
                            l.saveReservations();
                            break;
                        case 12:
                            l.viewMyReservations();
                            break;
                        case 13:
                            l.cancelReservation();
                            l.saveReservations();
                            break;

                        case 14:
                            l.renewmembership();
                            l.saveUsers();
                            break;

                        case 15:
                            l.cancelmembership();
                            l.saveUsers();
                            break;

                        case 16:
                            l.saveBooks();
                            l.saveUsers();
                            l.saveRecords();
                            l.saveReservations();
                            break;

                        default:
                            cout << "Please Select The Above Option : " << endl;
                        }
                    }
                    catch (const myexception &e)
                    {
                        cout << "Error : " << e.what() << endl;
                    }
                    catch (const exception &e)
                    {
                        cout << "Standard Error : " << e.what() << endl;
                    }
                    catch (...)
                    {
                        cout << "Unknown Error" << endl;
                    }
                } while (innerchoice != 16);
            }
            else
            {
                int choice;
                while (true)
                {
                    cout << "------------------------Please SignUp-------------------------" << endl;

                    cout << "1. Sign up" << endl;
                    cout << "2. Exit" << endl;
                    cout << "Please Select The Above Option : ";
                    choice = checkdigit();

                    if (choice == 1)
                    {
                        l.registerUser();
                        l.saveUsers();
                    }
                    else if (choice == 2)
                    {
                        break;
                    }
                    else
                    {
                        cout << "Invalid Option! Please Select 1 or 2." << endl;
                    }
                }
            }
        }
        else if (choice == 2)
        {
            try
            {
                if (l.registerUser() == true)
                {
                    int choice;
                    l.saveUsers();
                    cout << "1. Sign in" << endl;
                    cout << "2. Exit" << endl;
                    cout << "Please Select The Above Option : ";
                    choice = checkdigit();

                    if (choice == 1)
                    {
                        if (l.authenticUser() == 0)
                        {
                            do
                            {
                                mmenu();
                                choice = checkdigit();
                                try
                                {

                                    switch (choice)
                                    {
                                    case 1:
                                        l.issueResource();
                                        l.saveBooks();
                                        l.saveRecords();
                                        break;

                                    case 2:
                                        l.returnResource();
                                        l.saveBooks();
                                        l.saveRecords();
                                        break;

                                    case 3:
                                        l.searchResource();
                                        break;

                                    case 4:
                                        l.searchandRequestBook();
                                        break;

                                    case 5:
                                        l.payfine();
                                        break;

                                    case 6:
                                        l.mybook();
                                        break;

                                    case 7:
                                        l.updateuser();
                                        l.saveUsers();
                                        break;

                                    case 8:
                                        l.availableresource();
                                        break;

                                    case 9:
                                        l.viewbalance();
                                        break;
                                    case 10:
                                        l.addBalance();
                                        break;

                                    case 11:
                                        l.reserveResource();
                                        l.saveReservations();
                                        break;
                                    case 12:
                                        l.viewMyReservations();
                                        break;
                                    case 13:
                                        l.cancelReservation();
                                        l.saveReservations();
                                        break;

                                    case 14:
                                        l.renewmembership();
                                        l.saveUsers();
                                        break;

                                    case 15:
                                        l.cancelmembership();
                                        l.saveUsers();
                                        break;
                                    case 16:
                                        l.saveBooks();
                                        l.saveUsers();
                                        l.saveRecords();
                                        l.saveReservations();
                                        break;

                                    default:
                                        cout << "Please Select The Above Option : " << endl;
                                    }
                                }
                                catch (const myexception &e)
                                {
                                    cout << "Error : " << e.what() << endl;
                                }
                                catch (const exception &e)
                                {
                                    cout << "Standard Error : " << e.what() << endl;
                                }
                                catch (...)
                                {
                                    cout << "Unknown Error" << endl;
                                }
                            } while (choice != 16);
                        }
                        else
                        {
                            cout << "User Not Found" << endl;
                        }
                    }
                }
            }
            catch (const myexception &e)
            {
                cout << "Error : " << e.what() << endl;
            }
        }
        else if (choice == 3)
        {
            return;
        }
        else
        {
            cout << "Please Enter the Correct Option" << endl;
        }
    }
}
void admin(ADMIN &a, LIBRARY &l)
{
    a.adlogin(l.getBooks(), l.getRecords(), l.getMembers());
    l.saveBooks();
    l.saveRecords();
}
void Lib() // main controller menu function that control user and admin
{
    ADMIN a;
    LIBRARY l;

    l.loadBooks();
    l.loadUsers();
    l.loadRecords();
    l.loadReservations();

    int choice;

    while (true)
    {
        try
        {
            menu();
            choice = checkdigit();

            if (choice == 1)
            {
                user(l);
            }
            else if (choice == 2) // for admin login
            {
                admin(a, l);
            }
            else if (choice == 3)
            {
                l.saveBooks();
                l.saveUsers();
                l.saveRecords();
                l.saveReservations();
                return;
            }
            else
            {
                cout << "Please Select the desired Option : " << endl;
            }
        }
        catch (const myexception &e)
        {
            cout << "Library Error : " << e.what() << endl;
        }
        catch (const exception &e)
        {
            cout << "System Error : " << e.what() << endl;
        }
        catch (...)
        {
            cout << "Unknown Error" << endl;
        }
    }
}

int main()
{
    Lib();
    return 0;
}