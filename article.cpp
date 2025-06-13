#include "article.h"

Article::Article(int id, const QString& title, const QString& author, int year)
    : Resource(id, title, author, year, "Article") {}
