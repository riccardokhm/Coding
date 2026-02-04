// Generating a Truck object derived from the Vehicle base class
#include <iostream>
using namespace std;

class Vehicle {
    public:
        int passengers;
        int fuelcap;
        int lpk;
        Vehicle(int p, int f, int l) : passengers(p), fuelcap(f), lpk(l) {}
        void range() {
            cout << "Range: " << (fuelcap / lpk) * 100 << " km" << endl;
        }   
};

class Truck : public Vehicle {
    private:
        int cargocap;
    public:
        Truck(int p, int f, int l, int c) : Vehicle(p, f, l) { cargocap = c;}
        int get_cargocap() {
            return cargocap;
        }
};