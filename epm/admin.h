#ifndef ADMIN_H
#define ADMIN_H

#include "data.h"
#include <string>
using namespace std;

// =======================
// Admin Panel Functions
// =======================

bool adminLogin();                  // تسجيل دخول الأدمن
void addEmployee();                 // إضافة موظف جديد
void updateEmployee(long long empId);              // تعديل بيانات موظف موجود
void deleteEmployee();              // حذف موظف
void recordAttendance();            // تسجيل حضور الموظفين
void deleteAttendance();
void updateAttendance(long long empId);            // تعديل حضور الموظفين
void calculateSalary(long long empId);             // حساب صافي الراتب لكل موظف
void viewAllEmployeesData();
#endif // ADMIN_H