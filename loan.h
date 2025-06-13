#pragma once
#include <QDate>

class Loan {
private:
    int loanId;
    int bookId;
    int userId;
    QDate borrowDate;
    QDate dueDate;
    bool returned;
public:
    Loan(int loanId, int bookId, int userId, QDate borrowDate, QDate dueDate);
    int getLoanId() const;
    int getBookId() const;
    int getUserId() const;
    QDate getBorrowDate() const;
    QDate getDueDate() const;
    bool isReturned() const;
    void setReturned(bool returned);
};
