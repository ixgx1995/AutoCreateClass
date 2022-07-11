
#include "autocreate_p.h"
#include "autocreate.h"
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QApplication>

AutoCreate::AutoCreate(QObject *parent)
    : QObject(parent), d_ptr(new AutoCreatePrivate)
{
}

AutoCreate::~AutoCreate()
{
}

QString AutoCreate::className() const
{
    return d_ptr->className;
}

void AutoCreate::setClassName(QString value)
{
    if(d_ptr->className == value)
        return;
    d_ptr->className = value;
    emit classNameChanged();
}

bool AutoCreate::haveSignalValue() const
{
    return d_ptr->haveSignalValue;
}

void AutoCreate::setHaveSignalValue(bool value)
{
    if(d_ptr->haveSignalValue == value)
        return;
    d_ptr->haveSignalValue = value;
    emit haveSignalValueChanged();
}

bool AutoCreate::isDll() const
{
    return d_ptr->isDll;
}

void AutoCreate::setIsDll(bool value)
{
    if(d_ptr->isDll == value)
        return;
    d_ptr->isDll = value;
    emit isDllChanged();
}

bool AutoCreate::havePrivate() const
{
    return d_ptr->havePrivate;
}

void AutoCreate::setHavePrivate(bool value)
{
    if(d_ptr->havePrivate == value)
        return;
    d_ptr->havePrivate = value;
    emit havePrivateChanged();
}

int AutoCreate::previewIndex() const
{
    return d_ptr->previewIndex;
}

void AutoCreate::setPreviewIndex(int value)
{
    if(d_ptr->previewIndex == value)
        return;
    d_ptr->previewIndex = value;
    emit previewIndexChanged();
}

QString AutoCreate::previewData() const
{
    return d_ptr->previewData;
}

void AutoCreate::setPreviewData(QString value)
{
    if(d_ptr->previewData == value)
        return;
    d_ptr->previewData = value;
    emit previewDataChanged();
}

QString AutoCreate::parsData() const
{
    return d_ptr->parsData;
}

void AutoCreate::setParsData(QString value)
{
    if(d_ptr->parsData == value)
        return;
    d_ptr->parsData = value;
    emit parsDataChanged();
}

QStringList AutoCreate::pars()
{
    return d_ptr->parsData.split("\n",Qt::SkipEmptyParts);
}

QString AutoCreate::strFirstU(QString data)
{
    auto d = className();
    if(data != nullptr)
        d = data;
    auto c = QString(d[0]);
    d = d.remove(0,1);
    d.insert(0,c.toUpper());
    return d;
}

QString AutoCreate::tab()
{
    return "    ";
}

void AutoCreate::preview()
{
    QString data;
    switch (previewIndex()) {
    case 0:data = createH();break;
    case 1:data = createCpp();break;
    case 2:data = createPrivate();break;
    }
    setPreviewData(data);
}

void AutoCreate::create(QString path)
{
    if(path.isEmpty()) {
        path = QApplication::applicationDirPath() + "/file";
    }
    QDir dir(path);
    if(!dir.exists()){
        bool res = dir.mkpath(path);
        if(!res) {
            QMessageBox::information(NULL, "错误提示", "创建文件夹失败！");
            return;
        }
    }

    //create H file
    QFile fileH(QString(path + "/%1.h").arg(className().toLower()));
    if(!fileH.open(QIODevice::WriteOnly | QIODevice::Text)){
        QMessageBox::information(NULL, "错误提示", "创建H文件失败!");
        return;
    }
    fileH.write(createH().toUtf8());
    fileH.close();

    //create Cpp file
    QFile fileCpp(QString(path + "/%1.cpp").arg(className().toLower()));
    if(!fileCpp.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::information(NULL, "错误提示", "创建Cpp文件失败!");
        return;
    }
    fileCpp.write(createCpp().toUtf8());
    fileCpp.close();
    //create Private file
    if(havePrivate()) {
        path += "/private";
        QDir dir(path);
        if(!dir.exists()){
            bool res = dir.mkpath(path);
            if(!res) {
                QMessageBox::information(NULL, "错误提示", "创建Private文件夹失败!");
                return;
            }
        }
        QFile filePrivate(QString(path + "/%1_p.h").arg(className().toLower()));
        if(!filePrivate.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::information(NULL, "错误提示", "创建Private文件失败!");
            return;
        }
        filePrivate.write(createPrivate().toUtf8());
        filePrivate.close();
    }
}

