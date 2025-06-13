#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "resource.h"
#include "book.h"
#include "article.h"
#include "thesis.h"
#include "digitalcontent.h"
#include <QDebug>
#include <QTableWidgetItem>
#include <QInputDialog>
#include <QDate>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "addbookdialog.h"
#include "historyentry.h"
#include "scheduledialog.h"
#include "user.h"

// --- Notification function ---
void MainWindow::checkNotifications()
{
    QStringList alerts;
    QDate today = QDate::currentDate();

    for (Resource* res : resources) {
        // Overdue items
        if (res->getStatus() == "Borrowed" && res->getDueDate().isValid() && res->getDueDate() < today) {
            alerts << QString("Overdue: '%1' borrowed by %2 was due on %3.")
            .arg(res->getTitle())
                .arg(res->getBorrower())
                .arg(res->getDueDate().toString("yyyy-MM-dd"));
        }
        // Due soon (within 2 days)
        else if (res->getStatus() == "Borrowed" && res->getDueDate().isValid() && res->getDueDate() <= today.addDays(2)) {
            alerts << QString("Due soon: '%1' borrowed by %2 is due on %3.")
            .arg(res->getTitle())
                .arg(res->getBorrower())
                .arg(res->getDueDate().toString("yyyy-MM-dd"));
        }
        // Reserved items now available
        if (res->getStatus() == "Available" && !res->getReserver().isEmpty()) {
            alerts << QString("Reserved item available: '%1' reserved by %2 is now available.")
            .arg(res->getTitle())
                .arg(res->getReserver());
        }
    }

    if (!alerts.isEmpty()) {
        QMessageBox::information(this, "Library Notifications", alerts.join("\n\n"));
    }
}

MainWindow::MainWindow(QWidget *parent, const User& user)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentUser(user) // Use the user passed from LoginDialog
{
    ui->setupUi(this);

    // Display logged-in user's name and role
    ui->labelUserInfo->setText(
        QString("Logged in as: %1 (%2)")
            .arg(currentUser.getName())
            .arg(currentUser.getRole())
        );

    // Removed Manage Users button and its connection

    ui->tableWidgetResources->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Add filter options to combo boxes
    ui->comboBoxCategory->addItem("All");
    ui->comboBoxCategory->addItem("Book");
    ui->comboBoxCategory->addItem("Article");
    ui->comboBoxCategory->addItem("Thesis");
    ui->comboBoxCategory->addItem("Digital");

    ui->comboBoxStatus->addItem("All");
    ui->comboBoxStatus->addItem("Available");
    ui->comboBoxStatus->addItem("Borrowed");
    ui->comboBoxStatus->addItem("Reserved");

    // Set up table columns (8 columns, including Due Date)
    ui->tableWidgetResources->setColumnCount(8);
    QStringList headers;
    headers << "ID" << "Title" << "Author" << "Category" << "Status" << "Borrower" << "Reserver" << "Due Date";
    ui->tableWidgetResources->setHorizontalHeaderLabels(headers);

    loadResourcesFromFile("resources.json");
    loadHistoryFromFile("history.json");
    loadUsersFromFile("users.json");

    connect(ui->buttonAdd, &QPushButton::clicked, this, &MainWindow::onAddResourceClicked);
    connect(ui->buttonRemove, &QPushButton::clicked, this, &MainWindow::onRemoveResourceClicked);
    connect(ui->buttonEdit, &QPushButton::clicked, this, &MainWindow::onEditResourceClicked);
    connect(ui->buttonSearch, &QPushButton::clicked, this, &MainWindow::onSearchClicked);
    connect(ui->buttonBorrow, &QPushButton::clicked, this, &MainWindow::onBorrowResourceClicked);
    connect(ui->buttonReserve, &QPushButton::clicked, this, &MainWindow::onReserveResourceClicked);
    connect(ui->buttonReturn, &QPushButton::clicked, this, &MainWindow::onReturnResourceClicked);
    connect(ui->buttonRenew, &QPushButton::clicked, this, &MainWindow::onRenewResourceClicked);
    connect(ui->buttonViewHistory, &QPushButton::clicked, this, &MainWindow::onViewHistoryClicked);
    connect(ui->buttonSchedule, &QPushButton::clicked, this, &MainWindow::onScheduleClicked);

    // Only add a sample book if the library is empty
    if (resources.isEmpty()) {
        Resource* b = new Book(1, "C++ Primer", "Lippman", 2012);
        resources.append(b);

        int row = ui->tableWidgetResources->rowCount();
        ui->tableWidgetResources->insertRow(row);
        ui->tableWidgetResources->setItem(row, 0, new QTableWidgetItem(QString::number(b->getId())));
        ui->tableWidgetResources->setItem(row, 1, new QTableWidgetItem(b->getTitle()));
        ui->tableWidgetResources->setItem(row, 2, new QTableWidgetItem(b->getAuthor()));
        ui->tableWidgetResources->setItem(row, 3, new QTableWidgetItem(b->getCategory()));
        ui->tableWidgetResources->setItem(row, 4, new QTableWidgetItem(b->getStatus()));
        ui->tableWidgetResources->setItem(row, 5, new QTableWidgetItem(b->getBorrower()));
        ui->tableWidgetResources->setItem(row, 6, new QTableWidgetItem(b->getReserver()));
        ui->tableWidgetResources->setItem(row, 7, new QTableWidgetItem(""));
    }

    checkNotifications();
}

