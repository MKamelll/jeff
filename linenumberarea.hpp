#pragma once
#include <QWidget>
#include <QPaintEvent>
#include "codeeditor.hpp"

// stolen: https://felgo.com/doc/qt5/qtwidgets-widgets-codeeditor-example/

class LineNumberArea : public QWidget {
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const override {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;
};
