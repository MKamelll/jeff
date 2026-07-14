#include "linenumberarea.hpp"
#include "codeeditor.hpp"
#include <QPainter>
#include <QTextBlock>
#include <QFont>

constexpr auto FONT_NAME = "Dejavu Sans Mono";
constexpr auto FONT_SIZE = 16;

CodeEditor::CodeEditor(QWidget *parent)
    : QPlainTextEdit(parent), lineNumberArea(new LineNumberArea(this)),
      highlighter(new Highlighter(document())), m_parser(ts_parser_new()) {

    connect(this, SIGNAL(blockCountChanged(int)), this,
            SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect, int)), this,
            SLOT(updateLineNumberArea(QRect, int)));

    connect(this->document(), &QTextDocument::contentsChange, this,
            &CodeEditor::onTextChange);

    ts_parser_set_language(m_parser, tree_sitter_cpp());
    m_tree = ts_parser_parse_string(m_parser, nullptr,
                                    this->toPlainText().toStdString().c_str(),
                                    this->toPlainText().length());
    updateLineNumberAreaWidth(0);
    setFont(QFont(FONT_NAME, FONT_SIZE));
}

void CodeEditor::onTextChange(int from, int charsRemoved, int charsAdded) {
    TSPoint start_point;
    start_point.row = 0;
    start_point.column = 0;

    TSPoint old_end_point;
    old_end_point.row = 0;
    old_end_point.column = 0;

    TSPoint new_end_point;
    new_end_point.row = 0;
    new_end_point.column = 0;

    TSInputEdit edit;
    edit.start_byte = from;
    edit.old_end_byte = from + charsRemoved;
    edit.new_end_byte = from + charsAdded;

    edit.start_point = start_point;
    edit.old_end_point = old_end_point;
    edit.new_end_point = new_end_point;

    ts_tree_edit(m_tree, &edit);

    auto new_tree = ts_parser_parse_string(m_parser, m_tree,
                                           toPlainText().toStdString().c_str(),
                                           toPlainText().length());
    m_tree = new_tree;

    qDebug() << ts_node_string(ts_tree_root_node(m_tree)) << "\n";
}

int CodeEditor::lineNumberAreaWidth() {
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */) {
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy) {
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(),
                               rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent *e) {
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(
        QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event) {
    QPainter painter(lineNumberArea);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top =
        (int)blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int)blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::darkGray);
            painter.drawText(0, top, lineNumberArea->width(),
                             fontMetrics().height(), Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int)blockBoundingRect(block).height();
        ++blockNumber;
    }
}