void MainWindow::onAddResourceClicked()
{
    QStringList types = {"Book", "Article", "Thesis", "Digital"};
    bool ok;
    QString type = QInputDialog::getItem(this, "Select Resource Type", "Type:", types, 0, false, &ok);
    if (!ok || type.isEmpty()) return;

    AddBookDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        if (dialog.getTitle().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Title cannot be empty.");
            return;
        }
        int newId = resources.size() + 1;
        Resource* res = nullptr;
        if (type == "Book")
            res = new Book(newId, dialog.getTitle(), dialog.getAuthor(), dialog.getYear());
        else if (type == "Article")
            res = new Article(newId, dialog.getTitle(), dialog.getAuthor(), dialog.getYear());
        else if (type == "Thesis")
            res = new Thesis(newId, dialog.getTitle(), dialog.getAuthor(), dialog.getYear());
        else if (type == "Digital")
            res = new DigitalContent(newId, dialog.getTitle(), dialog.getAuthor(), dialog.getYear());
        else
            return;

        resources.append(res);

        int row = ui->tableWidgetResources->rowCount();
        ui->tableWidgetResources->insertRow(row);
        ui->tableWidgetResources->setItem(row, 0, new QTableWidgetItem(QString::number(res->getId())));
        ui->tableWidgetResources->setItem(row, 1, new QTableWidgetItem(res->getTitle()));
        ui->tableWidgetResources->setItem(row, 2, new QTableWidgetItem(res->getAuthor()));
        ui->tableWidgetResources->setItem(row, 3, new QTableWidgetItem(res->getCategory()));
        ui->tableWidgetResources->setItem(row, 4, new QTableWidgetItem(res->getStatus()));
        ui->tableWidgetResources->setItem(row, 5, new QTableWidgetItem(res->getBorrower()));
        ui->tableWidgetResources->setItem(row, 6, new QTableWidgetItem(res->getReserver()));
        ui->tableWidgetResources->setItem(row, 7, new QTableWidgetItem(res->getDueDate().isValid() ? res->getDueDate().toString("yyyy-MM-dd") : ""));
    }
}

void MainWindow::onRemoveResourceClicked()
{
    int row = ui->tableWidgetResources->currentRow();
    if (row < 0 || row >= resources.size()) {
        QMessageBox::warning(this, "Selection Error", "Please select a valid resource to remove.");
        return;
    }

    delete resources[row];
    resources.removeAt(row);
    ui->tableWidgetResources->removeRow(row);
}

void MainWindow::onEditResourceClicked()
{
    int row = ui->tableWidgetResources->currentRow();
    if (row < 0 || row >= resources.size()) {
        QMessageBox::warning(this, "Selection Error", "Please select a valid resource to edit.");
        return;
    }

    Resource* res = resources[row];

    AddBookDialog dialog(this);
    dialog.findChild<QLineEdit*>("lineEditTitle")->setText(res->getTitle());
    dialog.findChild<QLineEdit*>("lineEditAuthor")->setText(res->getAuthor());
    dialog.findChild<QLineEdit*>("lineEditYear")->setText(QString::number(res->getYear()));

    if (dialog.exec() == QDialog::Accepted) {
        if (dialog.getTitle().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Title cannot be empty.");
            return;
        }
        res->setTitle(dialog.getTitle());
        res->setAuthor(dialog.getAuthor());
        res->setYear(dialog.getYear());
        ui->tableWidgetResources->setItem(row, 1, new QTableWidgetItem(res->getTitle()));
        ui->tableWidgetResources->setItem(row, 2, new QTableWidgetItem(res->getAuthor()));
        ui->tableWidgetResources->setItem(row, 3, new QTableWidgetItem(res->getCategory()));
        ui->tableWidgetResources->setItem(row, 7, new QTableWidgetItem(res->getDueDate().isValid() ? res->getDueDate().toString("yyyy-MM-dd") : ""));
    }
}

