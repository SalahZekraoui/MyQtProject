#pragma once
#include "resource.h"

class Article : public Resource {
public:
    Article(int id, const QString& title, const QString& author, int year);
};
