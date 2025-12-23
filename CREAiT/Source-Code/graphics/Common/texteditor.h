#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QWidget>

namespace Ui {
class TextEditor;
}

class TextEditor : public QWidget
{
    Q_OBJECT

public:
    explicit TextEditor(QWidget *parent = nullptr);
    ~TextEditor();

private:
    Ui::TextEditor *ui;
};

#endif // TEXTEDITOR_H