void MainWindow::onSearchClicked()
{
    QString searchText = ui->lineEditSearch->text().trimmed().toLower();
    QString selectedCategory = ui->comboBoxCategory->currentText();
    QString selectedStatus = ui->comboBoxStatus->currentText();

    ui->tableWidgetResources->setRowCount(0);

    for (Resource* res : resources) {
        bool matches = true;

        if (selectedCategory != "All" && res->getCategory() != selectedCategory)
            matches = false;

        if (selectedStatus != "All" && res->getStatus() != selectedStatus)
            matches = false;

        if (!searchText.isEmpty() &&
            !res->getTitle().toLower().contains(searchText) &&
            !res->getAuthor().toLower().contains(searchText))
            matches = false;

        if (matches) {
            int row = ui->tableWidgetResources->rowCount();
            ui->tableWidgetResources->insertRow(row);
            ui->tableWidgetResources->setItem(row, 0, new QTableWidgetItem(QString::number(res->getId())));
            ui->tableWidgetResources->setItem(row, 1, new QTableWidgetItem(res->getTitle()));
            ui->tableWidgetResources->setItem(row, 2, new QTableWidgetItem(res->getAuthor()));
            ui->tableWidgetResources->setItem(row, 3, new QTableWidgetItem(res->getCategory()));
            ui->tableWidgetResources->setItem(row, 4, new QTableWidgetItem(res->getStatus()));
            ui->tableWidgetResources->setItem(row, 5, new QTableWidgetItem(res->getBorrower()));
            ui->tableWidgetResources->setItem(row, 6, new QTableWidgetItem(res->getReserver()));
            ui->tableWidgetResources->setItem(row, 7, new QTableWidgetItem(res->getDueDate().isValid() ? res->getDueDate().toString("yyyy-MM-dd") : ""));
        }
    }
}

void MainWindow::onBorrowResourceClicked()
{
    int row = ui->tableWidgetResources->currentRow();
    if (row < 0 || row >= resources.size()) {
        QMessageBox::warning(this, "Selection Error", "Please select a valid resource to borrow.");
        return;
    }
    Resource* res = resources[row];
    if (res->getStatus() == "Borrowed") {
        QMessageBox::information(this, "Already Borrowed", "This resource is already borrowed.");
        return;
    }

    QString borrower = QInputDialog::getText(this, "Borrow Resource", "Enter borrower name:");
    if (borrower.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Borrower name cannot be empty.");
        return;
    }

    QDate dueDate = QDate::currentDate().addDays(14);
    res->setStatus("Borrowed");
    res->setBorrower(borrower);
    res->setDueDate(dueDate);

    ui->tableWidgetResources->setItem(row, 4, new QTableWidgetItem("Borrowed"));
    ui->tableWidgetResources->setItem(row, 5, new QTableWidgetItem(borrower));
    ui->tableWidgetResources->setItem(row, 7, new QTableWidgetItem(dueDate.toString("yyyy-MM-dd")));

    history.append(HistoryEntry(res->getTitle(), borrower, "Borrowed", QDateTime::currentDateTime()));

    QMessageBox::information(this, "Borrowed", "Resource borrowed by " + borrower + "\nDue date: " + dueDate.toString("yyyy-MM-dd"));

    checkNotifications();
}

void MainWindow::onReserveResourceClicked()
{
    int row = ui->tableWidgetResources->currentRow();
    if (row < 0 || row >= resources.size()) {
        QMessageBox::warning(this, "Selection Error", "Please select a valid resource to reserve.");
        return;
    }
    Resource* res = resources[row];
    if (res->getStatus() != "Borrowed") {
        QMessageBox::information(this, "Not Borrowed", "Only borrowed resources can be reserved.");
        return;
    }

    QString reserver = QInputDialog::getText(this, "Reserve Resource", "Enter your name:");
    if (reserver.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Reserver name cannot be empty.");
        return;
    }

    res->setStatus("Reserved");
    res->setReserver(reserver);

    ui->tableWidgetResources->setItem(row, 4, new QTableWidgetItem("Reserved"));
    ui->tableWidgetResources->setItem(row, 6, new QTableWidgetItem(reserver));

    QMessageBox::information(this, "Reserved", "Resource reserved by " + reserver);

    checkNotifications();
}

