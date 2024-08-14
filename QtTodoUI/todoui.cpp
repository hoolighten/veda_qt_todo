#include "todoui.h"
#include "./ui_todoui.h"
#include "finishenabletab.h"
#include <QLabel>
#include <vector>
#include "Todo.h"

TodoUI::TodoUI(DataManage* manager, QString userName, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TodoUI)
{
    enabled = false;
    enablingWidget = nullptr;
    this->manager = manager;
    this->userName = userName;
    ui->setupUi(this);
    auto item1 = std::make_shared<TodoItem>(this);
    item1->setDueDate("2022-04-05");
    item1->setContent("Sample Task 1");
    item1->setIsFinish(true);
    item1->setIsDone(false);

    showTodoList();
}

void TodoUI::loadTodo()
{
    std::vector<Todo> data;

    bool isDone = manager->getFinishedData(data, userName.toStdString());

    for(Todo todo : data){
        TodoItem* item = new TodoItem();
        item->setContent(QString::fromStdString(todo.getContent()));
        item->setDueDate(QString::fromStdString(todo.getDueDate()));
        item->setIsDone(todo.getIsDone());
        item->setIsFinish(todo.getIsFin());
        item->setId(todo.getId());


        connect(item, &TodoItem::edit, this,
               [&](unsigned int id, QString content, QString date){
                    emit edit(id, userName, content, date);
                });

        connect(item, &TodoItem::remove,
                [&](unsigned int id){
                    manager->removeTodoData(userName.toStdString(), id);
                    removeListItems();
                    showTodoList();
                });
        if(todo.getIsDone()){
            this->doneTodo.push_back(item);
        }
        else{
            this->dueTodo.push_back(item);
        }
    }
    bool isFin = manager->getCurrentData(data, userName.toStdString());

    for(Todo todo : data){
        TodoItem* item = new TodoItem();

        item->setContent(QString::fromStdString(todo.getContent()));
        item->setDueDate(QString::fromStdString(todo.getDueDate()));
        item->setIsDone(todo.getIsDone());
        item->setIsFinish(todo.getIsFin());
        item->setId(todo.getId());

        connect(item, &TodoItem::edit, this,
                [&](unsigned int id, QString content, QString date)
                {  emit edit(id, userName, content, date);   });

        connect(item, &TodoItem::remove, this,
                [&](unsigned int id){
                    manager->removeTodoData(userName.toStdString(), id);
                    removeListItems();
                    showTodoList();
                });
        connect(item, &TodoItem::checked, this,
                [&](unsigned int id, bool flag){
                    if(flag){
                        manager->checkTodo(id, this->userName.toStdString());
                    }
                    removeListItems();
                    showTodoList();
                });
        if(todo.getIsDone()){
            this->doneTodo.push_back(item);
        }
        else{
            this->dueTodo.push_back(item);
        }
    }
}

TodoUI::~TodoUI()
{
    removeListItems();
    delete ui;
}

void TodoUI::showTodoList()
{
    ui->contents->clear();
    loadTodo();

    if(enabled){
        for(auto todo: doneTodo){
            QListWidgetItem* item = new QListWidgetItem();
            finishLayout.push_back(item);
            item->setSizeHint(todo->sizeHint());
            ui->contents->addItem(item);
            ui->contents->setItemWidget(item, todo);
        }
    }

    enablingWidget = new FinishEnableTab();
    enableLayout = new QListWidgetItem();
    enablingWidget->setEnable(this->enabled);
    connect(enablingWidget, &FinishEnableTab::clickTab, this,
            [&](bool enable){
                this->enabled = !(this->enabled);

                removeListItems();
                showTodoList();
            });

    enableLayout->setSizeHint(enablingWidget->sizeHint());
    ui->contents->addItem(enableLayout);
    ui->contents->setItemWidget(enableLayout, enablingWidget);

    for(auto todo: dueTodo){

        QListWidgetItem* item = new QListWidgetItem();
        item->setSizeHint(todo->sizeHint());
        duringLayout.push_back(item);
        ui->contents->addItem(item);
        ui->contents->setItemWidget(item, todo);
    }
}

void TodoUI::removeListItems()
{
    for(auto item: finishLayout){

        ui->contents->removeItemWidget(item);
        delete item;
    }
    ui->contents->removeItemWidget(enableLayout);
    delete enableLayout;
    delete enablingWidget;

    for(auto item: duringLayout){
        ui->contents->removeItemWidget(item);
        delete item;
    }
    finishLayout.clear();
    duringLayout.clear();


    for(auto todo:doneTodo){
        delete todo;
    }
    for(auto todo:dueTodo){
        delete todo;
    }
    doneTodo.clear();
    dueTodo.clear();

    ui->contents->clear();
}

void TodoUI::setUser(QString name)
{
    this->userName = name;
    if(name == ""){
        removeListItems();
    }
}

QString TodoUI::getUserName() const
{
    return this->userName;
}
