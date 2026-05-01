#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTabWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextStream>
#include <QVBoxLayout>
#include <QStringList>

#include "data.h"

Employee employees[MAX_EMPLOYEES];
Attendance attendanceRecords[MAX_ATTENDANCE];
AdminAccount admins[MAX_ADMINS];

int employeeCount = 0;
int attendanceCount = 0;
int adminCount = 0;

static QString findDataDirectory()
{
    const QStringList candidates = {
        QDir::currentPath(),
        QCoreApplication::applicationDirPath(),
        QDir(QCoreApplication::applicationDirPath()).filePath(".."),
        QDir(QCoreApplication::applicationDirPath()).filePath("../..")
    };

    for (const QString &candidate : candidates) {
        if (QFile::exists(QDir(candidate).filePath("employees.txt"))) {
            return QDir(candidate).absolutePath();
        }
    }

    return QDir::currentPath();
}

static QString filePath(const QString &fileName)
{
    return QDir(findDataDirectory()).filePath(fileName);
}

static QString toQString(const std::string &value)
{
    return QString::fromStdString(value);
}

static std::string toStdString(const QString &value)
{
    return value.trimmed().toStdString();
}

static int findEmployeeIndexById(long long employeeId)
{
    for (int i = 0; i < employeeCount; ++i) {
        if (employees[i].employeeID == employeeId) {
            return i;
        }
    }
    return -1;
}

static int findEmployeeIndexByCredentials(const QString &username, const QString &password)
{
    for (int i = 0; i < employeeCount; ++i) {
        if (toQString(employees[i].username) == username.trimmed() &&
            toQString(employees[i].password) == password) {
            return i;
        }
    }
    return -1;
}

static int findAdminIndexByCredentials(const QString &username, const QString &password)
{
    for (int i = 0; i < adminCount; ++i) {
        if (toQString(admins[i].username) == username.trimmed() &&
            toQString(admins[i].password) == password) {
            return i;
        }
    }
    return -1;
}

static void loadEmployees()
{
    QFile file(filePath("employees.txt"));
    employeeCount = 0;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd() && employeeCount < MAX_EMPLOYEES) {
        const QString line = in.readLine().trimmed();
        if (line.isEmpty()) {
            continue;
        }

        const QStringList parts = line.split("|");
        if (parts.size() < 14) {
            continue;
        }

        Employee &e = employees[employeeCount];
        e.employeeID = parts[0].toLongLong();
        e.name = toStdString(parts[1]);
        e.username = toStdString(parts[2]);
        e.age = parts[3].toInt();
        e.phone = parts[4].toLongLong();
        e.role = toStdString(parts[5]);
        e.basicSalary = parts[6].toDouble();
        e.bonus = parts[7].toDouble();
        e.overtime = parts[8].toDouble();
        e.tax = parts[9].toDouble();
        e.netSalary = parts[10].toDouble();
        e.password = toStdString(parts[11]);
        e.TotalHoursWorked = parts[12].toDouble();
        e.WorkHoursPerMonth = parts[13].toDouble();
        ++employeeCount;
    }
}

static void saveEmployees()
{
    QFile file(filePath("employees.txt"));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    QTextStream out(&file);
    for (int i = 0; i < employeeCount; ++i) {
        const Employee &e = employees[i];
        out << e.employeeID << "|"
            << toQString(e.name) << "|"
            << toQString(e.username) << "|"
            << e.age << "|"
            << e.phone << "|"
            << toQString(e.role) << "|"
            << e.basicSalary << "|"
            << e.bonus << "|"
            << e.overtime << "|"
            << e.tax << "|"
            << e.netSalary << "|"
            << toQString(e.password) << "|"
            << e.TotalHoursWorked << "|"
            << e.WorkHoursPerMonth << "\n";
    }
}

static void loadAdmins()
{
    QFile file(filePath("admins.txt"));
    adminCount = 0;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd() && adminCount < MAX_ADMINS) {
        const QString line = in.readLine().trimmed();
        if (line.isEmpty()) {
            continue;
        }

        const QStringList parts = line.split("|");
        if (parts.size() < 4) {
            continue;
        }

        AdminAccount &a = admins[adminCount];
        a.adminID = parts[0].toLongLong();
        a.name = toStdString(parts[1]);
        a.username = toStdString(parts[2]);
        a.password = toStdString(parts[3]);
        ++adminCount;
    }
}

