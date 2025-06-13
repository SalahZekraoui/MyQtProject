#pragma once
#include "resource.h"

class Book : public Resource {
public:
    Book(int id, const QString& title, const QString& author, int year);
};
