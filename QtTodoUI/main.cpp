
#include <QVBoxLayout>
#include "mainwindow.h"
#include "DataManage.h"
#include "UserTodoConverter.h"
#include "TodoRepo.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TodoRepo* repo = new TodoRepoFile("data");
    UserTodoConverter* converter = new UserTodoConverterImpl(repo);
    DataManage* manager = new DataManageImpl(repo, converter);

    MainWindow main_window(manager, nullptr);

    main_window.show();

    return a.exec();
}
