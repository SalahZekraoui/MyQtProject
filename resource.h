#pragma once
#include <QString>
#include <QDate>

class Resource {
protected:
    int id;
    QString title;
    QString author;
    int year;
    QString category; // e.g., "Book", "Article", "Thesis", "Digital"
    QString status;   // "Available", "Borrowed", "Reserved"
    QString borrower;
    QString reserver;
    QDate dueDate;
public:
    Resource(int id, const QString& title, const QString& author, int year, const QString& category);
    virtual ~Resource();

    int getId() const;
    QString getTitle() const;
    QString getAuthor() const;
    int getYear() const;
    QString getCategory() const;
    QString getStatus() const;
    QString getBorrower() const;
    QString getReserver() const;
    void setTitle(const QString& t);
    void setAuthor(const QString& a);
    void setYear(int y);

    void setStatus(const QString& s);
    void setBorrower(const QString& name);
    void setReserver(const QString& name);
    QDate getDueDate() const;
    void setDueDate(const QDate& date);
};