static void saveAdmins()
{
    QFile file(filePath("admins.txt"));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    QTextStream out(&file);
    for (int i = 0; i < adminCount; ++i) {
        const AdminAccount &a = admins[i];
        out << a.adminID << "|"
            << toQString(a.name) << "|"
            << toQString(a.username) << "|"
            << toQString(a.password) << "\n";
    }
}

static void loadAttendance()
{
    QFile file(filePath("attendance.txt"));
    attendanceCount = 0;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd() && attendanceCount < MAX_ATTENDANCE) {
        const QString line = in.readLine().trimmed();
        if (line.isEmpty()) {
            continue;
        }

        const QStringList parts = line.split("|");
        if (parts.size() < 4) {
            continue;
        }

        Attendance &a = attendanceRecords[attendanceCount];
        a.employeeID = parts[0].toLongLong();
        a.month = parts[1].toInt();
        a.daysPresent = parts[2].toInt();
        a.daysAbsent = parts[3].toInt();
        ++attendanceCount;
    }
}

static void saveAttendance()
{
    QFile file(filePath("attendance.txt"));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    QTextStream out(&file);
    for (int i = 0; i < attendanceCount; ++i) {
        const Attendance &a = attendanceRecords[i];
        out << a.employeeID << "|"
            << a.month << "|"
            << a.daysPresent << "|"
            << a.daysAbsent << "\n";
    }
}

static bool calculateSalary(long long employeeId, QString &message)
{
    const int index = findEmployeeIndexById(employeeId);
    if (index == -1) {
        message = "Employee ID not found.";
        return false;
    }

    if (employees[index].WorkHoursPerMonth <= 0) {
        message = "Work hours per month must be greater than zero.";
        return false;
    }

    employees[index].overtime = employees[index].TotalHoursWorked - employees[index].WorkHoursPerMonth;
    if (employees[index].overtime < 0) {
        employees[index].overtime = 0;
    }

    employees[index].bonus =
        (employees[index].basicSalary / employees[index].WorkHoursPerMonth) * employees[index].overtime;
    employees[index].tax = employees[index].basicSalary * TaxRate;
    employees[index].netSalary =
        (employees[index].basicSalary + employees[index].bonus) - employees[index].tax;

    saveEmployees();
    message = "Salary calculated successfully.";
    return true;
}

static bool addEmployee(const Employee &employee, QString &message)
{
    if (employeeCount >= MAX_EMPLOYEES) {
        message = "Employee list is full.";
        return false;
    }

    if (findEmployeeIndexById(employee.employeeID) != -1) {
        message = "Employee ID already exists. Use Update instead.";
        return false;
    }

    employees[employeeCount] = employee;
    ++employeeCount;
    saveEmployees();
    message = "Employee added successfully.";
    return true;
}

static bool updateEmployee(const Employee &employee, QString &message)
{
    const int index = findEmployeeIndexById(employee.employeeID);
    if (index == -1) {
        message = "Employee ID not found.";
        return false;
    }

    employees[index] = employee;
    saveEmployees();
    message = "Employee updated successfully.";
    return true;
}

static bool deleteEmployee(long long employeeId, QString &message)
{
    const int index = findEmployeeIndexById(employeeId);
    if (index == -1) {
        message = "Employee ID not found.";
        return false;
    }

    for (int i = index; i < employeeCount - 1; ++i) {
        employees[i] = employees[i + 1];
    }
    --employeeCount;

    for (int i = 0; i < attendanceCount; ) {
        if (attendanceRecords[i].employeeID == employeeId) {
            for (int j = i; j < attendanceCount - 1; ++j) {
                attendanceRecords[j] = attendanceRecords[j + 1];
            }
            --attendanceCount;
        } else {
            ++i;
        }
    }

    saveEmployees();
    saveAttendance();
    message = "Employee deleted successfully.";
    return true;
}

static bool recordAttendance(const Attendance &attendance, QString &message)
{
    if (findEmployeeIndexById(attendance.employeeID) == -1) {
        message = "Employee ID not found.";
        return false;
    }

    if (attendance.month < 1 || attendance.month > 12) {
        message = "Month must be between 1 and 12.";
        return false;
    }

    if (attendance.daysPresent < 0 || attendance.daysPresent > 31 ||
        attendance.daysAbsent < 0 || attendance.daysAbsent > 31) {
        message = "Attendance days must be between 0 and 31.";
        return false;
    }

    for (int i = 0; i < attendanceCount; ++i) {
        if (attendanceRecords[i].employeeID == attendance.employeeID &&
            attendanceRecords[i].month == attendance.month) {
            attendanceRecords[i] = attendance;
            saveAttendance();
            message = "Attendance updated successfully.";
            return true;
        }
    }

    if (attendanceCount >= MAX_ATTENDANCE) {
        message = "Attendance list is full.";
        return false;
    }

    attendanceRecords[attendanceCount] = attendance;
    ++attendanceCount;
    saveAttendance();
    message = "Attendance recorded successfully.";
    return true;
}

