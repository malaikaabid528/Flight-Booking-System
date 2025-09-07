#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Flight {
    string flightNumber;
    string destination;
    int seats;
};

struct Passenger {
    string name;
    string flightNumber;
};

// Admin Login
bool adminLogin() {
    string user, pass;
    cout << "\n--- Admin Login ---\n";
    cout << "Username: ";
    cin >> user;
    cout << "Password: ";
    cin >> pass;
    return (user == "admin" && pass == "1234"); // Default credentials
}

// Add flights
void addFlight() {
    if (!adminLogin()) {
        cout << "Invalid login! Access denied.\n";
        return;
    }
    ofstream fout("flights.txt", ios::app);
    Flight f;
    cout << "\nEnter Flight Number: ";
    cin >> f.flightNumber;
    cout << "Enter Destination: ";
    cin >> f.destination;
    cout << "Enter Total Seats: ";
    cin >> f.seats;
    fout << f.flightNumber << " " << f.destination << " " << f.seats << "\n";
    fout.close();
    cout << "Flight Added Successfully!\n";
}

// Function to display flights
void displayFlights() {
    ifstream fin("flights.txt");
    Flight f;
    cout << "\nAvailable Flights:\n";
    cout << "-------------------------------------------\n";
    cout << "Flight No    Destination    Seats\n";
    cout << "-------------------------------------------\n";
    while (fin >> f.flightNumber >> f.destination >> f.seats) {
        cout << f.flightNumber << "        " << f.destination << "        " << f.seats << "\n";
    }
    cout << "-------------------------------------------\n";
    fin.close();
}

// Search by destination
void searchFlight() {
    string dest;
    cout << "\nEnter destination to search: ";
    cin >> dest;
    ifstream fin("flights.txt");
    Flight f;
    bool found = false;
    cout << "\nFlights to " << dest << ":\n";
    cout << "-------------------------------------------\n";
    cout << "Flight No    Destination    Seats\n";
    cout << "-------------------------------------------\n";
    while (fin >> f.flightNumber >> f.destination >> f.seats) {
        if (f.destination == dest) {
            cout << f.flightNumber << "        " << f.destination << "        " << f.seats << "\n";
            found = true;
        }
    }
    cout << "-------------------------------------------\n";
    fin.close();
    if (!found) cout << "No flights found for this destination.\n";
}

// Book flight
void bookFlight() {
    displayFlights();
    string flightNum;
    cout << "\nEnter Flight Number to Book: ";
    cin >> flightNum;

    ifstream fin("flights.txt");
    Flight flights[100];
    int count = 0;
    bool found = false;

    while (fin >> flights[count].flightNumber >> flights[count].destination >> flights[count].seats) {
        if (flights[count].flightNumber == flightNum && flights[count].seats > 0) {
            flights[count].seats--;
            found = true;
        }
        count++;
    }
    fin.close();

    if (found) {
        ofstream fout("flights.txt");
        for (int i = 0; i < count; i++) {
            fout << flights[i].flightNumber << " " << flights[i].destination << " " << flights[i].seats << "\n";
        }
        fout.close();

        Passenger p;
        cout << "Enter Passenger Name: ";
        cin >> p.name;
        p.flightNumber = flightNum;

        ofstream pout("passengers.txt", ios::app);
        pout << p.name << " " << p.flightNumber << "\n";
        pout.close();

        cout << "Booking Successful!\n";
    } else {
        cout << "Flight not found or no seats available.\n";
    }
}

// Cancel booking
void cancelBooking() {
    string passengerName;
    cout << "\nEnter Passenger Name to Cancel Booking: ";
    cin >> passengerName;

    ifstream pin("passengers.txt");
    Passenger passengers[100];
    int passCount = 0;
    string cancelFlight;
    bool found = false;

    while (pin >> passengers[passCount].name >> passengers[passCount].flightNumber) {
        if (passengers[passCount].name == passengerName) {
            cancelFlight = passengers[passCount].flightNumber;
            found = true;
        } else {
            passCount++;
        }
    }
    pin.close();

    if (found) {
        ofstream pout("passengers.txt");
        for (int i = 0; i < passCount; i++) {
            pout << passengers[i].name << " " << passengers[i].flightNumber << "\n";
        }
        pout.close();

        // Update flight seats
        ifstream fin("flights.txt");
        Flight flights[100];
        int count = 0;
        while (fin >> flights[count].flightNumber >> flights[count].destination >> flights[count].seats) {
            if (flights[count].flightNumber == cancelFlight) {
                flights[count].seats++;
            }
            count++;
        }
        fin.close();

        ofstream fout("flights.txt");
        for (int i = 0; i < count; i++) {
            fout << flights[i].flightNumber << " " << flights[i].destination << " " << flights[i].seats << "\n";
        }
        fout.close();

        cout << "Booking Cancelled Successfully!\n";
    } else {
        cout << "Passenger not found.\n";
    }
}

int main() {
    int choice;
    do {
        cout << "\n===== Flight Booking System =====\n";
        cout << "1. Add Flight (Admin)\n";
        cout << "2. View Flights\n";
        cout << "3. Search Flight by Destination\n";
        cout << "4. Book Flight\n";
        cout << "5. Cancel Booking\n";
        cout << "6. Exit\n";
        cout << "Enter Choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addFlight(); break;
            case 2: displayFlights(); break;
            case 3: searchFlight(); break;
            case 4: bookFlight(); break;
            case 5: cancelBooking(); break;
            case 6: cout << "Exiting...\n"; break;
            default: cout << "Invalid Choice!\n";
        }
    } while (choice != 6);

    return 0; 
}
