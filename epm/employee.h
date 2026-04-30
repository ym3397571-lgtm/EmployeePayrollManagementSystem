#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "data.h"
#include <string>
using namespace std;

// =======================
// Employee Panel Functions
// =======================

bool employeeLogin();               // تسجيل دخول الموظف
void viewPersonalInfo();            // عرض البيانات الشخصية للموظف
void viewSalary();                  // عرض تفاصيل المرتب
void viewAttendance();              // عرض سجل الحضور للموظف

#endif