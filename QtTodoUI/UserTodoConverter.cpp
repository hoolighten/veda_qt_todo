#define _CRT_SECURE_NO_WARNINGS

#include "UserTodoConverter.h"
#include <ctime>
#include <QWidget>
#include <map>
using namespace std;

UserTodoConverterImpl::UserTodoConverterImpl(TodoRepo* repo)
{
    this->repo = repo;
}

unsigned int UserTodoConverterImpl::save(map<string, UserTodo> data)
{
    unsigned int i = 0;
    qDebug()<<"convert save";
    for (auto iter = data.begin(); iter != data.end(); iter++) {

        vector<Todo> currentTodo;
        iter->second.getCurrentData(currentTodo);
        repo->saveArray(currentTodo);

        vector<Todo> finishTodo;
        iter->second.getFinishedData(finishTodo);
        repo->saveArray(finishTodo);

        i++;
    }

    return i;
}

bool UserTodoConverterImpl::load(map<std::string, UserTodo>& res)
{
    vector<Todo> data;

    this->repo->load(data);

    if (data.empty()) {
        return false;
    }


    for (auto todo : data) {

        //	map�� ������ ������ �߰�
        if (res.find(todo.getUserName()) == res.end()) {

            vector<Todo> data;
            vector<Todo> fin;

            UserTodo user = UserTodo(0, todo.getUserName(), data, fin);

            res.insert({ todo.getUserName(), user});
        }

        //	todo�� �ش� UserTodo�� �߰�
        res[todo.getUserName()].insertData(todo);

    }
    //	�̹� ������ �з�
    for (auto& userTodo : res) {
        userTodo.second.convertData();
    }
    return true;
}