static bool parseLongLong(const QString &text, long long &value)
{
    bool ok = false;
    value = text.trimmed().toLongLong(&ok);
    return ok;
}

static bool parseInt(const QString &text, int &value)
{
    bool ok = false;
    value = text.trimmed().toInt(&ok);
    return ok;
}

static bool parseDouble(const QString &text, double &value)
{
    bool ok = false;
    value = text.trimmed().toDouble(&ok);
    return ok;
}

static bool readEmployeeFromForm(
    QLineEdit *idEdit,
    QLineEdit *nameEdit,
    QLineEdit *usernameEdit,
    QLineEdit *passwordEdit,
    QLineEdit *ageEdit,
    QLineEdit *phoneEdit,
    QLineEdit *roleEdit,
    QLineEdit *salaryEdit,
    QLineEdit *totalHoursEdit,
    QLineEdit *monthlyHoursEdit,
    Employee &employee,
    QString &message)
{
    if (nameEdit->text().trimmed().isEmpty() ||
        usernameEdit->text().trimmed().isEmpty() ||
        passwordEdit->text().isEmpty() ||
        roleEdit->text().trimmed().isEmpty()) {
        message = "Please fill all text fields.";
        return false;
    }

    if (!parseLongLong(idEdit->text(), employee.employeeID) ||
        !parseInt(ageEdit->text(), employee.age) ||
        !parseLongLong(phoneEdit->text(), employee.phone) ||
        !parseDouble(salaryEdit->text(), employee.basicSalary) ||
        !parseDouble(totalHoursEdit->text(), employee.TotalHoursWorked) ||
        !parseDouble(monthlyHoursEdit->text(), employee.WorkHoursPerMonth)) {
        message = "Please enter valid numeric values.";
        return false;
    }

    employee.name = toStdString(nameEdit->text());
    employee.username = toStdString(usernameEdit->text());
    employee.password = toStdString(passwordEdit->text());
    employee.role = toStdString(roleEdit->text());
    employee.bonus = 0;
    employee.overtime = 0;
    employee.tax = 0;
    employee.netSalary = 0;
    return true;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      tabWidget(nullptr),
      adminLoginWidget(nullptr),
      adminContentWidget(nullptr),
      adminUsernameEdit(nullptr),
      adminPasswordEdit(nullptr),
      employeeIdEdit(nullptr),
      employeeNameEdit(nullptr),
      employeeUsernameFormEdit(nullptr),
      employeePasswordFormEdit(nullptr),
      employeeAgeEdit(nullptr),
      employeePhoneEdit(nullptr),
      employeeRoleEdit(nullptr),
      employeeSalaryEdit(nullptr),
      employeeTotalHoursEdit(nullptr),
      employeeMonthlyHoursEdit(nullptr),
      deleteEmployeeIdEdit(nullptr),
      attendanceEmployeeIdEdit(nullptr),
      attendanceMonthEdit(nullptr),
      attendancePresentEdit(nullptr),
      attendanceAbsentEdit(nullptr),
      salaryEmployeeIdEdit(nullptr),
      adminStatusLabel(nullptr),
      employeeTable(nullptr),
      employeeLoginWidget(nullptr),
      employeeContentWidget(nullptr),
      employeeLoginUsernameEdit(nullptr),
      employeeLoginPasswordEdit(nullptr),
      employeeWelcomeLabel(nullptr),
      employeeInfoLabel(nullptr),
      employeeSalaryLabel(nullptr),
      employeeAttendanceTable(nullptr),
      loggedInAdminIndex(-1),
      loggedInEmployeeIndex(-1)
{
    loadEmployees();
    loadAdmins();
    loadAttendance();

    setWindowTitle("Employee Management System - Qt Widgets");
    resize(1100, 750);

    setupUi();
    refreshEmployeeTable();
    refreshEmployeeView();
    setAdminLoggedIn(false);
    setEmployeeLoggedIn(false);
}

MainWindow::~MainWindow()
{
    saveEmployees();
    saveAdmins();
    saveAttendance();
}

void MainWindow::setupUi()
{
    tabWidget = new QTabWidget(this);
    tabWidget->addTab(createAdminPanel(), "Admin Panel");
    tabWidget->addTab(createEmployeePanel(), "Employee Panel");
    setCentralWidget(tabWidget);
}

QWidget *MainWindow::createAdminPanel()
{
    QWidget *panel = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(panel);

    adminLoginWidget = new QWidget(panel);
    QFormLayout *loginLayout = new QFormLayout(adminLoginWidget);
    adminUsernameEdit = new QLineEdit(adminLoginWidget);
    adminPasswordEdit = new QLineEdit(adminLoginWidget);
    adminPasswordEdit->setEchoMode(QLineEdit::Password);
    QPushButton *adminLoginButton = new QPushButton("Login", adminLoginWidget);
    loginLayout->addRow("Username:", adminUsernameEdit);
    loginLayout->addRow("Password:", adminPasswordEdit);
    loginLayout->addWidget(adminLoginButton);

    adminContentWidget = new QWidget(panel);
    QVBoxLayout *contentLayout = new QVBoxLayout(adminContentWidget);

    adminStatusLabel = new QLabel(adminContentWidget);
    QPushButton *adminLogoutButton = new QPushButton("Logout", adminContentWidget);

    QHBoxLayout *topRow = new QHBoxLayout();
    topRow->addWidget(adminStatusLabel);
    topRow->addStretch();
    topRow->addWidget(adminLogoutButton);
    contentLayout->addLayout(topRow);

    QGroupBox *employeeFormGroup = new QGroupBox("Add / Update Employee", adminContentWidget);
    QGridLayout *employeeFormLayout = new QGridLayout(employeeFormGroup);

    employeeIdEdit = new QLineEdit(employeeFormGroup);
    employeeNameEdit = new QLineEdit(employeeFormGroup);
    employeeUsernameFormEdit = new QLineEdit(employeeFormGroup);
    employeePasswordFormEdit = new QLineEdit(employeeFormGroup);
    employeeAgeEdit = new QLineEdit(employeeFormGroup);
    employeePhoneEdit = new QLineEdit(employeeFormGroup);
    employeeRoleEdit = new QLineEdit(employeeFormGroup);
    employeeSalaryEdit = new QLineEdit(employeeFormGroup);
    employeeTotalHoursEdit = new QLineEdit(employeeFormGroup);
    employeeMonthlyHoursEdit = new QLineEdit(employeeFormGroup);

    employeeFormLayout->addWidget(new QLabel("Employee ID:"), 0, 0);
    employeeFormLayout->addWidget(employeeIdEdit, 0, 1);
    employeeFormLayout->addWidget(new QLabel("Name:"), 0, 2);
    employeeFormLayout->addWidget(employeeNameEdit, 0, 3);
    employeeFormLayout->addWidget(new QLabel("Username:"), 1, 0);
    employeeFormLayout->addWidget(employeeUsernameFormEdit, 1, 1);
    employeeFormLayout->addWidget(new QLabel("Password:"), 1, 2);
    employeeFormLayout->addWidget(employeePasswordFormEdit, 1, 3);
    employeeFormLayout->addWidget(new QLabel("Age:"), 2, 0);
    employeeFormLayout->addWidget(employeeAgeEdit, 2, 1);
    employeeFormLayout->addWidget(new QLabel("Phone:"), 2, 2);
    employeeFormLayout->addWidget(employeePhoneEdit, 2, 3);
    employeeFormLayout->addWidget(new QLabel("Role:"), 3, 0);
    employeeFormLayout->addWidget(employeeRoleEdit, 3, 1);
    employeeFormLayout->addWidget(new QLabel("Basic Salary:"), 3, 2);
    employeeFormLayout->addWidget(employeeSalaryEdit, 3, 3);
    employeeFormLayout->addWidget(new QLabel("Total Hours Worked:"), 4, 0);
    employeeFormLayout->addWidget(employeeTotalHoursEdit, 4, 1);
    employeeFormLayout->addWidget(new QLabel("Work Hours / Month:"), 4, 2);
    employeeFormLayout->addWidget(employeeMonthlyHoursEdit, 4, 3);

    QPushButton *addEmployeeButton = new QPushButton("Add Employee", employeeFormGroup);
    QPushButton *updateEmployeeButton = new QPushButton("Update Employee", employeeFormGroup);
    QPushButton *clearFormButton = new QPushButton("Clear Form", employeeFormGroup);
    employeeFormLayout->addWidget(addEmployeeButton, 5, 1);
    employeeFormLayout->addWidget(updateEmployeeButton, 5, 2);
    employeeFormLayout->addWidget(clearFormButton, 5, 3);
    contentLayout->addWidget(employeeFormGroup);

    QGroupBox *actionsGroup = new QGroupBox("Delete / Attendance / Salary", adminContentWidget);
    QGridLayout *actionsLayout = new QGridLayout(actionsGroup);

    deleteEmployeeIdEdit = new QLineEdit(actionsGroup);
    attendanceEmployeeIdEdit = new QLineEdit(actionsGroup);
    attendanceMonthEdit = new QLineEdit(actionsGroup);
    attendancePresentEdit = new QLineEdit(actionsGroup);
    attendanceAbsentEdit = new QLineEdit(actionsGroup);
    salaryEmployeeIdEdit = new QLineEdit(actionsGroup);

    QPushButton *deleteEmployeeButton = new QPushButton("Delete Employee", actionsGroup);
    QPushButton *recordAttendanceButton = new QPushButton("Save Attendance", actionsGroup);
    QPushButton *calculateSalaryButton = new QPushButton("Calculate Salary", actionsGroup);

    actionsLayout->addWidget(new QLabel("Delete Employee ID:"), 0, 0);
    actionsLayout->addWidget(deleteEmployeeIdEdit, 0, 1);
    actionsLayout->addWidget(deleteEmployeeButton, 0, 2);
    actionsLayout->addWidget(new QLabel("Attendance Employee ID:"), 1, 0);
    actionsLayout->addWidget(attendanceEmployeeIdEdit, 1, 1);
    actionsLayout->addWidget(new QLabel("Month:"), 1, 2);
    actionsLayout->addWidget(attendanceMonthEdit, 1, 3);
    actionsLayout->addWidget(new QLabel("Days Present:"), 2, 0);
    actionsLayout->addWidget(attendancePresentEdit, 2, 1);
    actionsLayout->addWidget(new QLabel("Days Absent:"), 2, 2);
    actionsLayout->addWidget(attendanceAbsentEdit, 2, 3);
    actionsLayout->addWidget(recordAttendanceButton, 3, 3);
    actionsLayout->addWidget(new QLabel("Salary Employee ID:"), 4, 0);
    actionsLayout->addWidget(salaryEmployeeIdEdit, 4, 1);
    actionsLayout->addWidget(calculateSalaryButton, 4, 2);
    contentLayout->addWidget(actionsGroup);

    employeeTable = new QTableWidget(adminContentWidget);
    employeeTable->setColumnCount(9);
    employeeTable->setHorizontalHeaderLabels({
        "ID", "Name", "Username", "Age", "Phone", "Role", "Basic Salary", "Net Salary", "Hours"
    });
    employeeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    employeeTable->setSelectionBehavior(QTableWidget::SelectRows);
    employeeTable->setSelectionMode(QTableWidget::SingleSelection);
    contentLayout->addWidget(employeeTable);

    mainLayout->addWidget(adminLoginWidget);
    mainLayout->addWidget(adminContentWidget);

    connect(adminLoginButton, &QPushButton::clicked, this, [this]() { attemptAdminLogin(); });
    connect(adminLogoutButton, &QPushButton::clicked, this, [this]() { setAdminLoggedIn(false); });
    connect(addEmployeeButton, &QPushButton::clicked, this, [this]() { handleAddEmployee(); });
    connect(updateEmployeeButton, &QPushButton::clicked, this, [this]() { handleUpdateEmployee(); });
    connect(deleteEmployeeButton, &QPushButton::clicked, this, [this]() { handleDeleteEmployee(); });
    connect(recordAttendanceButton, &QPushButton::clicked, this, [this]() { handleRecordAttendance(); });
    connect(calculateSalaryButton, &QPushButton::clicked, this, [this]() { handleCalculateSalary(); });
    connect(clearFormButton, &QPushButton::clicked, this, [this]() { clearEmployeeForm(); });
    connect(employeeTable, &QTableWidget::cellClicked, this, [this](int row, int) { loadEmployeeIntoForm(row); });

    return panel;
}

QWidget *MainWindow::createEmployeePanel()
{
    QWidget *panel = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(panel);

    employeeLoginWidget = new QWidget(panel);
    QFormLayout *loginLayout = new QFormLayout(employeeLoginWidget);
    employeeLoginUsernameEdit = new QLineEdit(employeeLoginWidget);
    employeeLoginPasswordEdit = new QLineEdit(employeeLoginWidget);
    employeeLoginPasswordEdit->setEchoMode(QLineEdit::Password);
    QPushButton *employeeLoginButton = new QPushButton("Login", employeeLoginWidget);
    loginLayout->addRow("Username:", employeeLoginUsernameEdit);
    loginLayout->addRow("Password:", employeeLoginPasswordEdit);
    loginLayout->addWidget(employeeLoginButton);

    employeeContentWidget = new QWidget(panel);
    QVBoxLayout *contentLayout = new QVBoxLayout(employeeContentWidget);

    employeeWelcomeLabel = new QLabel(employeeContentWidget);
    employeeInfoLabel = new QLabel(employeeContentWidget);
    employeeSalaryLabel = new QLabel(employeeContentWidget);
    QPushButton *employeeLogoutButton = new QPushButton("Logout", employeeContentWidget);

    employeeInfoLabel->setWordWrap(true);
    employeeSalaryLabel->setWordWrap(true);

    contentLayout->addWidget(employeeWelcomeLabel);
    contentLayout->addWidget(employeeInfoLabel);
    contentLayout->addWidget(employeeSalaryLabel);
    contentLayout->addWidget(employeeLogoutButton);

    employeeAttendanceTable = new QTableWidget(employeeContentWidget);
    employeeAttendanceTable->setColumnCount(3);
    employeeAttendanceTable->setHorizontalHeaderLabels({"Month", "Days Present", "Days Absent"});
    employeeAttendanceTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    contentLayout->addWidget(new QLabel("Attendance"));
    contentLayout->addWidget(employeeAttendanceTable);

    mainLayout->addWidget(employeeLoginWidget);
    mainLayout->addWidget(employeeContentWidget);

    connect(employeeLoginButton, &QPushButton::clicked, this, [this]() { attemptEmployeeLogin(); });
    connect(employeeLogoutButton, &QPushButton::clicked, this, [this]() { setEmployeeLoggedIn(false); });

    return panel;
}

void MainWindow::setAdminLoggedIn(bool loggedIn)
{
    adminLoginWidget->setVisible(!loggedIn);
    adminContentWidget->setVisible(loggedIn);

    if (loggedIn && loggedInAdminIndex >= 0) {
        adminStatusLabel->setText("Logged in as: " + toQString(admins[loggedInAdminIndex].name));
    } else {
        loggedInAdminIndex = -1;
        adminUsernameEdit->clear();
        adminPasswordEdit->clear();
        adminStatusLabel->clear();
    }
}

void MainWindow::setEmployeeLoggedIn(bool loggedIn)
{
    employeeLoginWidget->setVisible(!loggedIn);
    employeeContentWidget->setVisible(loggedIn);

    if (!loggedIn) {
        loggedInEmployeeIndex = -1;
        employeeLoginUsernameEdit->clear();
        employeeLoginPasswordEdit->clear();
        employeeWelcomeLabel->setText("Please login first.");
        employeeInfoLabel->clear();
        employeeSalaryLabel->clear();
        employeeAttendanceTable->setRowCount(0);
    }
}

void MainWindow::refreshEmployeeTable()
{
    employeeTable->setRowCount(employeeCount);

    for (int i = 0; i < employeeCount; ++i) {
        const Employee &e = employees[i];
        employeeTable->setItem(i, 0, new QTableWidgetItem(QString::number(e.employeeID)));
        employeeTable->setItem(i, 1, new QTableWidgetItem(toQString(e.name)));
        employeeTable->setItem(i, 2, new QTableWidgetItem(toQString(e.username)));
        employeeTable->setItem(i, 3, new QTableWidgetItem(QString::number(e.age)));
        employeeTable->setItem(i, 4, new QTableWidgetItem(QString::number(e.phone)));
        employeeTable->setItem(i, 5, new QTableWidgetItem(toQString(e.role)));
        employeeTable->setItem(i, 6, new QTableWidgetItem(QString::number(e.basicSalary, 'f', 2)));
        employeeTable->setItem(i, 7, new QTableWidgetItem(QString::number(e.netSalary, 'f', 2)));
        employeeTable->setItem(i, 8, new QTableWidgetItem(
            QString("%1 / %2").arg(e.TotalHoursWorked, 0, 'f', 0).arg(e.WorkHoursPerMonth, 0, 'f', 0)));
    }
}

void MainWindow::refreshEmployeeView()
{
    if (loggedInEmployeeIndex < 0 || loggedInEmployeeIndex >= employeeCount) {
        return;
    }

    QString salaryMessage;
    calculateSalary(employees[loggedInEmployeeIndex].employeeID, salaryMessage);

    const Employee &e = employees[loggedInEmployeeIndex];
    employeeWelcomeLabel->setText("Welcome, " + toQString(e.name));
    employeeInfoLabel->setText(
        "Employee ID: " + QString::number(e.employeeID) + "\n" +
        "Username: " + toQString(e.username) + "\n" +
        "Age: " + QString::number(e.age) + "\n" +
        "Phone: " + QString::number(e.phone) + "\n" +
        "Role: " + toQString(e.role));

    employeeSalaryLabel->setText(
        "Basic Salary: " + QString::number(e.basicSalary, 'f', 2) + "\n" +
        "Bonus: " + QString::number(e.bonus, 'f', 2) + "\n" +
        "Overtime: " + QString::number(e.overtime, 'f', 2) + "\n" +
        "Tax: " + QString::number(e.tax, 'f', 2) + "\n" +
        "Net Salary: " + QString::number(e.netSalary, 'f', 2));

    int row = 0;
    employeeAttendanceTable->setRowCount(0);
    for (int i = 0; i < attendanceCount; ++i) {
        if (attendanceRecords[i].employeeID == e.employeeID) {
            employeeAttendanceTable->insertRow(row);
            employeeAttendanceTable->setItem(row, 0, new QTableWidgetItem(QString::number(attendanceRecords[i].month)));
            employeeAttendanceTable->setItem(row, 1, new QTableWidgetItem(QString::number(attendanceRecords[i].daysPresent)));
            employeeAttendanceTable->setItem(row, 2, new QTableWidgetItem(QString::number(attendanceRecords[i].daysAbsent)));
            ++row;
        }
    }
}

void MainWindow::loadEmployeeIntoForm(int row)
{
    if (row < 0 || row >= employeeCount) {
        return;
    }

    const Employee &e = employees[row];
    employeeIdEdit->setText(QString::number(e.employeeID));
    employeeNameEdit->setText(toQString(e.name));
    employeeUsernameFormEdit->setText(toQString(e.username));
    employeePasswordFormEdit->setText(toQString(e.password));
    employeeAgeEdit->setText(QString::number(e.age));
    employeePhoneEdit->setText(QString::number(e.phone));
    employeeRoleEdit->setText(toQString(e.role));
    employeeSalaryEdit->setText(QString::number(e.basicSalary, 'f', 2));
    employeeTotalHoursEdit->setText(QString::number(e.TotalHoursWorked, 'f', 0));
    employeeMonthlyHoursEdit->setText(QString::number(e.WorkHoursPerMonth, 'f', 0));
    deleteEmployeeIdEdit->setText(QString::number(e.employeeID));
    attendanceEmployeeIdEdit->setText(QString::number(e.employeeID));
    salaryEmployeeIdEdit->setText(QString::number(e.employeeID));
}

void MainWindow::clearEmployeeForm()
{
    employeeIdEdit->clear();
    employeeNameEdit->clear();
    employeeUsernameFormEdit->clear();
    employeePasswordFormEdit->clear();
    employeeAgeEdit->clear();
    employeePhoneEdit->clear();
    employeeRoleEdit->clear();
    employeeSalaryEdit->clear();
    employeeTotalHoursEdit->clear();
    employeeMonthlyHoursEdit->clear();
}

void MainWindow::attemptAdminLogin()
{
    const int index = findAdminIndexByCredentials(adminUsernameEdit->text(), adminPasswordEdit->text());
    if (index == -1) {
        QMessageBox::warning(this, "Login Failed", "Invalid admin username or password.");
        return;
    }

    loggedInAdminIndex = index;
    setAdminLoggedIn(true);
}

void MainWindow::attemptEmployeeLogin()
{
    const int index = findEmployeeIndexByCredentials(
        employeeLoginUsernameEdit->text(),
        employeeLoginPasswordEdit->text());

    if (index == -1) {
        QMessageBox::warning(this, "Login Failed", "Invalid employee username or password.");
        return;
    }

    loggedInEmployeeIndex = index;
    setEmployeeLoggedIn(true);
    refreshEmployeeView();
}

void MainWindow::handleAddEmployee()
{
    Employee employee;
    QString message;

    if (!readEmployeeFromForm(
            employeeIdEdit,
            employeeNameEdit,
            employeeUsernameFormEdit,
            employeePasswordFormEdit,
            employeeAgeEdit,
            employeePhoneEdit,
            employeeRoleEdit,
            employeeSalaryEdit,
            employeeTotalHoursEdit,
            employeeMonthlyHoursEdit,
            employee,
            message)) {
        QMessageBox::warning(this, "Invalid Data", message);
        return;
    }

    if (!addEmployee(employee, message)) {
        QMessageBox::warning(this, "Add Employee", message);
        return;
    }

    QString salaryMessage;
    calculateSalary(employee.employeeID, salaryMessage);
    refreshEmployeeTable();
    clearEmployeeForm();
    QMessageBox::information(this, "Add Employee", message);
}

void MainWindow::handleUpdateEmployee()
{
    Employee employee;
    QString message;

    if (!readEmployeeFromForm(
            employeeIdEdit,
            employeeNameEdit,
            employeeUsernameFormEdit,
            employeePasswordFormEdit,
            employeeAgeEdit,
            employeePhoneEdit,
            employeeRoleEdit,
            employeeSalaryEdit,
            employeeTotalHoursEdit,
            employeeMonthlyHoursEdit,
            employee,
            message)) {
        QMessageBox::warning(this, "Invalid Data", message);
        return;
    }

    if (!updateEmployee(employee, message)) {
        QMessageBox::warning(this, "Update Employee", message);
        return;
    }

    QString salaryMessage;
    calculateSalary(employee.employeeID, salaryMessage);
    refreshEmployeeTable();
    refreshEmployeeView();
    QMessageBox::information(this, "Update Employee", message);
}

void MainWindow::handleDeleteEmployee()
{
    long long employeeId = 0;
    long long loggedInEmployeeId = -1;

    if (loggedInEmployeeIndex >= 0 && loggedInEmployeeIndex < employeeCount) {
        loggedInEmployeeId = employees[loggedInEmployeeIndex].employeeID;
    }

    if (!parseLongLong(deleteEmployeeIdEdit->text(), employeeId)) {
        QMessageBox::warning(this, "Delete Employee", "Please enter a valid employee ID.");
        return;
    }

    QString message;
    if (!deleteEmployee(employeeId, message)) {
        QMessageBox::warning(this, "Delete Employee", message);
        return;
    }

    if (loggedInEmployeeId == employeeId) {
        setEmployeeLoggedIn(false);
    }

    refreshEmployeeTable();
    clearEmployeeForm();
    deleteEmployeeIdEdit->clear();
    QMessageBox::information(this, "Delete Employee", message);
}

void MainWindow::handleRecordAttendance()
{
    Attendance attendance;
    QString message;

    if (!parseLongLong(attendanceEmployeeIdEdit->text(), attendance.employeeID) ||
        !parseInt(attendanceMonthEdit->text(), attendance.month) ||
        !parseInt(attendancePresentEdit->text(), attendance.daysPresent) ||
        !parseInt(attendanceAbsentEdit->text(), attendance.daysAbsent)) {
        QMessageBox::warning(this, "Attendance", "Please enter valid attendance numbers.");
        return;
    }

    if (!recordAttendance(attendance, message)) {
        QMessageBox::warning(this, "Attendance", message);
        return;
    }

    if (loggedInEmployeeIndex >= 0 &&
        employees[loggedInEmployeeIndex].employeeID == attendance.employeeID) {
        refreshEmployeeView();
    }

    QMessageBox::information(this, "Attendance", message);
}

void MainWindow::handleCalculateSalary()
{
    long long employeeId = 0;
    if (!parseLongLong(salaryEmployeeIdEdit->text(), employeeId)) {
        QMessageBox::warning(this, "Salary", "Please enter a valid employee ID.");
        return;
    }

    QString message;
    if (!calculateSalary(employeeId, message)) {
        QMessageBox::warning(this, "Salary", message);
        return;
    }

    refreshEmployeeTable();
    refreshEmployeeView();
    QMessageBox::information(this, "Salary", message);
}
