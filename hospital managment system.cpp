
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

// Forward declarations
class Patient;
class Doctor;
class Appointment;

// Utility functions
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pressEnterToContinue() {
    cout << "\nPress Enter to continue...";
    cin.ignore(1000, '\n');
    cin.get();
}

// --- Classes ---

class Person {
protected:
    int id;
    string name;
    int age;
    string gender;
    string phone;

public:
    Person() : id(0), age(0) {}
    virtual void inputDetails() {
        cout << "Enter ID: "; cin >> id;
        cin.ignore();
        cout << "Enter Name: "; getline(cin, name);
        cout << "Enter Age: "; cin >> age;
        cin.ignore();
        cout << "Enter Gender: "; getline(cin, gender);
        cout << "Enter Phone: "; getline(cin, phone);
    }
    virtual void displayDetails() const {
        cout << left << setw(10) << id << setw(20) << name << setw(10) << age << setw(10) << gender << setw(15) << phone;
    }
    int getId() const { return id; }
    string getName() const { return name; }
};

class Patient : public Person {
    string disease;
public:
    void inputDetails() override {
        Person::inputDetails();
        cout << "Enter Disease/Medical History: "; getline(cin, disease);
    }
    void displayDetails() const override {
        Person::displayDetails();
        cout << setw(20) << disease << endl;
    }
    string getDisease() const { return disease; }
    
    // File handling helpers
    void saveToFile(ofstream &out) const {
        out << id << "|" << name << "|" << age << "|" << gender << "|" << phone << "|" << disease << endl;
    }
    static Patient loadFromFile(string line) {
        Patient p;
        size_t pos = 0;
        string token;
        vector<string> tokens;
        while ((pos = line.find("|")) != string::npos) {
            token = line.substr(0, pos);
            tokens.push_back(token);
            line.erase(0, pos + 1);
        }
        tokens.push_back(line);
        if (tokens.size() >= 6) {
            p.id = stoi(tokens[0]);
            p.name = tokens[1];
            p.age = stoi(tokens[2]);
            p.gender = tokens[3];
            p.phone = tokens[4];
            p.disease = tokens[5];
        }
        return p;
    }
};

class Doctor : public Person {
    string specialization;
public:
    void inputDetails() override {
        Person::inputDetails();
        cout << "Enter Specialization: "; getline(cin, specialization);
    }
    void displayDetails() const override {
        Person::displayDetails();
        cout << setw(20) << specialization << endl;
    }
    string getSpecialization() const { return specialization; }

    void saveToFile(ofstream &out) const {
        out << id << "|" << name << "|" << age << "|" << gender << "|" << phone << "|" << specialization << endl;
    }
    static Doctor loadFromFile(string line) {
        Doctor d;
        size_t pos = 0;
        string token;
        vector<string> tokens;
        while ((pos = line.find("|")) != string::npos) {
            token = line.substr(0, pos);
            tokens.push_back(token);
            line.erase(0, pos + 1);
        }
        tokens.push_back(line);
        if (tokens.size() >= 6) {
            d.id = stoi(tokens[0]);
            d.name = tokens[1];
            d.age = stoi(tokens[2]);
            d.gender = tokens[3];
            d.phone = tokens[4];
            d.specialization = tokens[5];
        }
        return d;
    }
};

class Appointment {
    int appointmentId;
    int patientId;
    int doctorId;
    string date;
public:
    Appointment() : appointmentId(0), patientId(0), doctorId(0) {}
    void book(int id, int pId, int dId, string d) {
        appointmentId = id;
        patientId = pId;
        doctorId = dId;
        date = d;
    }
    void display(const vector<Patient>& patients, const vector<Doctor>& doctors) const {
        string pName = "Unknown", dName = "Unknown";
        for(const auto& p : patients) if(p.getId() == patientId) pName = p.getName();
        for(const auto& d : doctors) if(d.getId() == doctorId) dName = d.getName();
        
        cout << left << setw(15) << appointmentId << setw(20) << pName << setw(20) << dName << setw(15) << date << endl;
    }
    int getAppointmentId() const { return appointmentId; }
    