void MainWindow::onReturnResourceClicked()
{
    int row = ui->tableWidgetResources->currentRow();
    if (row < 0 || row >= resources.size()) {
        QMessageBox::warning(this, "Selection Error", "Please select a valid resource to return.");
        return;
    }
    Resource* res = resources[row];
    QString status = res->getStatus();
    if (status != "Borrowed" && status != "Reserved") {
        QMessageBox::information(this, "Not Borrowed/Reserved", "This resource is neither borrowed nor reserved.");
        return;
    }

    QString lastBorrower = res->getBorrower();
    history.append(HistoryEntry(res->getTitle(), lastBorrower, "Returned", QDateTime::currentDateTime()));

    res->setStatus("Available");
    res->setBorrower("");
    res->setReserver("");
    res->setDueDate(QDate());

    ui->tableWidgetResources->setItem(row, 4, new QTableWidgetItem("Available"));
    ui->tableWidgetResources->setItem(row, 5, new QTableWidgetItem(""));
    ui->tableWidgetResources->setItem(row, 6, new QTableWidgetItem(""));
    ui->tableWidgetResources->setItem(row, 7, new QTableWidgetItem(""));

    QMessageBox::information(this, "Returned", "Resource has been returned and is now available.");

    checkNotifications();
}

void MainWindow::saveResourcesToFile(const QString& filename)
{
    QJsonArray resourcesArray;
    for (int i = 0; i < resources.size(); ++i) {
        Resource* res = resources[i];
        QJsonObject obj;
        obj["id"] = res->getId();
        obj["title"] = res->getTitle();
        obj["author"] = res->getAuthor();
        obj["year"] = res->getYear();
        obj["category"] = res->getCategory();
        obj["status"] = res->getStatus();
        obj["borrower"] = res->getBorrower();
        obj["reserver"] = res->getReserver();
        obj["dueDate"] = res->getDueDate().isValid() ? res->getDueDate().toString("yyyy-MM-dd") : "";

        resourcesArray.append(obj);
    }
    QJsonDocument doc(resourcesArray);
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "File Error", "Could not open " + filename + " for writing.");
        return;
    }
    file.write(doc.toJson());
    file.close();
}

void MainWindow::loadResourcesFromFile(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "File Error", "Could not open " + filename + " for reading.");
        return;
    }
    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isArray()) {
        QMessageBox::warning(this, "File Error", "Invalid or corrupted resource file.");
        return;
    }
    QJsonArray resourcesArray = doc.array();
    qDeleteAll(resources);
    resources.clear();
    ui->tableWidgetResources->setRowCount(0);

    for (const QJsonValue& val : resourcesArray) {
        QJsonObject obj = val.toObject();
        Resource* res = nullptr;
        QString category = obj["category"].toString();
        if (category == "Book")
            res = new Book(obj["id"].toInt(), obj["title"].toString(), obj["author"].toString(), obj["year"].toInt());
        else if (category == "Article")
            res = new Article(obj["id"].toInt(), obj["title"].toString(), obj["author"].toString(), obj["year"].toInt());
        else if (category == "Thesis")
            res = new Thesis(obj["id"].toInt(), obj["title"].toString(), obj["author"].toString(), obj["year"].toInt());
        else if (category == "Digital")
            res = new DigitalContent(obj["id"].toInt(), obj["title"].toString(), obj["author"].toString(), obj["year"].toInt());
        else
            continue;

        res->setStatus(obj["status"].toString());
        res->setBorrower(obj["borrower"].toString());
        res->setReserver(obj["reserver"].toString());
        QString dueDateStr = obj["dueDate"].toString();
        if (!dueDateStr.isEmpty())
            res->setDueDate(QDate::fromString(dueDateStr, "yyyy-MM-dd"));
        else
            res->setDueDate(QDate());

        resources.append(res);

        int row = ui->tableWidgetResources->rowCount();
        ui->tableWidgetResources->insertRow(row);
        ui->tableWidgetResources->setItem(row, 0, new QTableWidgetItem(QString::number(res->getId())));
        ui->tableWidgetResources->setItem(row, 1, new QTableWidgetItem(res->getTitle()));
        ui->tableWidgetResources->setItem(row, 2, new QTableWidgetItem(res->getAuthor()));
        ui->tableWidgetResources->setItem(row, 3, new QTableWidgetItem(res->getCategory()));
        ui->tableWidgetResources->setItem(row, 4, new QTableWidgetItem(res->getStatus()));
        ui->tableWidgetResources->setItem(row, 5, new QTableWidgetItem(res->getBorrower()));
        ui->tableWidgetResources->setItem(row, 6, new QTableWidgetItem(res->getReserver()));
        ui->tableWidgetResources->setItem(row, 7, new QTableWidgetItem(res->getDueDate().isValid() ? res->getDueDate().toString("yyyy-MM-dd") : ""));
    }
}

