#ifndef AUTOCREATE_P_H
#define AUTOCREATE_P_H

#include <QObject>

class AutoCreatePrivate : public QObject
{

public:
    AutoCreatePrivate()
        : className(""), haveSignalValue(false),
          isDll(false), havePrivate(false), previewIndex(0), previewData("")
    { }

    QString className;
    bool haveSignalValue;
    bool isDll;
    bool havePrivate;

    int previewIndex;

    QString previewData;
    QString parsData;

};

#endif // AUTOCREATE_P_H
