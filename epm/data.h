#ifndef DATA_H
#define DATA_H

#include <string>
using namespace std;

// =======================
// Structs
// =======================

struct Employee {
    long long employeeID = 0 ;         // معرف الموظف
    string name  ;            // الاسم
    string username;
    int age = 0 ;                // العمر
    long long phone = 0 ;           // رقم الهاتف
    string role ;            // الوظيفة
    double basicSalary = 0 ;     // الراتب الأساسي
    double bonus = 0  ;           // البونص
    double overtime = 0  ;        // العمل الإضافي
    double tax = 0  ;             // الضريبة
    double netSalary = 0  ;       // صافي الراتب
    string password ;        // كلمة السر
    double TotalHoursWorked = 0;
    double WorkHoursPerMonth = 0;
};

struct Attendance {
    long long employeeID = 0 ;         // ID الموظف
    int month = 0  ;           // الشهر
    int daysPresent = 0 ;        // عدد أيام الحضور
    int daysAbsent = 0 ;         // عدد أيام الغياب
};

struct AdminAccount {
    long long adminID = 0 ;            // معرف الأدمن
    string name;
    string username ;
    string password ;        // كلمة السر
};

// =======================
// Constants
// =======================

const int MAX_EMPLOYEES = 100;
const int MAX_ATTENDANCE = 200;
const int MAX_ADMINS = 2;
const double TaxRate = 0.14;

// =======================
// Arrays
// =======================

extern Employee employees[MAX_EMPLOYEES];
extern Attendance attendanceRecords[MAX_ATTENDANCE];
extern AdminAccount admins[MAX_ADMINS];

// =======================
// Counters
// =======================

extern int employeeCount;
extern int attendanceCount;
extern int adminCount;
#endif