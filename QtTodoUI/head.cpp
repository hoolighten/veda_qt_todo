#include "head.h"
#include "ui_head.h"
#include <QTimer>
#include <QDateTime>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
Head::Head(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Head)
{
    isLogin=false;
    ui->setupUi(this);

    ui->save->setIcon(":/icons/save.png");
    ui->logout->setIcon(":/icons/logout.png");
    ui->login->setIcon(":/icons/login.png");
    timer = new QTimer();

    ui->label_2->setText(getCurrentDateTime());
    //  10sec

    timer->setInterval(1000);
    connect(timer, &QTimer::timeout,
            [&](){
                ui->label_2->setText(getCurrentDateTime());
    });
    ui->save->setClick([&](){ emit save(); });
    ui->logout->setClick(this, [&](){
        if(!isLogin){
            QMessageBox::warning(this, "로그인 오류", "로그인 먼저 하세요.", QMessageBox::Ok);
            return;
        }
        emit logout();
        isLogin=false;
    });
    ui->login->setClick(this, [&](){
        if(isLogin){
            QMessageBox::warning(this, "로그인 오류", "이미 로그인 중 입니다", QMessageBox::Ok);
            return;
        }
        QString userName;
        bool ok;
        userName = QInputDialog::getText(this, "로그인", "유저이름", QLineEdit::Normal, userName, &ok);
        if(ok && !userName.isEmpty()){
            emit login(userName);
            isLogin=true;
        }
    });
}

void Head::setUserLabel(QString name)
{
    ui->label->setText(name);
}
Head::~Head()
{
    delete ui;
    delete timer;
}

QString Head::getCurrentDateTime()
{
    QDateTime current = QDateTime::currentDateTime();
    QDate curDate = current.date();
    QTime curTime = current.time();
    QString timeStr;
    timeStr.append(QString::number(curDate.year())).append("-");
    //  month
    if(curDate.month()<10){
        timeStr.append("0").append(QString::number(curDate.month())).append("-");
    }
    else{
        timeStr.append(QString::number(curDate.month())).append("-");
    }
    //  day
    if(curDate.day()<10){
        timeStr.append("0").append(QString::number(curDate.day())).append(" ");
    }
    else{
        timeStr.append(QString::number(curDate.day())).append(" ");
    }
    // hour
    if(curTime.hour()<10){
        timeStr.append("0").append(QString::number(curTime.hour())).append(":");
    }
    else{
        timeStr.append(QString::number(curTime.hour())).append(":");
    }
    //  minute
    if(curTime.minute()<10){
        timeStr.append("0").append(QString::number(curTime.minute()));
    }
    else{
        timeStr.append(QString::number(curTime.minute()));
    }

    return timeStr;
}
