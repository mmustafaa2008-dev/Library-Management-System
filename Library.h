#pragma once
class LIBRARY
{
private:
    vector<BORROWRECORD> records;
    vector<RESOURCE> book;
    vector<USER> member;
    string current_user;
    vector<RESERVATION> reservations;

public:
    bool isDateAfter(string date1, string date2)
    {
        int d1, m1, y1, d2, m2, y2;
        sscanf(date1.c_str(), "%d-%d-%d", &d1, &m1, &y1);
        sscanf(date2.c_str(), "%d-%d-%d", &d2, &m2, &y2);
        if (y1 > y2)
            return true;
        if (y1 == y2 && m1 > m2)
            return true;
        if (y1 == y2 && m1 == m2 && d1 > d2)
            return true;
        return false;
    }
    void checkMembershipExpiry()
    {
        for (int i = 0; i < member.size(); i++)
        {
            if (member[i].get_username() == current_user && member[i].get_memtype() != "basic")
            {
                if (isDateAfter(getTodayDate(), member[i].get_expiry()))
                {
                    member[i].set_status("Expired");
                    cout << "WARNING: Your membership has expired!" << endl;
                }
            }
        }
    }
    void membermenu()
    {
        cout << "=====================MEMBERSHIP DETAILS========================" << endl;
        cout << "1- BASIC PLAN      RS : 1000 | 2 Book Issue | 10 Days Due Date | 2 Reserved Book" << endl;
        cout << "2- SILVER PLAN     RS : 2000 | 5 Book Issue | 30 Days Due Date | 2 Reserved Book | 20% off on fine" << endl;
        cout << "3- GOLD PLAN       RS : 3000 | 7 Book Issue | 60 Days Due Date | 2 Reserved Book | 50% off on fine" << endl;
        cout << "Please Select The Above Option : ";
    }
    string getEffectiveMembership(const string &uname)
    {
        for (int i = 0; i < member.size(); i++)
        {
            if (member[i].get_username() == uname)
            {
                if (member[i].get_status() != "Active")
                    return "basic";
                return member[i].get_memtype();
            }
        }
        return "basic";
    }
    void fetchFromOpenLibrary(string query, string searchType)
    {
        string illegalChars = "&;|><$`";
        for (char c : illegalChars)
        {
            query.erase(remove(query.begin(), query.end(), c), query.end());
        }
        string url;
        string q = query;
        if (searchType == "isbn")
        {
            for (int i = 0; i < q.length(); i++)
            {
                if (q[i] == '-' || q[i] == ' ')
                {
                    q.erase(i, 1);
                    i--;
                }
            }
        }
        else
        {
            for (int i = 0; i < q.length(); i++)
            {
                if (q[i] == ' ')
                    q[i] = '+';
            }
        }
        // Build URL based on search type
        if (searchType == "title")
            url = "https://openlibrary.org/search.json?title=" + q + "&limit=3&fields=title,author_name,language,edition_count,first_publish_year";
        else if (searchType == "author")
            url = "https://openlibrary.org/search.json?author=" + q + "&limit=3&fields=title,author_name,language,edition_count,first_publish_year";
        else if (searchType == "isbn")
        {
            url = "https://openlibrary.org/api/books?bibkeys=ISBN:" + q + "&format=json&jscmd=data";
        }

        // Save result to a temp file using Windows built-in curl
        string command = "curl -s \"" + url + "\" -o response.txt";

        cout << "Connecting to Open Library..." << endl;
        int result = system(command.c_str());

        if (result != 0)
        {
            cout << "Internet connection failed!" << endl;
            return;
        }

        // Read and display the result
        ifstream file("response.txt");
        if (!file)
        {
            cout << "Could not read response." << endl;
            return;
        }

        string content = "";
        string line;

        while (getline(file, line))
        {
            content += line;
        }
        file.close();
        // =========  ISBN CASE =============== //
        if (searchType == "isbn")
        {
            cout << "\n===== ISBN RESULT =====\n";

            if (content == "{}")
            {
                cout << "No book found for this ISBN\n";
                return;
            }

            size_t titlePos = content.find("\"title\":");

            if (titlePos == string::npos)
            {
                cout << "book found but detail missing\n";
                return;
            }

            // extract title
            size_t tstart = content.find("\"", titlePos + 8) + 1;
            size_t tend = content.find("\"", tstart);
            string title = content.substr(tstart, tend - tstart);

            // extract author name (different format!)
            size_t authorPos = content.find("\"name\":");
            string author = "Unknown";

            if (authorPos != string::npos)
            {
                size_t astart = content.find("\"", authorPos + 7) + 1;
                size_t aend = content.find("\"", astart);
                author = content.substr(astart, aend - astart);
            }
            string edition = "N/A";

            size_t editionPos = content.find("\"edition_count\":");
            if (editionPos != string::npos)
            {
                size_t estart = content.find(":", editionPos) + 1;
                size_t eend = content.find(",", estart);
                edition = content.substr(estart, eend - estart);
            }

            cout << "Title   : " << title << endl;
            cout << "Author  : " << author << endl;
            cout << "Edition : " << edition << endl;

            char req;
            cout << "\nDo you want to REQUEST this book from Admin? (y/n): ";
            cin >> req;

            if (req == 'y' || req == 'Y')
            {
                ofstream file("requests.txt", ios::app);
                if (file)
                {
                    file << current_user << " requested: "
                         << title << ", " << author << endl;
                    file.close();

                    cout << "Request sent to Admin!" << endl;
                }
            }
            return; // IMPORTANT → skip old parsing
        }

        // Extract simple data
        cout << "\n===== CLEAN SEARCH RESULT =====\n";

        size_t pos = 0;
        int count = 0;
        string authors[5];
        string titles[5];
        string editions[5];

        while ((pos = content.find("\"title\":", pos)) != string::npos && count < 5)
        {
            // for title
            size_t start = content.find("\"", pos + 8) + 1;
            size_t end = content.find("\"", start);
            string title = content.substr(start, end - start);

            // for author
            size_t authorPos = content.find("\"author_name\":", end);
            size_t astart = content.find("\"", authorPos + 15) + 1;
            size_t aend = content.find("\"", astart);
            string author = content.substr(astart, aend - astart);

            // edition
            size_t editionPos = content.find("\"edition_count\":", end);
            size_t estart = content.find(":", editionPos) + 1;
            size_t eend = content.find(",", estart);
            string edition = content.substr(estart, eend - estart);

            // for language
            size_t languagePos = content.find("\"language\":", end);
            size_t lstart = content.find("\"", languagePos + 11) + 1;
            size_t lend = content.find("\"", lstart);
            string language = content.substr(lstart, lend - lstart);
            authors[count] = author;
            titles[count] = title;
            editions[count] = edition;

            cout << count + 1 << ". " << title << "| " << author << "| " << edition << endl;

            cout << "------------------------" << endl;

            pos = lend;
            count++;
        }
        int choice;
        cout << "Select Book Number : ";
        cin >> choice;

        choice--;
        string selectedtitle = titles[choice];
        string selectedauthor = authors[choice];
        string selectededition = editions[choice];

        char req;
        cout << "\nDo you want to REQUEST this book from Admin? (y/n): ";
        cin >> req;

        if (req == 'y' || req == 'Y')
        {
            ofstream file("requests.txt", ios::app);
            if (file)
            {
                file << current_user << " requested: "
                     << selectedtitle << ", "
                     << selectedauthor << ", "
                     << selectededition << endl;
                file.close();
                cout << "Request sent to Admin!" << endl;
                cout << "Once Admin adds it, you can issue it." << endl;
            }
        }

        // file.close();
        cout << "=================================" << endl;

        // Clean up temp file
        remove("response.txt");
    }
    bool unpaidfineblocked()
    {
        for (int i = 0; i < records.size(); i++)
        {
            if (records[i].get_username() == current_user)
            {
                if (records[i].get_returndate() != "")
                {
                    if (records[i].get_fine() >= 10 && records[i].get_paidfine() == false)
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }
    bool registerUser()
    {
        string username;
        string name;
        string email;
        string password;
        double bal;
        string membership_type;
        string expiry;
        string status;
        int choice;

        cin.ignore();
        while (true)
        {
            cout << "Enter Username: ";
            getline(cin, username);

            if (checkusername(username))
                break;
            cout << "Invalid Username!\n";
        }

        while (true)
        {
            cout << "Enter the Full Name : ";
            getline(cin, name);
            if (checkfullname(name))
            {
                break;
            }
            cout << "Invalid Full Name Format" << endl;
        }

        while (true)
        {
            displayPasswordMenu();
            cout << "Enter Password: ";
            getline(cin, password);

            if (checkpass(password))
            {
                break;
            }
            cout << "Weak Password! Please follow the requirements menu above" << endl;
        }

        while (true)
        {
            cout << "Enter Email: ";
            getline(cin, email);

            if (checkemail(email))
                break;
            cout << "Email must contain '@' and domain (example: user@gmail.com)" << endl;
        }

        while (true)
        {
            membermenu();
            choice = checkdigit();
            if (choice == 1)
            {
                membership_type = "basic";
                expiry = "Lifetime";
                status = "Active";
                cout << "Enter the Initial Balance (min Rs 1000): ";
                bal = checkDouble();
                if (bal < 1000)
                    throw userexception("Insufficient Balance! Initial Deposit must be at least Rs 1000");
                else
                    break;
            }
            else if (choice == 2)
            {
                membership_type = "silver";
                expiry = getFutureDate(365); // FIX: was getTodayDate() + getFutureDate(365) which just concatenates strings
                status = "Active";
                cout << "Enter the Initial Balance (min Rs 2000): ";
                bal = checkDouble();
                if (bal < 2000)
                    throw userexception("Insufficient Balance! Initial Deposit must be at least Rs 2000");
                else
                    break;
            }

            else if (choice == 3)
            {
                membership_type = "gold";    // FIX: was "silver" for gold plan
                expiry = getFutureDate(365); // FIX: same wrong string concat
                status = "Active";
                cout << "Enter the Initial Balance (min Rs 3000): ";
                bal = checkDouble();
                if (bal < 3000)
                    throw userexception("Insufficient Balance! Initial Deposit must be at least Rs 3000");
                else
                    break;
            }
            else
            {
                cout << "Invalid Plan Selected! Please Select 1-3" << endl;
            }
        }

        for (int i = 0; i < member.size(); i++)
        {
            if (username == member[i].get_username())
                throw userexception("User Already Exists!");
        }

        USER u(username, password, email, name, bal, membership_type, expiry, status);
        member.push_back(u);
        cout << "\n[SUCCESS] User registered successfully!" << endl;
        cout << "Membership  : " << membership_type << endl;
        if (membership_type != "basic")
            cout << "Valid Until : " << expiry << endl;
        return true;
    }
    int authenticUser()
    {
        string username;
        string password;

        cin.ignore();
        while (true)
        {
            cout << "Enter Username: ";
            getline(cin, username);

            if (checkusername(username))
                break;

            cout << "Invalid Username Format! Try again.\n";
        }
        while (true)
        {
            displayPasswordMenu();
            cout << "Enter Password: ";
            getline(cin, password);

            if (checkpass(password))
            {
                break;
            }
            cout << "Weak Password! Please follow the requirements menu above" << endl;
        }

        for (int i = 0; i < member.size(); i++)
        {
            if (username == member[i].get_username() && password == member[i].get_password())
            {
                current_user = username;
                cout << "\nLogin Successful!" << endl;
                cout << "Balance: Rs " << member[i].get_bal() << endl;

                // FIX: Check expiry on every login
                checkMembershipExpiry();
                return 0;
            }
        }
        cout << "Invalid Username or Password\n";
        return 1;
    }
    void payfine()
    {
        cin.ignore();
        string id;
        cout << "Enter Resource ID : ";
        getline(cin, id);

        bool found = false;
        for (int i = 0; i < records.size(); i++)
        {
            if (records[i].get_id() == id && records[i].get_username() == current_user)
            {
                found = true;
                double fine = records[i].get_fine();

                // FIX: use effective membership for discount
                string membership = getEffectiveMembership(current_user);

                if (membership == "gold" && fine > 0)
                {
                    fine *= 0.50;
                    cout << "Gold Membership Discount Applied! Fine reduced by 50%" << endl;
                }
                else if (membership == "silver" && fine > 0)
                {
                    fine *= 0.80; // 20% off means pay 80%
                    cout << "Silver Membership Discount Applied! Fine reduced by 20%" << endl;
                }

                if (fine >= 10 && records[i].get_paidfine() == false)
                {
                    for (int j = 0; j < member.size(); j++)
                    {
                        if (member[j].get_username() == current_user)
                        {
                            if (member[j].get_bal() < fine)
                            {
                                throw fineexception("Insufficient Balance to pay fine. Please add balance.");
                            }

                            member[j].set_bal(member[j].get_bal() - fine);
                            cout << "Fine Paid       : Rs " << fine << endl;
                            cout << "Remaining Bal   : Rs " << member[j].get_bal() << endl;
                            records[i].set_paidfine(true);
                            records[i].set_fine(0);
                            cout << "Fine Paid Successfully!" << endl;
                            return;
                        }
                    }
                }
                else if (records[i].get_paidfine() == true)
                    cout << "Fine is already paid for this book." << endl;
                else
                    cout << "No Fine Pending for this book." << endl;
                return;
            }
        }
        if (!found)
        {
            throw booknotfound("Record Not Found in the Library");
        }
    }
    void availableresource()
    {
        if (book.empty())
        {
            throw booknotfound("No Book Found In Library");
        }
        bool found = false;
        cout << "====================================Available Resource In Library=================================" << endl;
        for (int i = 0; i < book.size(); i++)
        {
            if (book[i].get_isavailable() == true)
            {
                found = true;
                book[i].display_resource();
                cout << endl;
            }
        }
        if (found == false)
        {
            throw booknotfound("No Book is available");
        }
    }
    void issueResource()
    {
        string membership = getEffectiveMembership(current_user);

        int issueLimit, dueDays;
        if (membership == "basic")
        {
            issueLimit = 2;
            dueDays = 10;
        }
        else if (membership == "silver")
        {
            issueLimit = 5;
            dueDays = 30;
        }
        else if (membership == "gold")
        {
            issueLimit = 7;
            dueDays = 60;
        }
        else
        {
            issueLimit = 2;
            dueDays = 10;
        }

        int count = 0;
        for (int i = 0; i < records.size(); i++)
        {
            if (records[i].get_username() == current_user && records[i].get_returndate() == "")
            {
                count++;
            }
            if (records[i].get_fine() >= 10 && records[i].get_paidfine() == false)
                throw fineexception("You have an unpaid fine. Please pay it to issue a book.");
        }

        if (count >= issueLimit)
            throw limitexceed("Issue limit exceeded! (" + to_string(issueLimit) +
                              " books for " + membership + " plan)");

        if (unpaidfineblocked())
            throw fineexception("You cannot issue books until your fine is paid.");

        cin.ignore();
        string id = "", author = "", title = "";
        int choice;

        cout << "1- Search by Author" << endl;
        cout << "2- Search by Title" << endl;
        cout << "3- Search by Resource ID" << endl;
        cout << "Enter Choice(1-3) : ";
        choice = checkdigit();
        cin.ignore();

        if (choice == 1)
        {
            while (true)
            {
                cout << "Enter the Full Name : ";
                getline(cin, author);
                if (checkfullname(author))
                {
                    break;
                }
                cout << "Invalid Full Name Format" << endl;
            }
        }
        else if (choice == 2)
        {
            cout << "Enter the Title Name : ";
            getline(cin, title);
        }
        else
        {
            cout << "Enter the Resource ID : ";
            getline(cin, id);
        }

        bool found = false;
        for (int i = 0; i < book.size(); i++)
        {
            if ((id != "" && id == book[i].get_id()) ||
                (title != "" && title == book[i].get_title()) ||
                (author != "" && author == book[i].get_author()))
            {
                found = true;
                if (book[i].get_isavailable())
                {
                    string actual_id = book[i].get_id();
                    string issue = getTodayDate();
                    string due = getFutureDate(dueDays); // FIX: now uses membership-based due days

                    cout << "Issue Date  : " << issue << endl;
                    cout << "Due Date    : " << due << endl;
                    cout << "Resource ID : " << actual_id << endl;

                    book[i].mark_issued();
                    BORROWRECORD br(actual_id, current_user, issue, due);
                    records.push_back(br);
                    cout << "Book Successfully Issued!" << endl;
                }
                else
                    throw booknotfound("Book is not available right now.");
                break;
            }
        }
        if (!found)
            throw booknotfound("Book Not Found in Library");
    }
    void returnResource()
    {
        string id;
        string returnd;
        cin.ignore();
        cout << "Enter the Resource ID : ";
        getline(cin, id);
        bool found = false;
        for (int i = 0; i < records.size(); i++)
        {
            if (id == records[i].get_id() && records[i].get_username() == current_user && records[i].get_returndate() == "")
            {
                found = true;
                string returnd = getTodayDate();
                records[i].set_returndate(returnd);
                cout << "Return Date : " << returnd << endl;

                string membership = getEffectiveMembership(current_user);
                float f = records[i].calculatefine(membership);

                records[i].set_fine(f);
                records[i].set_paidfine(false);

                for (int j = 0; j < book.size(); j++)
                {
                    if (id == book[j].get_id())
                    {
                        book[j].mark_returned();
                        autoIssueReserved(id);
                        autoIssueForUser(current_user);
                        break;
                    }
                }

                records[i].display_detail();

                if (f > 0)
                {
                    saveBooks();
                    saveRecords();
                    throw fineexception("Book returned late! Fine of Rs." +
                                        to_string((int)f) + " has been generated. Please pay it.");
                }

                cout << "Resource Returned Successfully" << endl;
                return;
            }
        }

        if (!found)
        {
            throw booknotfound("Record Not Found");
        }
    }
    void searchResource()
    {
        string title;
        string resourceid;

        cin.ignore();
        cout << "Enter the Title : ";
        getline(cin, title);

        cout << "Enter the Resource ID : ";
        getline(cin, resourceid);
        bool found = false;
        for (int i = 0; i < book.size(); i++)
        {
            if (title == book[i].get_title() || resourceid == book[i].get_id())
            {
                book[i].display_resource();
                found = true;
                break;
            }
        }
        if (found == false)
        {
            throw booknotfound("Book Not Found");
        }
    }
    void viewbalance()
    {
        bool found = false;

        for (int i = 0; i < member.size(); i++)
        {
            if (member[i].get_username() == current_user)
            {
                member[i].showBalance();
                found = true;
                break;
            }
        }

        if (!found)
        {
            cout << "User Not Found" << endl;
            return;
        }
    }
    void addBalance()
    {
        double amount;

        cout << "Enter amount to add: ";
        amount = checkDouble();

        if (amount <= 0)
        {
            cout << "Invalid amount!" << endl;
            return;
        }

        for (int i = 0; i < member.size(); i++)
        {
            if (member[i].get_username() == current_user)
            {
                double newBalance = member[i].get_bal() + amount;
                member[i].set_bal(newBalance);

                cout << "Balance Added Successfully!" << endl;
                cout << "New Balance: " << newBalance << endl;
                return;
            }
        }
        cout << "User not found!" << endl;
        return;
    }
    void updateuser()
    {
        int choice;
        bool found = false;
        cout << "1- Update Email Id : " << endl;
        cout << "2- Update Password : " << endl;
        cout << "Please Select The Above Option : ";

        choice = checkdigit();

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1)
        {
            string olde;
            while (true)
            {
                cout << "Enter Old Email ID: ";
                getline(cin, olde);

                if (checkemail(olde))
                    break;

                cout << "Email must contain '@' and domain (example: user@gmail.com) " << endl;
            }
            for (int i = 0; i < member.size(); i++)
            {
                if (member[i].get_username() == current_user)
                {
                    if (member[i].get_email() != olde)
                    {
                        throw userexception("Incorrect Old Email ID");
                    }
                    found = true;
                    string email;
                    while (true)
                    {
                        cout << "Enter Your Email ID : ";
                        getline(cin, email);
                        if (checkemail(email))
                        {
                            break;
                        }
                        cout << "Email must contain '@' and domain (example: user@gmail.com)" << endl;
                    }
                    member[i].set_email(email);
                    saveUsers();
                    cout << "Email Updated!" << endl;
                    break;
                }
            }
            if (!found)
            {
                throw userexception("User Not Found");
            }
        }
        else if (choice == 2)
        {
            string oldp;
            while (true)
            {
                cout << "Enter Old Password: ";
                getline(cin, oldp);

                if (checkpass(oldp))
                    break;

                cout << "Invalid Password Format! Try again.\n";
            }
            for (int i = 0; i < member.size(); i++)
            {
                if (member[i].get_username() == current_user)
                {
                    if (member[i].get_password() != oldp)
                    {
                        throw userexception("Incorrect Old Password");
                    }
                    found = true;
                    string password;
                    while (true)
                    {
                        cout << "Enter New Password: ";
                        getline(cin, password);

                        if (checkpass(password))
                            break;

                        cout << "Invalid Password Format! Try again.\n";
                    }
                    member[i].set_password(password);
                    cout << "Password Updated!" << endl;
                    break;
                }
            }
            if (!found)
            {
                throw userexception("User Not Found");
            }
        }
        else
        {
            cout << "Please Select the Correct Option!!" << endl;
            return;
        }
    }
    void mybook()
    {
        if (records.empty())
        {
            cout << "No Book Found" << endl;
            return;
        }
        bool iss = false;
        bool ret = false;
        cout << "===============Issued Books =================" << endl;
        for (int i = 0; i < records.size(); i++)
        {
            if (records[i].get_username() == current_user && records[i].get_returndate() == "")
            {
                records[i].display_detail();
                iss = true;
            }
        }
        if (!iss)
        {
            cout << "No Book is being Issued By You" << endl;
        }

        cout << "================Returned Books ===============" << endl;
        for (int i = 0; i < records.size(); i++)
        {
            if (records[i].get_username() == current_user && records[i].get_returndate() != "")
            {
                records[i].display_detail();
                ret = true;
            }
        }
        if (!ret)
        {
            cout << "No Book is being Returned by You" << endl;
        }
    }
    void searchandRequestBook()
    {
        int apichoice;
        string query, type;

        cout << "SEARCH ONLINE BOOK (OPEN LIBRARY API)" << endl;
        cout << "1- Search by TITLE" << endl;
        cout << "2- Search by AUTHOR" << endl;
        cout << "3- Search by ISBN " << endl;
        cout << "Select the Above Option : ";
        apichoice = checkdigit();

        cin.ignore();

        if (apichoice == 1)
        {
            cout << "Enter Title Name : ";
            getline(cin, query);
            type = "title";
        }
        else if (apichoice == 2)
        {
            while (true)
            {
                cout << "Enter the Full Name : ";
                getline(cin, query);
                if (checkfullname(query))
                {
                    break;
                }
                cout << "Invalid Full Name Format" << endl;
            }
            type = "author";
        }
        else if (apichoice == 3)
        {
            cout << "Enter ISBN Number: ";
            getline(cin, query);
            type = "isbn";
        }
        else
        {
            cout << "Invalid Option" << endl;
            return;
        }

        fetchFromOpenLibrary(query, type);
    }
    void saveBooks()
    {
        ofstream file("books.txt");
        if (!file)
        {
            cout << "Error File Not Opening" << endl;
            return;
        }

        for (int i = 0; i < book.size(); i++)
        {
            file << book[i].get_id() << ","
                 << book[i].get_title() << ","
                 << book[i].get_author() << ","
                 << book[i].get_category() << ","
                 << book[i].get_isavailable() << endl;
        }

        file.close();
    }
    void loadBooks()
    {
        ifstream file("books.txt");

        if (!file)
        {
            return;
        }

        string id, title, author, category;
        bool availability;

        while (getline(file, id, ',') &&
               getline(file, title, ',') &&
               getline(file, author, ',') &&
               getline(file, category, ',') &&
               file >> availability)
        {
            file.ignore(); // skip newline

            RESOURCE r(id, title, author, category); // constructor called
            r.set_availability(availability);

            book.push_back(r);
        }

        file.close();
    }
    void saveUsers()
    {
        ofstream file("users.txt");
        if (!file)
        {
            cout << "Error File Not Opening" << endl;
            return;
            //  for safe side if the file not open then it return to the main menu
        }
        for (int i = 0; i < member.size(); i++)
        {
            file << member[i].get_username() << ","
                 << member[i].get_name() << ","
                 << member[i].get_password() << ","
                 << member[i].get_email() << ","
                 << member[i].get_memtype() << ","
                 << member[i].get_expiry() << ","
                 << member[i].get_status() << ","
                 << member[i].get_bal() << endl;
        }

        file.close();
    }
    void loadUsers()
    {
        ifstream file("users.txt");
        if (!file)
        {
            return;
        }
        string username, name, password, email, membership_type, expiry, status;
        double balance;

        while (getline(file, username, ',') &&
               getline(file, name, ',') &&
               getline(file, password, ',') &&
               getline(file, email, ',') &&
               getline(file, membership_type, ',') &&
               getline(file, expiry, ',') &&
               getline(file, status, ',') &&
               file >> balance)
        {
            file.ignore(numeric_limits<streamsize>::max(), '\n');
            USER u(username, password, email, name, balance, membership_type, expiry, status);
            member.push_back(u);
        }

        file.close();
    }
    void saveRecords()
    {
        ofstream file("records.txt");

        if (!file)
        {
            cout << "Error opening records file!" << endl;
            return;
        }

        for (int i = 0; i < records.size(); i++)
        {
            file << records[i].get_username() << ","
                 << records[i].get_id() << ","
                 << records[i].get_issuedate() << ","
                 << records[i].get_duedate() << ","
                 << records[i].get_returndate() << ","
                 << records[i].get_fine() << ","
                 << records[i].get_paidfine() << endl;
        }

        file.close();
    }
    void loadRecords()
    {
        ifstream file("records.txt");

        if (!file)
            return;

        string username, id, issue, due, ret;
        double fine;
        bool paid;

        while (getline(file, username, ',') &&
               getline(file, id, ',') &&
               getline(file, issue, ',') &&
               getline(file, due, ',') &&
               getline(file, ret, ',') &&
               file >> fine &&
               file.ignore() &&
               file >> paid)
        {
            file.ignore(); // skip newline

            BORROWRECORD br;
            br.set_username(username);
            br.set_id(id);
            br.set_issuedate(issue);
            br.set_duedate(due);
            br.set_returndate(ret);
            br.set_fine(fine);
            br.set_paidfine(paid);

            records.push_back(br);
        }

        file.close();
    }
    vector<RESOURCE> &getBooks()
    {
        return book;
    }
    vector<BORROWRECORD> &getRecords()
    {
        return records;
    }
    string get_user()
    {
        return current_user;
    }
    vector<USER> &getMembers()
    {
        return member;
    }
    void reserveResource()
    {
        if (unpaidfineblocked())
            throw fineexception("Pay your pending fine before reserving a book");

        int reserveCount = 0;
        for (int i = 0; i < reservations.size(); i++)
            if (reservations[i].get_username() == current_user)
                reserveCount++;

        if (reserveCount >= 2)
        {
            cout << "\n[DENIED] Reservation limit reached (2/2)." << endl;
            cout << "Cancel an existing reservation before making a new one." << endl;
            return;
        }

        int issuedCount = 0;
        for (int i = 0; i < records.size(); i++)
            if (records[i].get_username() == current_user && records[i].get_returndate() == "")
            {
                issuedCount++;
            }

        cin.ignore();
        string id;
        cout << "Enter the Resource ID to Reserve : ";
        getline(cin, id);

        bool bookFound = false;
        for (int i = 0; i < book.size(); i++)
        {
            if (book[i].get_id() == id)
            {
                bookFound = true;

                for (int j = 0; j < reservations.size(); j++)
                {
                    if (reservations[j].get_username() == current_user &&
                        reservations[j].get_resource_id() == id)
                    {
                        cout << "You have already reserved this book!" << endl;
                        return;
                    }
                }

                if (issuedCount >= 2)
                {
                    RESERVATION r(current_user, id, book[i].get_title(), getTodayDate());
                    reservations.push_back(r);
                    cout << "\n[RESERVED] Issue limit reached (2/2)." << endl;
                    cout << "Will be auto-issued once you return a book." << endl;
                    return;
                }

                if (!book[i].get_isavailable())
                {
                    RESERVATION r(current_user, id, book[i].get_title(), getTodayDate());
                    reservations.push_back(r);
                    cout << "\n[RESERVED] Book is currently unavailable." << endl;
                    cout << "Will be auto-issued once returned by current holder." << endl;
                    return;
                }

                cout << "\nBook is AVAILABLE and you are within your limit." << endl;
                cout << "Please use 'Issue Resource' instead." << endl;
                return;
            }
        }
        if (!bookFound)
            throw booknotfound("Book Not Found in Library");
    }
    void autoIssueReserved(const string &resource_id)
    {
        for (int i = 0; i < reservations.size(); i++)
        {
            if (reservations[i].get_resource_id() == resource_id)
            {
                string res_user = reservations[i].get_username();
                int count = 0;
                for (int j = 0; j < records.size(); j++)
                    if (records[j].get_username() == res_user && records[j].get_returndate() == "")
                        count++;

                if (count < 2)
                {
                    string memtype = getEffectiveMembership(res_user);

                    int dueDays = (memtype == "gold") ? 60 : (memtype == "silver") ? 30
                                                                                   : 10;

                    string issue = getTodayDate();
                    string due = getFutureDate(dueDays);
                    BORROWRECORD br(resource_id, res_user, issue, due);
                    records.push_back(br);
                    for (int j = 0; j < book.size(); j++)
                    {
                        if (book[j].get_id() == resource_id)
                        {
                            book[j].mark_issued();
                            break;
                        }
                    }
                    cout << "\n========================================" << endl;
                    cout << "[AUTO-ISSUED] '" << resource_id << "' issued to '" << res_user << "'" << endl;
                    cout << "Issue Date : " << issue << "  |  Due Date : " << due << endl;
                    cout << "========================================\n"
                         << endl;
                    reservations.erase(reservations.begin() + i);
                    return;
                }
            }
        }
    }
    void viewMyReservations()
    {
        bool found = false;
        cout << "\n========== YOUR ACTIVE RESERVATIONS ==========" << endl;
        for (int i = 0; i < reservations.size(); i++)
        {
            if (reservations[i].get_username() == current_user)
            {
                reservations[i].display_reservation();
                cout << "----------------------------------------------" << endl;
                found = true;
            }
        }
        if (!found)
            cout << "No Active Reservations Found." << endl;
        cout << "================================================" << endl;
    }
    void cancelReservation()
    {
        cin.ignore();
        string id;
        cout << "Enter Resource ID to Cancel Reservation : ";
        getline(cin, id);
        for (int i = 0; i < reservations.size(); i++)
        {
            if (reservations[i].get_username() == current_user &&
                reservations[i].get_resource_id() == id)
            {
                reservations.erase(reservations.begin() + i);
                cout << "Reservation Cancelled Successfully!" << endl;
                return;
            }
        }
        cout << "No reservation found for this Resource ID." << endl;
    }
    void saveReservations()
    {
        ofstream file("reservations.txt");
        if (!file)
        {
            cout << "Error opening reservations file!" << endl;
            return;
        }
        for (int i = 0; i < reservations.size(); i++)
        {
            file << reservations[i].get_username() << ","
                 << reservations[i].get_resource_id() << ","
                 << reservations[i].get_resource_title() << ","
                 << reservations[i].get_reserve_date() << endl;
        }
        file.close();
    }
    void loadReservations()
    {
        ifstream file("reservations.txt");
        if (!file)
            return;
        string username, resource_id, resource_title, reserve_date;
        while (getline(file, username, ',') &&
               getline(file, resource_id, ',') &&
               getline(file, resource_title, ',') &&
               getline(file, reserve_date))
        {
            RESERVATION r(username, resource_id, resource_title, reserve_date);
            reservations.push_back(r);
        }
        file.close();
    }
    void autoIssueForUser(const string &username)
    {
        int count = 0;
        for (int i = 0; i < records.size(); i++)
            if (records[i].get_username() == username && records[i].get_returndate() == "")
                count++;

        if (count >= 2)
            return;

        for (int i = 0; i < reservations.size(); i++)
        {
            if (reservations[i].get_username() == username)
            {
                string res_id = reservations[i].get_resource_id();

                for (int j = 0; j < book.size(); j++)
                {
                    if (book[j].get_id() == res_id && book[j].get_isavailable())
                    {
                        string memtype = getEffectiveMembership(username);
                        int dueDays;

                        if (memtype == "gold")
                        {
                            dueDays = 60;
                        }
                        else if (memtype == "silver")
                        {
                            dueDays = 30;
                        }
                        else
                        {
                            dueDays = 10;
                        }
                        string issue = getTodayDate();
                        string due = getFutureDate(dueDays);

                        BORROWRECORD br(res_id, username, issue, due);
                        records.push_back(br);
                        book[j].mark_issued();

                        cout << "\n========================================" << endl;
                        cout << "[AUTO-ISSUED] Your reserved book '" << res_id
                             << "' has been automatically issued to you!" << endl;
                        cout << "Issue Date : " << issue << "  |  Due Date : " << due << endl;
                        cout << "========================================\n"
                             << endl;

                        reservations.erase(reservations.begin() + i);
                        return;
                    }
                }
            }
        }
    }
    void renewmembership()
    {
        for (int i = 0; i < member.size(); i++)
        {
            if (member[i].get_username() == current_user)
            {
                string memtype = member[i].get_memtype();
                string status = member[i].get_status();
                string expiry = member[i].get_expiry();

                if (memtype == "basic")
                {
                    throw membershipexception("Basic plan is lifetime. No renewal needed.");
                }

                if (status == "Cancelled")
                {
                    throw membershipexception("Cancelled membership cannot be renewed. Please contact admin.");
                }
                string today = getTodayDate();
                if (status == "Active" && !isDateAfter(today, expiry))
                {
                    cout << "Membership already active till " << expiry << endl;
                    cout << "You cannot renew before expiry.\n";
                    return;
                }

                double fee = (memtype == "silver") ? 2000.0 : 3000.0;

                cout << "\n========== RENEW MEMBERSHIP ==========" << endl;
                cout << "Plan           : " << memtype << endl;
                cout << "Current Expiry : " << expiry << endl;
                cout << "Current Status : " << status << endl;
                cout << "Renewal Fee    : Rs " << fee << endl;
                cout << "Your Balance   : Rs " << member[i].get_bal() << endl;
                cout << "=======================================" << endl;
                cout << "Confirm Renewal? (y/n): ";
                char confirm;
                cin >> confirm;
                if (confirm != 'y' && confirm != 'Y')
                {
                    cout << "Renewal Cancelled." << endl;
                    return;
                }

                if (member[i].get_bal() < fee)
                {
                    throw membershipexception("Insufficient Balance to renew membership. Please add balance first.");
                }

                member[i].set_bal(member[i].get_bal() - fee);

                string newExpiry;

                newExpiry = getFutureDate(365);

                member[i].set_expiry(newExpiry);
                member[i].set_status("Active");

                cout << "\n[SUCCESS] Membership Renewed!" << endl;
                cout << "New Expiry     : " << newExpiry << endl;
                cout << "Remaining Bal  : Rs " << member[i].get_bal() << endl;
                return;
            }
        }
        throw userexception("User not found!");
    }
    void cancelmembership()
    {
        for (int i = 0; i < member.size(); i++)
        {
            if (member[i].get_username() == current_user)
            {
                string memtype = member[i].get_memtype();
                string status = member[i].get_status();

                if (memtype == "basic")
                {
                    throw membershipexception("Basic plan cannot be cancelled. It is your default plan.");
                }
                if (status == "Cancelled")
                {
                    throw membershipexception("Your membership is already cancelled.");
                }

                // Check if user has more books than basic limit (2)
                int issuedCount = 0;
                for (int j = 0; j < records.size(); j++)
                    if (records[j].get_username() == current_user && records[j].get_returndate() == "")
                        issuedCount++;

                if (issuedCount > 2)
                {
                    throw membershipexception(
                        "You have " + to_string(issuedCount) +
                        " books issued. Please return books until you have 2 or fewer before cancelling.");
                }

                cout << "\n========== CANCEL MEMBERSHIP ==========" << endl;
                cout << "Plan    : " << memtype << endl;
                cout << "Expiry  : " << member[i].get_expiry() << endl;
                cout << "WARNING : You will be downgraded to Basic plan." << endl;
                cout << "NOTE    : No refund will be provided." << endl;
                cout << "Confirm Cancellation? (y/n): ";
                char confirm;
                cin >> confirm;
                if (confirm != 'y' && confirm != 'Y')
                {
                    cout << "Cancellation Aborted." << endl;
                    return;
                }

                member[i].set_status("Cancelled");
                cout << "\n[CANCELLED] Your " << memtype << " membership has been cancelled." << endl;
                cout << "You are now on Basic plan limits." << endl;
                return;
            }
        }
        throw userexception("User not found!");
    }
};