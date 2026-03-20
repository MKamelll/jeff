#include <QApplication>
#include <QMenuBar>
#include <QVBoxLayout>

#include "mainwindow.hpp"
#include "codeeditor.hpp"

MainWindow::MainWindow() {

    setWindowTitle("jeff");
    resize(860, 640);

    createActions();
    createMenus();

    QWidget *central = new QWidget;
    setCentralWidget(central);

    CodeEditor *editor = new CodeEditor;

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(editor);
    central->setLayout(layout);
}

void MainWindow::createActions() {
    newAction = new QAction("New", this);
    saveAction = new QAction("Save", this);
    saveAsAction = new QAction("Save as", this);
    quitAction = new QAction("Quit", this);

    connect(quitAction, &QAction::triggered, this, &QApplication::exit);
}
void MainWindow::createMenus() {
    filemenu = menuBar()->addMenu("File");
    editmenu = menuBar()->addMenu("Edit");
    toolsmenu = menuBar()->addMenu("Tools");
    helpmenu = menuBar()->addMenu("Help");
    aboutmenu = menuBar()->addMenu("About");

    filemenu->addAction(newAction);
    filemenu->addAction(saveAction);
    filemenu->addAction(saveAsAction);
    filemenu->addAction(quitAction);
}

void MainWindow::newFile() {}
void MainWindow::save() {}
void MainWindow::saveAs() {}
