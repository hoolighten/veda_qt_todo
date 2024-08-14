#ifndef HEAD_H
#define HEAD_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class Head;
}

class Head : public QWidget
{
    Q_OBJECT

public:
    explicit Head(QWidget *parent = nullptr);
    void setUserLabel(QString name);
    ~Head();

private:

    QString getCurrentDateTime();
    Ui::Head *ui;
    QTimer* timer;
    bool isLogin;

signals:
    void save();
    void load();
    void logout();
    void login(QString userName);
};

#endif // HEAD_H
