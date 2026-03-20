#pragma once
#include <QMainWindow>
#include <QMenu>
#include <QAction>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();

private slots:
    void newFile();
    void save();
    void saveAs();

private:
    void createActions();
    void createMenus();

    QMenu *filemenu;
    QMenu *editmenu;
    QMenu *toolsmenu;
    QMenu *helpmenu;
    QMenu *aboutmenu;

    QAction *newAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *quitAction;
};
