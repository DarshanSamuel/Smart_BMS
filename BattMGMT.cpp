#include <iostream>
using namespace std;

class Battery
{
    private:
    float voltage;
    int capacity;

    public:
    Battery()
    {
        this->voltage = 0.0f; // Default voltage
        this->capacity = 0;   // Default capacity
    }
    void getBatteryData()
    {
        float voltage = this->voltage;
        int capacity = this->capacity;

        cout << "Entering Battery Data:\n";
        cout << "------------------------\n";
        cout << "Enter Battery Voltage: " << "\n";
        cout << "Enter Battery Capacity: " << "\n";
        cin >> voltage;
        cin >> capacity;
        cout << "------------------------\n";
    }

    void showStatus()
    {
        if(capacity >= 80 && capacity <= 100)
        {
            cout << "Battery Status: Excellent\n";
        }
        else if(capacity >= 50 && capacity < 80)
        {
            cout << "Battery Status: Good\n";
        }
        else if(capacity >= 20 && capacity < 50)
        {
            cout << "Battery Status: Attention Required\n";
        }
        else if(capacity < 20 && capacity > 0)
        {
            cout << "Battery Status: Critical\n";
        }
        else
        {
            cout << "Battery Status: Not Connected\n";
        }
    }
};

int main()
{
    Battery obj[10];

    for(int i = 0; i < 10; i++)
    {
        cout << "Battery " << i + 1 << ":\n";
        obj[i].getBatteryData();
        obj[i].showStatus();
        cout << "\n";
    }
    cout << "All battery data has been entered and status displayed.\n";
    return 0;
}