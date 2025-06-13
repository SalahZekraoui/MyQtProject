#pragma once
#include "resource.h"

class Thesis : public Resource {
public:
    Thesis(int id, const QString& title, const QString& author, int year);
};
