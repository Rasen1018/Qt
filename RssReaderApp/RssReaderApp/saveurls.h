#ifndef SAVEURLS_H
#define SAVEURLS_H
#include <QStringList>

class SaveUrls
{
public:
    SaveUrls();

    void save(QStringList);
    QStringList load();
};

#endif // SAVEURLS_H
