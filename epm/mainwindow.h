#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTableWidget;
class QTabWidget;
class QWidget;

class MainWindow : public QMainWindow
{
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    void setupUi();
    QWidget *createAdminPanel();
    QWidget *createEmployeePanel();

    void setAdminLoggedIn(bool loggedIn);
    void setEmployeeLoggedIn(bool loggedIn);

    void refreshEmployeeTable();
    void refreshEmployeeView();
    void loadEmployeeIntoForm(int row);
    void clearEmployeeForm();

    void attemptAdminLogin();
    void attemptEmployeeLogin();

    void handleAddEmployee();
    void handleUpdateEmployee();
    void handleDeleteEmployee();
    void handleRecordAttendance();
    void handleCalculateSalary();

    QTabWidget *tabWidget;

    QWidget *adminLoginWidget;
    QWidget *adminContentWidget;
    QLineEdit *adminUsernameEdit;
    QLineEdit *adminPasswordEdit;

    QLineEdit *employeeIdEdit;
    QLineEdit *employeeNameEdit;
    QLineEdit *employeeUsernameFormEdit;
    QLineEdit *employeePasswordFormEdit;
    QLineEdit *employeeAgeEdit;
    QLineEdit *employeePhoneEdit;
    QLineEdit *employeeRoleEdit;
    QLineEdit *employeeSalaryEdit;
    QLineEdit *employeeTotalHoursEdit;
    QLineEdit *employeeMonthlyHoursEdit;
    QLineEdit *deleteEmployeeIdEdit;
    QLineEdit *attendanceEmployeeIdEdit;
    QLineEdit *attendanceMonthEdit;
    QLineEdit *attendancePresentEdit;
    QLineEdit *attendanceAbsentEdit;
    QLineEdit *salaryEmployeeIdEdit;
    QLabel *adminStatusLabel;
    QTableWidget *employeeTable;

    QWidget *employeeLoginWidget;
    QWidget *employeeContentWidget;
    QLineEdit *employeeLoginUsernameEdit;
    QLineEdit *employeeLoginPasswordEdit;
    QLabel *employeeWelcomeLabel;
    QLabel *employeeInfoLabel;
    QLabel *employeeSalaryLabel;
    QTableWidget *employeeAttendanceTable;

    int loggedInAdminIndex;
    int loggedInEmployeeIndex;
};

#endif
