#pragma once
#include <QWidget>
#include <QResizeEvent>
#include <QPlainTextEdit>
#include "highlighter.hpp"
#include <tree_sitter/api.h>

// stolen: https://felgo.com/doc/qt5/qtwidgets-widgets-codeeditor-example/
extern "C" {
const TSLanguage *tree_sitter_cpp();
}

class CodeEditor : public QPlainTextEdit {
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &, int);
    void onTextChange(int from, int charsRemoved, int charsAdded);

private:
    QWidget *lineNumberArea;
    Highlighter *highlighter;
    TSParser *m_parser;
    TSTree *m_tree;
};