    void saveToFile(ofstream &out) const {
        out << appointmentId << "|" << patientId << "|" << doctorId << "|" << date << endl;
    }
    static Appointment loadFromFile(string line) {
        Appointment a;
        size_t pos = 0;
        string token;
        vector<string> tokens;
        while ((pos = line.find("|")) != string::npos) {
            token = line.substr(0, pos);
            tokens.push_back(token);
            line.erase(0, pos + 1);
        }
        tokens.push_back(line);
        if (tokens.size() >= 4) {
            a.appointmentId = stoi(tokens[0]);
            a.patientId = stoi(tokens[1]);
            a.doctorId = stoi(tokens[2]);
            a.date = tokens[3];
        }
        return a;
    }
};

// --- Hospital Management System Class ---

class HospitalSystem {
    vector<Patient> patients;
    vector<Doctor> doctors;
    vector<Appointment> appointments;
    const string patientFile = "patients.txt";
    const string doctorFile = "doctors.txt";
    const string appointmentFile = "appointments.txt";

public:
    HospitalSystem() {
        loadData();
    }

    void loadData() {
        string line;
        ifstream pIn(patientFile);
        while (getline(pIn, line)) patients.push_back(Patient::loadFromFile(line));
        pIn.close();

        ifstream dIn(doctorFile);
        while (getline(dIn, line)) doctors.push_back(Doctor::loadFromFile(line));
        dIn.close();

        ifstream aIn(appointmentFile);
        while (getline(aIn, line)) appointments.push_back(Appointment::loadFromFile(line));
        aIn.close();
    }

    void saveData() {
        ofstream pOut(patientFile);
        for (const auto& p : patients) p.saveToFile(pOut);
        pOut.close();

        ofstream dOut(doctorFile);
        for (const auto& d : doctors) d.saveToFile(dOut);
        dOut.close();

        ofstream aOut(appointmentFile);
        for (const auto& a : appointments) a.saveToFile(aOut);
        aOut.close();
    }

    // Patient Methods
    void addPatient() {
        Patient p;
        p.inputDetails();
        patients.push_back(p);
        cout << "Patient added successfully!\n";
        saveData();
    }

    void viewPatients() {
        cout << "\n--- Patient List ---\n";
        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Age" << setw(10) << "Gender" << setw(15) << "Phone" << setw(20) << "Disease" << endl;
        cout << string(85, '-') << endl;
        for (const auto& p : patients) p.displayDetails();
    }

    void searchPatient() {
        int id;
        cout << "Enter Patient ID to search: "; cin >> id;
        for (const auto& p : patients) {
            if (p.getId() == id) {
                p.displayDetails();
                return;
            }
        }
        cout << "Patient not found.\n";
    }

    void deletePatient() {
        int id;
        cout << "Enter Patient ID to delete: "; cin >> id;
        auto it = remove_if(patients.begin(), patients.end(), [id](const Patient& p) { return p.getId() == id; });
        if (it != patients.end()) {
            patients.erase(it, patients.end());
            cout << "Patient deleted.\n";
            saveData();
        } else {
            cout << "Patient not found.\n";
        }
    }

    // Doctor Methods
    void addDoctor() {
        Doctor d;
        d.inputDetails();
        doctors.push_back(d);
        cout << "Doctor added successfully!\n";
        saveData();
    }

    void viewDoctors() {
        cout << "\n--- Doctor List ---\n";
        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Age" << setw(10) << "Gender" << setw(15) << "Phone" << setw(20) << "Specialization" << endl;
        cout << string(85, '-') << endl;
        for (const auto& d : doctors) d.displayDetails();
    }

