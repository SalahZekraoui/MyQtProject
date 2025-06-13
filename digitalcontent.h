#pragma once
#include "resource.h"

class DigitalContent : public Resource {
public:
    DigitalContent(int id, const QString& title, const QString& author, int year);
};