QString AutoCreate::createH()
{
    QString data;
    data.append(QString("#ifndef %1_H").arg(className().toUpper()) + "\n");
    data.append(QString("#define %1_H").arg(className().toUpper()) + "\n");
    data.append("\n");
    if(isDll())
        data.append(QString("#include \"%1_global.h\"").arg(className().toLower()) + "\n");
    data.append("#include <QObject>");
    data.append("\n");
    if(havePrivate())
        data.append(QString("\nclass %1Private;").arg(strFirstU()) + "\n");
    data.append("\n");
    data.append(QString("class %1%2 : public QObject").arg(isDll()? (QString("%1_EXPORT ").arg(className().toUpper())) : "" , strFirstU()) + "\n");
    data.append("{\n");
    data.append(QString("%1Q_OBJECT\n").arg(tab()));
    foreach(QString par, pars()){
        auto pardata = par.replace(";","");
        auto list = pardata.split(" ", Qt::SkipEmptyParts);
        auto ptype = list.first();
        auto pname = list.last();
        data.append(QString("%5Q_PROPERTY(%1 %2 READ %2 WRITE set%3 NOTIFY %4Changed)").arg(ptype,pname,strFirstU(pname),pname,tab()) + "\n");
    }
    if(havePrivate())
        data.append(QString("%5Q_DECLARE_PRIVATE(%1)").arg(strFirstU(),tab()) + "\n");
    data.append("\npublic:\n");
    data.append(tab());
    if(isDll())
        data.append("explicit ");
    data.append(QString("%1(QObject *parent = nullptr);").arg(strFirstU()) + "\n");
    data.append(QString("%1~%2();").arg(tab(),strFirstU()) + "\n");
    foreach(QString par, pars()){
        data.append("\n");
        auto pardata = par.replace(";","");
        auto list = pardata.split(" ", Qt::SkipEmptyParts);
        auto ptype = list.first();
        auto pname = list.last();
        data.append(QString("%1%2 %3() const;").arg(tab(),ptype, pname) + "\n");
        data.append(QString("%1void set%2(%3 value);").arg(tab(),strFirstU(pname),ptype) + "\n");
    }
    data.append("\n");
    data.append("signals:\n");
    foreach(QString par, pars()) {
        auto pardata = par.replace(";","");
        auto list = pardata.split(" ", Qt::SkipEmptyParts);
        auto ptype = list.first();
        auto pname = list.last();
        data.append(QString("%1void %2Changed(").arg(tab(),pname));
        if(haveSignalValue())
            data.append(QString("%1 value").arg(ptype));
        data.append(");\n");
    }
    data.append("\n");
    if(havePrivate()) {
        data.append(QString("protected:\n"));
        data.append(QString("%1QScopedPointer<%2Private> d_ptr;").arg(tab(),strFirstU()) + "\n");
    } else {
        data.append(QString("private:\n"));
        foreach(QString par, pars()) {
            auto pardata = par.replace(";","");
            auto list = pardata.split(" ", Qt::SkipEmptyParts);
            auto ptype = list.first();
            auto pname = list.last();
            data.append(QString("%1%2 _%3;\n").arg(tab(),ptype,pname));
        }
    }
    data.append("\n");
    data.append("};\n");
    data.append("\n");
    data.append( QString("#endif  // %1_H").arg(className().toUpper()));
    data.append("\n");
    return data;
}

QString AutoCreate::createCpp()
{
    QString data;
    data.append("\n");
    data.append(QString("#include \"%1_p.h\"").arg(className().toLower()) + "\n");
    data.append(QString("#include \"%1.h\"").arg(className().toLower()) + "\n");
    data.append("\n");
    data.append(QString("%1::%1(QObject *parent)\n%2 : QObject(parent)").arg(strFirstU(),tab()));
    if(havePrivate())
        data.append(QString(", d_ptr(new %1Private)").arg(strFirstU()));
    data.append("\n");
    data.append("{\n}\n");
    data.append("\n");
    data.append(QString("%1::~%1()\n{\n}\n").arg(strFirstU()));
    QString prefix = havePrivate() ? "d_ptr->" : "_";
    foreach(QString par, pars()) {
        auto pardata = par.replace(";","");
        auto list = pardata.split(" ", Qt::SkipEmptyParts);
        auto ptype = list.first();
        auto pname = list.last();
        data.append("\n");
        data.append(QString("%1 %2::%3() const\n").arg(ptype,strFirstU(),pname));
        data.append(QString("{\n%1return %2%3;\n}\n").arg(tab(),prefix,pname));
        data.append("\n");
        data.append(QString("void %1::set%2(%3 value)\n").arg(strFirstU(),strFirstU(pname),ptype));
        data.append("{\n");
        data.append(QString("%1if(%2%3 == value)\n").arg(tab(),prefix,pname));
        data.append(QString("%1%1return;\n").arg(tab()));
        data.append(QString("%1%2%3 = value;\n").arg(tab(),prefix,pname));
        data.append(QString("%1emit %2Changed(%3);\n").arg(tab(),pname,haveSignalValue() ? "value" : ""));
        data.append("}\n");
    }
    data.append("\n");
    return data;
}

QString AutoCreate::createPrivate()
{
    QString data;
    data.append(QString("#ifndef %1_P_H").arg(className().toUpper()) + "\n");
    data.append(QString("#define %1_P_H").arg(className().toUpper()) + "\n");
    data.append("\n");
    if(isDll())
        data.append(QString("#include \"%1_global.h\"").arg(className().toLower()) + "\n");
    data.append("#include <QObject>\n");
    data.append("\n");
    data.append(QString("class %1%2Private : public QObject").arg(isDll()? (QString("%1_EXPORT ").arg(className().toUpper())) : "" , strFirstU()) + "\n");
    data.append("{\n");
    data.append("\n");
    data.append("public:\n");
    data.append(QString("%1%2Private()\n%3\n%1{}\n").arg(tab(),strFirstU(),""));
    data.append("\n");
    foreach(QString par, pars()) {
        auto pardata = par.replace(";","");
        auto list = pardata.split(" ", Qt::SkipEmptyParts);
        auto ptype = list.first();
        auto pname = list.last();
        data.append(QString("%1%2 %3;\n").arg(tab(),ptype,pname));

    }
    data.append("};\n");
    data.append("\n");
    data.append( QString("#endif  // %1_P_H").arg(className().toUpper()));
    data.append("\n");
    return data;
}