    // Appointment Methods
    void bookAppointment() {
        int pId, dId, aId;
        string date;
        cout << "Enter Appointment ID: "; cin >> aId;
        cout << "Enter Patient ID: "; cin >> pId;
        cout << "Enter Doctor ID: "; cin >> dId;
        cout << "Enter Date (DD/MM/YYYY): "; cin >> date;
        
        Appointment a;
        a.book(aId, pId, dId, date);
        appointments.push_back(a);
        cout << "Appointment booked!\n";
        saveData();
    }

    void viewAppointments() {
        cout << "\n--- Appointments ---\n";
        cout << left << setw(15) << "Appt ID" << setw(20) << "Patient" << setw(20) << "Doctor" << setw(15) << "Date" << endl;
        cout << string(70, '-') << endl;
        for (const auto& a : appointments) a.display(patients, doctors);
    }

    // Billing System
    void generateBill() {
        int pId;
        double consult, treatment;
        cout << "Enter Patient ID for billing: "; cin >> pId;
        
        Patient* patient = nullptr;
        for (auto& p : patients) if (p.getId() == pId) patient = &p;
        
        if (!patient) {
            cout << "Patient not found.\n";
            return;
        }

        cout << "Enter Consultation Fee: "; cin >> consult;
        cout << "Enter Treatment Cost: "; cin >> treatment;

        clearScreen();
        cout << "========================================\n";
        cout << "           HOSPITAL INVOICE             \n";
        cout << "========================================\n";
        cout << "Patient Name: " << patient->getName() << endl;
        cout << "Patient ID:   " << patient->getId() << endl;
        cout << "Disease:      " << patient->getDisease() << endl;
        cout << "----------------------------------------\n";
        cout << left << setw(25) << "Consultation Fee:" << "$" << consult << endl;
        cout << left << setw(25) << "Treatment Cost:" << "$" << treatment << endl;
        cout << "----------------------------------------\n";
        cout << left << setw(25) << "TOTAL AMOUNT:" << "$" << (consult + treatment) << endl;
        cout << "========================================\n";
    }
};

// --- Authentication ---

bool login() {
    string user, pass;
    cout << "\n--- Hospital Management System Login ---\n";
    cout << "Username: "; cin >> user;
    cout << "Password: "; cin >> pass;

    if (user == "admin" && pass == "admin123") {
        return true;
    } else {
        cout << "Invalid credentials!\n";
        return false;
    }
}

// --- Main Menu ---

int main() {
    if (!login()) return 0;

    HospitalSystem system;
    int choice;

    while (true) {
        clearScreen();
        cout << "\n--- Hospital Management System ---\n";
        cout << "1. Patient Management\n";
        cout << "2. Doctor Management\n";
        cout << "3. Appointment System\n";
        cout << "4. Billing System\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int pChoice;
                cout << "\n1. Add Patient\n2. View Patients\n3. Search Patient\n4. Delete Patient\nBack to main menu (any other key)\nChoice: ";
                cin >> pChoice;
                if (pChoice == 1) system.addPatient();
                else if (pChoice == 2) system.viewPatients();
                else if (pChoice == 3) system.searchPatient();
                else if (pChoice == 4) system.deletePatient();
                pressEnterToContinue();
                break;
            }
            case 2: {
                int dChoice;
                cout << "\n1. Add Doctor\n2. View Doctors\nBack to main menu (any other key)\nChoice: ";
                cin >> dChoice;
                if (dChoice == 1) system.addDoctor();
                else if (dChoice == 2) system.viewDoctors();
                pressEnterToContinue();
                break;
            }
            case 3: {
                int aChoice;
                cout << "\n1. Book Appointment\n2. View Appointments\nBack to main menu (any other key)\nChoice: ";
                cin >> aChoice;
                if (aChoice == 1) system.bookAppointment();
                else if (aChoice == 2) system.viewAppointments();
                pressEnterToContinue();
                break;
            }
            case 4:
                system.generateBill();
                pressEnterToContinue();
                break;
            case 5:
                cout << "Exiting system...\n";
                return 0;
            default:
                cout << "Invalid choice!\n";
                pressEnterToContinue();
        }
    }

    return 0;
}


