#include "loan.h"

Loan::Loan(int loanId, int bookId, int userId, QDate borrowDate, QDate dueDate)
    : loanId(loanId), bookId(bookId), userId(userId), borrowDate(borrowDate), dueDate(dueDate), returned(false) {}

int Loan::getLoanId() const { return loanId; }
int Loan::getBookId() const { return bookId; }
int Loan::getUserId() const { return userId; }
QDate Loan::getBorrowDate() const { return borrowDate; }
QDate Loan::getDueDate() const { return dueDate; }
bool Loan::isReturned() const { return returned; }
void Loan::setReturned(bool r) { returned = r; }
