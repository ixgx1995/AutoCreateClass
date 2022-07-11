#ifndef AUTOCREATE_H
#define AUTOCREATE_H

#include <QObject>

class AutoCreatePrivate;
//previewIndex
class AutoCreate : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString className READ className WRITE setClassName NOTIFY classNameChanged)
    Q_PROPERTY(bool haveSignalValue READ haveSignalValue WRITE setHaveSignalValue NOTIFY haveSignalValueChanged)
    Q_PROPERTY(bool isDll READ isDll WRITE setIsDll NOTIFY isDllChanged)
    Q_PROPERTY(bool havePrivate READ havePrivate WRITE setHavePrivate NOTIFY havePrivateChanged)
    Q_PROPERTY(int previewIndex READ previewIndex WRITE setPreviewIndex NOTIFY previewIndexChanged)
    Q_PROPERTY(QString previewData READ previewData WRITE setPreviewData NOTIFY previewDataChanged)
    Q_PROPERTY(QString parsData READ parsData WRITE setParsData NOTIFY parsDataChanged)
    Q_DECLARE_PRIVATE(AutoCreate)

public:
    AutoCreate(QObject *parent = nullptr);
    ~AutoCreate();

    QString className() const;
    void setClassName(QString value);

    bool haveSignalValue() const;
    void setHaveSignalValue(bool value);

    bool isDll() const;
    void setIsDll(bool value);

    bool havePrivate() const;
    void setHavePrivate(bool value);

    int previewIndex() const;
    void setPreviewIndex(int value);

    QString previewData() const;
    void setPreviewData(QString value);

    QString parsData() const;
    void setParsData(QString value);
    QStringList pars();

    Q_INVOKABLE void preview();
    Q_INVOKABLE void create(QString path = nullptr);

    QString createH();
    QString createCpp();
    QString createPrivate();

signals:
    void classNameChanged();
    void haveSignalValueChanged();
    void isDllChanged();
    void havePrivateChanged();
    void previewIndexChanged();
    void previewDataChanged();
    void parsDataChanged();

private:
    QString strFirstU(QString data = nullptr);
    QString tab();

protected:
    QScopedPointer<AutoCreatePrivate> d_ptr;
};

#endif // AUTOCREATE_H
