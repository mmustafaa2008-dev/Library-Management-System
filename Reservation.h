#pragma once
class RESERVATION
{
private:
    string username, resource_id, resource_title, reserve_date;

public:
    // ======================== SETTER ============================//
    void set_username(string u) { username = u; }
    void set_resource_id(string id) { resource_id = id; }
    void set_resource_title(string t) { resource_title = t; }
    void set_reserve_date(string d) { reserve_date = d; }

    // ===================== GETTER ===========================//

    string get_username() const { return username; }
    string get_resource_id() const { return resource_id; }
    string get_resource_title() const { return resource_title; }
    string get_reserve_date() const { return reserve_date; }

    RESERVATION() : username(""), resource_id(""), resource_title(""), reserve_date("") {}
    RESERVATION(string u, string id, string title, string date)
        : username(u), resource_id(id), resource_title(title), reserve_date(date) {}

    void display_reservation()
    {
        cout << "Resource ID    : " << resource_id << endl;
        cout << "Resource Title : " << resource_title << endl;
        cout << "Reserved On    : " << reserve_date << endl;
    }
};
