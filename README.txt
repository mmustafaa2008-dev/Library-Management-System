================================================================================
              LIBRARY MANAGEMENT SYSTEM — README FILE
              Object-Oriented Programming | C++ Console Application
================================================================================

--------------------------------------------------------------------------------
  PROJECT TITLE    : Library Management System
  LANGUAGE         : C++ (Standard: C++11 or later)
  PARADIGM         : Object-Oriented Programming (OOP)
  PLATFORM         : Windows (Console-based)
--------------------------------------------------------------------------------


================================================================================
                                  GROUP MEMBERS
================================================================================

  1.  Muhammad Mustafa  (CS-25072)  -- Section : B
  2.  Syeda Sameen Fatima (CS-25066)  -- Section : B
  3.  Muhammad Waheed ul Haq Siddique (CS-25070) -- Section : B


================================================================================
                                PROJECT OVERVIEW
================================================================================

  This is a console-based Library Management System developed in C++ as part
  of our Object-Oriented Programming course project. The system simulates a
  real-world library environment where users can borrow, return, and search
  for books, while an admin can manage the entire library inventory.

  The system is divided into two main domains:

    [1] USER DOMAIN   — For registered library members
    [2] ADMIN DOMAIN  — For the library administrator


================================================================================
                                 KEY FEATURES
================================================================================

  USER FEATURES:
  --------------
    - User Registration (Sign Up) with input validation
    - User Login (Sign In) with credential verification
    - Membership System (Basic / Silver / Gold)
    - Membership expiry and cancellation handling
    - Membership-based issue limits
    - Membership-based due dates
    - Issue a book according to membership plan
    - Return a book with automatic fine calculation
    - Membership discounts applied on overdue fines
    - Reserve unavailable books
    - Auto-issue reserved books when returned
    - Search for a book by Title, Author, or Resource ID
    - Search Online via Open Library API (by Title, Author, or ISBN)
    - Request a book from the Admin
    - Pay overdue fines from wallet balance
    - View currently issued and previously returned books
    - View and add wallet balance
    - Update Email and Password

  ================================================================================
                    MEMBERSHIP & RESERVATION SYSTEM
  ================================================================================

  The system supports three membership plans with different borrowing
  limits, due dates, and fine discounts.

+-----------------+-------------+-------------+-----------------------+
| Membership Type | Issue Limit | Due Date    | Fine Discount         |
+-----------------+-------------+-------------+-----------------------+
| Basic           | 2 Books     | 10 Days     | No Discount           |
+-----------------+-------------+-------------+-----------------------+
| Silver          | 5 Books     | 30 Days     | 20% Discount          |
+-----------------+-------------+-------------+-----------------------+
| Gold            | 7 Books     | 60 Days     | 50% Discount          |
+-----------------+-------------+-------------+-----------------------+

  MEMBERSHIP FEATURES:
  --------------------
    - Basic membership has Lifetime validity
    - Silver and Gold memberships remain active for 1 year
    - Expired or Cancelled memberships automatically switch
      to Basic membership limits
    - Membership status is checked automatically during login

  RESERVATION FEATURES:
  ---------------------
    - Users can reserve unavailable books
    - Reservation stores:
         > Username
         > Resource ID
         > Resource Title
         > Reservation Date
    - Reserved books can be automatically issued
      when returned
    - Reservation records are managed using
      the RESERVATION class

  ADMIN FEATURES:
    ---------------
      - Secure Admin Login (ID + Username + Password)
      - Add new books/resources to the library
      - Remove existing books/resources
      - Update book details (Title, Author, Category, Availability)
      - View all resources in the library
      - View all currently issued books
      - View all student book requests
      - Accept and process student requests

  SYSTEM FEATURES:
  ----------------

    - File persistence — data is saved and loaded across sessions
        > books.txt         — stores all library resources
        > users.txt         — stores registered user accounts
        > records.txt       — stores borrow/return records
        > requests.txt      — stores student book requests
        > reservations.txt  — stores reserved book records

    - Membership System:
        > Basic, Silver, and Gold membership plans
        > Membership expiry and cancellation handling
        > Membership-based issue limits and due dates

    - Reservation System:
        > Users can reserve unavailable books
        > Auto-issue functionality for reserved books

    - Fine Calculation System:
        > Base fine = Rs. 10/day
        > Silver members get 20% fine discount
        > Gold members get 50% fine discount
        > Basic member fine doubles if overdue exceeds 10 days

    - Input validation throughout
        > Username validation
        > Password strength validation
        > Email validation
        > Full name validation
        > Numeric input validation

    - Custom exception handling hierarchy
    - Automatic date handling using <ctime>
    - Open Library API integration for online search


================================================================================
                              HOW TO RUN THE PROJECT
================================================================================

  REQUIREMENTS:
  -------------
    - A C++ compiler (e.g., g++ via MinGW, or MSVC)
    - Windows OS (for the curl-based API feature)
    - Internet connection (optional — only needed for Online Book Search)

  STEPS TO COMPILE AND RUN:
  --------------------------

    Step 1: Open a terminal / command prompt in the project folder.

    Step 2: Compile using g++:
            g++ main.cpp

    Step 3: Run the executable:
            .\a.exe

    Step 4: The main menu will appear. Select your domain:
            1 = User Domain
            2 = Admin Domain
            3 = Exit


================================================================================
                              ADMIN LOGIN CREDENTIALS
================================================================================

    Admin ID   : 786
    Username   : admin
    Password   : Admin@123

  (These are hardcoded for demonstration purposes.)


================================================================================
                                INPUT VALIDATION RULES
