#include "resource.h"

Resource::Resource(int id, const QString& title, const QString& author, int year, const QString& category)
    : id(id), title(title), author(author), year(year), category(category), status("Available"), borrower(""), reserver("") {}

Resource::~Resource() {}

int Resource::getId() const { return id; }
QString Resource::getTitle() const { return title; }
QString Resource::getAuthor() const { return author; }
int Resource::getYear() const { return year; }
QString Resource::getCategory() const { return category; }
QString Resource::getStatus() const { return status; }
QString Resource::getBorrower() const { return borrower; }
QString Resource::getReserver() const { return reserver; }

void Resource::setStatus(const QString& s) { status = s; }
void Resource::setBorrower(const QString& name) { borrower = name; }
void Resource::setReserver(const QString& name) { reserver = name; }
void Resource::setTitle(const QString& t) { title = t; }
void Resource::setAuthor(const QString& a) { author = a; }
void Resource::setYear(int y) { year = y; }
QDate Resource::getDueDate() const { return dueDate; }
void Resource::setDueDate(const QDate& date) { dueDate = date; }
