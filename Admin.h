#pragma once
class ADMIN : public person // one exception case is used here
{
private:
    int id;
    string password;

public:
    void enu() // for admin domain
    {
        cout << "1- Add Resource" << endl;
        cout << "2- Remove Resource" << endl;
        cout << "3- Update Resource" << endl;
        cout << "4- View All Resource" << endl;
        cout << "5- View All Issued Resource" << endl; // see who issued the book
        cout << "6- View All Student Request" << endl;
        cout << "7- Accept Student Request" << endl;
        cout << "8- Delete User" << endl;
        cout << "9- View All Users" << endl;
        cout << "10- Search User" << endl;
        cout << "11- Cancel Memebership" << endl;
        cout << "12- Exit" << endl;
        cout << "Please Select from the above option : ";
    }
    void adlogin(vector<RESOURCE> &book, vector<BORROWRECORD> &record, vector<USER> &members)
    {
        int choice;
        while (true)
        {
            cout << "Enter The Admin ID : ";
            id = checkdigit(); // exception function

            if (id > 0)
            {
                break;
            }
            cout << "Please Enter the Integer Input" << endl;
        }

        cin.ignore();

        while (true)
        {
            cout << "Enter The Username : ";
            getline(cin, username);
            if (checkusername(username))
            {
                break;
            }
            cout << "Invalid Username Format. Letter + Digit"<<endl;
        }

        while (true)
        {
            displayPasswordMenu();
            cout << "Enter The Admin Password : ";
            getline(cin, password);
            if (checkpass(password))
            {
                break;
            }

            cout << "\n>>> ERROR: ADMIN PASSWORD FORMAT INVALID <<<" << endl;
            cout << "--------------------------------------------" << endl;
        }

        if (id != adminid || username != aduser || password != adpass)
        {
            throw adminexception("Invalid Admin Credential");
        }
        else
        {
            cout << "Login Successfull" << endl;
            do
            {
                enu();
                choice = checkdigit();
                switch (choice)
                {
                case 1:
                    addResource(book);
                    break;

                case 2:
                    removeResource(book);
                    break;

                case 3:
                    updateResource(book);
                    break;

                case 4:
                    viewResource(book);
                    break;

                case 5:
                    viewIssuedBook(record);
                    break;

                case 6:
                    viewrequest();
                    break;

                case 7:
                    acceptrequest(book);
                    break;

                case 8:
                    deleteUser(members, record);
                    break;
                case 9:
                    viewAllUsers(members);
                    break;
                case 10:
                    searchUser(members, record);
                    break;

                case 11:
                    cancelUserMembership(members);
                    break;

                case 12:
                    return;

                default:
                    cout << "Please Select Above options : " << endl;
                    break;
                }
            } while (choice != 12);
        }
    }
    void display() override
    {
        cout << "Admin Username : " << username << endl;
    }
    void cancelUserMembership(vector<USER> &members)
    {
        cin.ignore();
        string uname;
        cout << "Enter Username to Cancel Membership : ";
        getline(cin, uname);
        while (!checkusername(uname))
        {
            cout << "Invalid Username Format. Try again: ";
            getline(cin, uname);
        }

        for (int i = 0; i < members.size(); i++)
        {
            if (members[i].get_username() == uname)
            {
                if (members[i].get_memtype() == "basic")
                {
                    cout << uname << " you are already on Basic plan." << endl;
                    return;
                }
                if (members[i].get_status() == "Cancelled")
                {
                    cout << "Membership is already cancelled." << endl;
                    return;
                }

                cout << "User       : " << uname << endl;
                cout << "Plan       : " << members[i].get_memtype() << endl;
                cout << "Status     : " << members[i].get_status() << endl;
                cout << "Expiry     : " << members[i].get_expiry() << endl;

                cout << "Confirm cancel membership for this user? (y/n): ";
                char confirm;
                cin >> confirm;
                if (confirm != 'y' && confirm != 'Y')
                {
                    cout << "Cancelled." << endl;
                    return;
                }

                members[i].set_status("Cancelled");
                cout << "[ADMIN] Membership of '" << uname << "' has been cancelled." << endl;
                cout << "User is now on Basic plan limits." << endl;
                return;
            }
        }
        throw userexception("User not found!");
    }
    void addResource(vector<RESOURCE> &book)
    {
        string id;
        string title;
        string author;
        string category;
        bool availability;

        cin.ignore();
        cout << "Enter the Resource ID : "; // resource id can be consist of letter and digit like "CS-111"
        getline(cin, id);

        bool found = false;

        for (int i = 0; i < book.size(); i++)
        {
            if (id == book[i].get_id())
            {
                found = true;
                cout << "Book exist at this ID" << endl;
                return;
            }
        }
        if (!found)
        {
            cout << "Enter the Title Name : ";
            getline(cin, title);

            while (true)
            {
                cout << "Enter the Author Name : ";
                getline(cin, author);
                if (checkfullname(author))
                {
                    break;
                }
                cout << "Invalid Username Format. Try again" << endl;
            }

            cout << "Enter the Category Type : ";
            getline(cin, category);

            cout << "Enter Availability (1 = Available, 0 = Not Available): ";
            availability = checkAvailability();

            RESOURCE r(id, title, author, category); // calling the constructor
            book.push_back(r);                       // this is used to store the data into the resource class
            cout << "Successfully Added" << endl;
        }
    }
    void removeResource(vector<RESOURCE> &book)
    {
        string id;
        cin.ignore();
        cout << "Enter the Resource ID : ";
        getline(cin, id);
        bool found = false;
        for (int i = 0; i < book.size(); i++)
        {
            if (id == book[i].get_id())
            {
                found = true;
                book.erase(book.begin() + i);
                cout << "Resource Successfully Deleted" << endl;
                return;
            }
        }
        if (!found)
        {
            cout << "Book Not Found" << endl;
        }
    }
    void upmenu()
    {
        cout << "1- Title Name " << endl;
        cout << "2- Author Name " << endl;
        cout << "3- Category Type " << endl;
        cout << "4- Availability " << endl;
        cout << "What do you want to Update??";
    }
    void updateResource(vector<RESOURCE> &book)
    {
        string id;
        cin.ignore();
        cout << "Enter the Resource ID : ";
        getline(cin, id);

        bool found = false;
        for (int i = 0; i < book.size(); i++)
        {
            if (id == book[i].get_id())
            {
                found = true;
                int choice;
                upmenu();
                choice = checkdigit();
                cin.ignore();

                if (choice == 1)
                {
                    string t;
                    cout << "Enter the Updated Title Name : ";
                    getline(cin, t);
                    book[i].set_title(t);
                    cout << "Title Name has been Updated" << endl;
                }

                else if (choice == 2)
                {
                    string a;
                    cout << "Enter the Updated Author Name : ";
                    getline(cin, a);
                    book[i].set_author(a);
                    cout << "Author Name has been Updated" << endl;
                }

                else if (choice == 3)
                {
                    string c;
                    cout << "Enter the Updated Category Type : ";
                    getline(cin, c);
                    book[i].set_category(c);
                    cout << "Category Type has been Updated" << endl;
                }

                else if (choice == 4)
                {
                    bool avail;
                    cout << "Enter the  Updated Availability : ";
                    avail = checkAvailability();
                    book[i].set_availability(avail);
                    cout << "Availability has been Updated" << endl;
                }
                break;
            }
        }
        if (!found)
        {
            throw booknotfound("Book Not Found");
        }
    }
    void viewResource(vector<RESOURCE> &book) // admin can sees the total books in the library
    {
        if (book.empty())
        {
            cout << "No Resource Available" << endl;
            return;
        }
        for (int i = 0; i < book.size(); i++)
        {
            book[i].display_resource();
            cout << endl;
        }
    }
    void viewIssuedBook(vector<BORROWRECORD> &record) // admin can see which book issued by whom
    {
        if (record.empty())
        {
            cout << "No Record Available" << endl;
            return;
        }

        for (int i = 0; i < record.size(); i++)
        {
            if (record[i].get_returndate() == "")
            {
                record[i].display_detail();
            }
        }
        for (int i = 0; i < record.size(); i++)
        {
            if (record[i].get_returndate() != "")
            {
                record[i].display_detail();
            }
        }
    }
    void viewrequest()
    {
        ifstream file("requests.txt");
        if (!file)
        {
            cout << "Error Opening File" << endl;
            return;
        }
        cout << "\n===== STUDENT BOOK REQUESTS =====" << endl;
        string line;
        int count = 1;

        while (getline(file, line))
        {
            cout << count++ << ". " << line << endl;
        }

        if (count == 1)
        {
            cout << "No Request Found" << endl;
            return;
        }
        file.close();
        cout << "===================================" << endl;
        cout << "Use 'Add Resource' option to add these books." << endl;
    }
    void acceptrequest(vector<RESOURCE> &book)
    {
        vector<string> requests;
        string line;
        ifstream file("requests.txt");
        if (!file)
        {
            cout << "File Not Opening" << endl;
            return;
        }

        else
        {
            while (getline(file, line))
            {
                requests.push_back(line);
            }
            file.close();
        }

        if (requests.empty())
        {
            cout << "All Request Accepted" << endl;
            return;
        }

        for (int i = 0; i < requests.size(); i++)
        {
            cout << i + 1 << ". " << requests[i] << endl;
        }

        int choice;
        cout << "Which Request Do you want to select : ";
        choice = checkdigit();
        if (choice < 1 || choice > (int)requests.size())
        {
            cout << "Invalid selection!" << endl;
            return;
        }

        cout << "Processing : " << requests[choice - 1] << endl;

        cin.ignore();
        string id, category, author, title;

        cout << "Enter Resouce ID : ";
        getline(cin, id);

        cout << "Enter the Title Name : ";
        getline(cin, title);

        while (true)
        {
            cout << "Enter the Author Name : ";
            getline(cin, author);
            if (checkfullname(author))
            {
                break;
            }
            cout << "Invalid FullName Format. Try again." << endl;
        }

        cout << "Enter the Category : ";
        getline(cin, category);

        RESOURCE r(id, title, author, category);
        book.push_back(r);
        cout << "Successfully the Book is being Updated!" << endl;

        ofstream outfile("requests.txt");
        {
            for (int i = 0; i < requests.size(); i++)
            {
                if (i != choice - 1)
                {
                    outfile << requests[i] << endl;
                }
            }
        }
        outfile.close();
    }
    void deleteUser(vector<USER> &members, vector<BORROWRECORD> &records)
    {
        string username;
        cin.ignore();
        while (true)
        {
            cout << "Enter Username to delete: ";
            getline(cin, username);
            if (checkusername(username))
            {
                break;
            }
            // throw userexception("Invalid Username Name, Please Use Digit and Letter");
            cout << "Invalid Username Name, Please Use Digit and Letter" << endl;
            // continue;
        }

        bool found = false;

        // Check if user has issued books
        for (int i = 0; i < records.size(); i++)
        {
            if (records[i].get_username() == username &&
                records[i].get_returndate() == "")
            {
                throw userexception("User has issued books. Cannot delete.");
            }
        }

        // Delete user
        for (int i = 0; i < members.size(); i++)
        {
            if (members[i].get_username() == username)
            {
                members.erase(members.begin() + i);
                found = true;
                cout << "User deleted successfully!" << endl;
                break;
            }
        }

        if (!found)
        {
            throw userexception("User not found!");
        }
        // ALSO delete all records of that user (history cleanup)
        records.erase(
            remove_if(records.begin(), records.end(),
                      [&](BORROWRECORD &r)
                      {
                          return r.get_username() == username;
                      }),
            records.end());

        cout << "All records of the user also removed." << endl;
    }
    void viewAllUsers(vector<USER> &members)
    {
        if (members.empty())
        {
            cout << "No Users Found in System" << endl;
            return;
        }

        cout << "\n========== REGISTERED USERS ==========\n";

        for (int i = 0; i < members.size(); i++)
        {
            cout << "User #" << i + 1 << endl;
            members[i].display();
            cout << "--------------------------------------" << endl;
        }

        cout << "Total Users: " << members.size() << endl;
    }
    void searchUser(vector<USER> &members, vector<BORROWRECORD> &records)
    {
        if (members.empty())
        {
            cout << "No users available in system." << endl;
            return;
        }

        cin.ignore();
        string username;

        while (true)
        {
            cout << "Enter username to search: ";
            getline(cin, username);
            if (checkusername(username))
            {
                break;
            }
            cout << "Invalid Username Name, Please Use Digit and Letter" << endl;
            // continue;
            // getline(cin, username);
        }

        bool user = false;

        for (int i = 0; i < members.size(); i++)
        {
            if (members[i].get_username() == username)
            {
                members[i].display();
                user = true;
                break;
            }
        }

        if (!user)
        {
            cout << "User not found!" << endl;
            return;
        }

        bool iss = false;
        bool ret = false;

        cout << "\n========== ACTIVE ISSUED BOOKS ==========\n";
        for (int i = 0; i < records.size(); i++)
        {
            if (records[i].get_username() == username && records[i].get_returndate() == "")
            {
                records[i].display_detail(); //  his borrowing records
                iss = true;
            }
        }
        if (!iss)
        {
            cout << "No Book is being Issued By You" << endl;
        }
        cout << "\n========== PAST RETURNED BOOKS ==========\n";
        for (int i = 0; i < records.size(); i++)
        {
            if (records[i].get_username() == username && records[i].get_returndate() != "")
            {
                records[i].display_detail(); //  his borrowing records
                ret = true;
            }
        }
        if (!ret)
        {
            cout << "No Book is being Returned by You" << endl;
        }

        cout << "===================================\n";
    }
};
