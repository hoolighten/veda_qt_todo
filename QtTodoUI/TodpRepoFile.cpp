#include"TodoRepo.h"
#include <QWidget>
using namespace std;

TodoRepoFile::TodoRepoFile(string dir)
{
    output = ofstream();
    input = ifstream();
    this->dir = dir;
}


unsigned int TodoRepoFile::saveArray(vector<Todo> data)
{

    int i = 0;
    lock.lock();

    qDebug()<<"save";
    output.open(dir, ios::binary | ios::app);
    if (!output.is_open()) {
        cerr << "output stream cannot open";
        lock.unlock();
        return 0;
    }
    output.seekp(ios::beg);
    for (i = 0; i < data.size(); i++) {
        qDebug()<<data[i].getContent();
        output.write((char*)(&data[i]), sizeof(Todo));
    }
    output.close();
    lock.unlock();

    return i;
}

void TodoRepoFile::load(vector<Todo>& res)
{
    lock.lock();
    input.open(dir, ios::in | ios::binary );

    if (!input.is_open()) {
        cerr << "input stream cannot open";
        lock.unlock();
        return;
    }
    if (!input.is_open()) {
        output.open(dir, ios::out | ios::binary);
        output.close();
        lock.unlock();

        return;
    }
    input.seekg(ios::beg);
    vector<Todo> tmp;

    while (!input.eof()) {

        if (input.tellg() != ios::beg) {

            input.seekg(-1, ios::cur);
        }
        Todo *buf = new Todo();
        input.read(reinterpret_cast<char*>(buf), sizeof(Todo));
        input.get();
        Todo save = Todo(buf->getId(),
                         buf->getUserName(), buf->getContent(),
                         buf->getInsertDate(), buf->getDueDate(),
                         buf->getIsDone(), buf->getIsFin());
        tmp.push_back(save);
    }
    res.swap(tmp);

    input.close();
    lock.unlock();

}