void MainWindow::onRenewResourceClicked()
{
    int row = ui->tableWidgetResources->currentRow();
    if (row < 0 || row >= resources.size()) {
        QMessageBox::warning(this, "Selection Error", "Please select a valid resource to renew.");
        return;
    }

    Resource* res = resources[row];

    if (res->getStatus() != "Borrowed") {
        QMessageBox::warning(this, "Renewal Not Allowed", "Only borrowed resources can be renewed.");
        return;
    }

    if (!res->getReserver().isEmpty()) {
        QMessageBox::warning(this, "Renewal Not Allowed", "This resource is reserved by another user and cannot be renewed.");
        return;
    }

    QDate newDueDate = res->getDueDate().addDays(14);
    res->setDueDate(newDueDate);

    ui->tableWidgetResources->setItem(row, 7, new QTableWidgetItem(newDueDate.toString("yyyy-MM-dd")));

    QMessageBox::information(this, "Renewed", "Resource renewed. New due date: " + newDueDate.toString("yyyy-MM-dd"));

    checkNotifications();
}

void MainWindow::onViewHistoryClicked()
{
    QString historyText;
    for (const HistoryEntry& entry : history) {
        historyText += QString("%1 | %2 | %3 | %4\n")
        .arg(entry.resourceTitle)
            .arg(entry.user)
            .arg(entry.action)
            .arg(entry.dateTime.toString("yyyy-MM-dd HH:mm"));
    }
    QMessageBox::information(this, "Borrow/Return History", historyText.isEmpty() ? "No history yet." : historyText);
}

void MainWindow::onScheduleClicked() {
    ScheduleDialog dlg(this);
    dlg.exec();
}

void MainWindow::saveHistoryToFile(const QString& filename)
{
    QJsonArray arr;
    for (const HistoryEntry& entry : history) {
        arr.append(entry.toJson());
    }
    QJsonDocument doc(arr);
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "File Error", "Could not open " + filename + " for writing.");
        return;
    }
    file.write(doc.toJson());
    file.close();
}

void MainWindow::loadHistoryFromFile(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "File Error", "Could not open " + filename + " for reading.");
        return;
    }
    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isArray()) {
        QMessageBox::warning(this, "File Error", "Invalid or corrupted history file.");
        return;
    }
    QJsonArray arr = doc.array();
    history.clear();
    for (const QJsonValue& val : arr) {
        history.append(HistoryEntry::fromJson(val.toObject()));
    }
}
// Save users to file
void MainWindow::saveUsersToFile(const QString& filename)
{
    QJsonArray arr;
    for (const User& user : users) {
        QJsonObject obj;
        obj["id"] = user.getId();
        obj["name"] = user.getName();
        obj["email"] = user.getEmail();
        obj["role"] = user.getRole();
        arr.append(obj);
    }
    QJsonDocument doc(arr);
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "File Error", "Could not open " + filename + " for writing.");
        return;
    }
    file.write(doc.toJson());
    file.close();
}

// Load users from file
void MainWindow::loadUsersFromFile(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        // It's OK if the file doesn't exist yet
        return;
    }
    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) return;
    QJsonArray arr = doc.array();
    users.clear();
    for (const QJsonValue& val : arr) {
        QJsonObject obj = val.toObject();
        users.append(User(
            obj["id"].toInt(),
            obj["name"].toString(),
            obj["email"].toString(),
            obj["role"].toString()
            ));
    }
}

MainWindow::~MainWindow()
{
    saveResourcesToFile("resources.json");
    saveHistoryToFile("history.json");
    saveUsersToFile("users.json");
    qDeleteAll(resources);
    resources.clear();
    delete ui;
}
