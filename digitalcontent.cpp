#include "digitalcontent.h"

DigitalContent::DigitalContent(int id, const QString& title, const QString& author, int year)
    : Resource(id, title, author, year, "Digital") {}
