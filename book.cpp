#include "book.h"

Book::Book(int id, const QString& title, const QString& author, int year)
    : Resource(id, title, author, year, "Book") {}
