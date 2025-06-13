#include "thesis.h"

Thesis::Thesis(int id, const QString& title, const QString& author, int year)
    : Resource(id, title, author, year, "Thesis") {}