================================================================================

    Username   : Minimum 3 characters, letters and digits only, no spaces
    Full Name  : At least 2 words, letters and spaces only (e.g. Ali Khan)
    Password   : Minimum 8 characters, must include 1 uppercase + 1 digit
    Email      : Must contain '@' and a domain (e.g. user@gmail.com)
    Balance    : Minimum initial deposit of Rs. 3000
    Numeric    : Custom functions prevent letters being entered as numbers


================================================================================
                                  OOP CONCEPTS USED
================================================================================

    - Classes and Objects :
        Implemented using the classes:
        person, USER, ADMIN, RESOURCE, BORROWRECORD, and LIBRARY

    - Inheritance :
        USER and ADMIN classes publicly inherit from the person class.

    - Polymorphism :
        Virtual function display() is declared in the person class
        and overridden in the USER and ADMIN classes.

    - Encapsulation :
        Data members in USER, ADMIN, RESOURCE, and BORROWRECORD
        classes are kept private and accessed through getter/setter
        member functions.

    - Constructors :
        Default, parameterized, and copy constructors are used in
        classes such as USER, RESOURCE, and BORROWRECORD.

    - Exception Handling :
        Custom exception hierarchy implemented using:
            > myexception        (base class)
            > userexception
            > adminexception
            > booknotfound
            > limitexceed
            > fineexception

    - File Handling :
        ifstream and ofstream are used in functions responsible for
        saving/loading data from:
            > books.txt
            > users.txt
            > records.txt
            > requests.txt

    - Vectors (STL) :
        vector containers are used in the LIBRARY class to dynamically
        store users, resources, borrow records, and requests.

    - API Integration :
        Open Library API is accessed using the system() function with
        curl commands inside the online search functionality.
         
    - Date & Time Handling :
      <ctime> library is used for:
         > Due date generation
         > Membership expiry checking
         > Fine calculation
         > Reservation dates



================================================================================
                              PROJECT FILE STRUCTURE
================================================================================

    main.cpp          — Full source code of the project
    books.txt         — Auto-generated; stores book records
    users.txt         — Auto-generated; stores user accounts
    records.txt       — Auto-generated; stores borrow/return history
    requests.txt      — Auto-generated; stores user book requests
    reservations.txt  — Auto-generated; stores reserved book records
    response.txt      — Temporary file used during API calls (auto-deleted)
    README.txt        — This file

================================================================================
                              FINE CALCULATION LOGIC
================================================================================

    The system automatically calculates fines when a book is
  returned after its due date.

  Base Formula:

      Fine = Overdue Days × Rs. 10

  Applied Membership Rules:

    - Gold Members:
         50% discount on total fine

    - Silver Members:
         20% discount on total fine

    - Basic Members:
         No discount applied
         If overdue exceeds 10 days,
         the fine becomes double

  Additional Rules:
  -----------------
    - Fine is generated automatically during return
    - Users with unpaid fines cannot issue new books
    - Fine payment deducts amount from wallet balance
    - Membership discounts are applied automatically



================================================================================
                             ONLINE BOOK SEARCH (API)
================================================================================

    - Powered by the Open Library API (https://openlibrary.org)
    - Search by: Title / Author / ISBN
    - Results show: Book Title, Author, Edition Count
    - Users can send a request to the Admin for any online book found
    - The Admin can then accept and add the book to the library

  NOTE: This feature requires an active internet connection and uses
        the system's built-in 'curl' command (available on Windows 10+).


================================================================================
                                KNOWN LIMITATIONS
================================================================================

    - Exact match search only: searchResource() requires the user to type
      the Title or Resource ID exactly as stored — it is case-sensitive
      and does not support partial or keyword-based searching.

    - Any user can return any book: returnResource() matches only by
      Resource ID, not by username. This means one user could accidentally
      return a book issued to a different user.

    - Passwords stored in plain text: user passwords are saved directly
      into users.txt with no encryption or hashing, which is a security
      concern in a real-world system.

    - No logout option in the user menu: once a user logs in, they can
      only exit by selecting option 10. There is no dedicated logout
      button that returns to the main menu cleanly.

    - addBalance() skips input validation: unlike other numeric inputs
      in the System

    - Manual JSON parsing for API: the Open Library API response is
      parsed using string search methods (find/substr), which may break
      if the API changes its response format or returns unexpected data.

    - Single admin account only: the system supports only one hardcoded
      admin. Multiple admins or admin registration is not supported.

    - Data stored in plain text files: no database is used, so data is
      vulnerable to manual editing and has no concurrency support.

================================================================================
                            LEARNING RESOURCES USED
================================================================================

  The following resources were referenced during the development of this project:

  1. GeeksForGeeks (https://www.geeksforgeeks.org)
       - OOP concepts in C++
       - File handling with fstream
       - Exception handling and custom exception classes
       - STL vectors and string operations
       - Date/time functions using <ctime>

  2. Code With Harry — YouTube Channel
     (https://www.youtube.com/@CodeWithHarry)
       - C++ programming tutorials (Beginner to Intermediate)
       - Practical examples of classes, inheritance, and constructors
       - Console-based project structure and design

  3. Teacher / Course Notes
       - OOP principles as taught in class
       - Project requirements and feature specifications
       - Guidance on class design, exception hierarchy, and file persistence


================================================================================
                                 DISCLAIMER
================================================================================

  This project is developed solely for academic/educational purposes as part
  of the Object-Oriented Programming course. All features are implemented
  using standard C++ libraries only (no third-party frameworks).
  Some advanced features such as database integration, password
  encryption, and multi-user concurrency were outside the scope
  of this academic project and may be implemented in future versions

================================================================================
                               END OF README
================================================================================






