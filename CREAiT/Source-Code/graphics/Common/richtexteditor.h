#ifndef RICHTEXTEDITOR_H
#define RICHTEXTEDITOR_H

#include <QWidget>
#include <QTextEdit>
#include <QToolBar>
#include <QFontComboBox>
#include <QSpinBox>
#include <QAction>

class QFrame;

class RichTextEditor : public QWidget {
    Q_OBJECT
public:
    explicit RichTextEditor(QWidget *parent = nullptr);

    QString html() const;
    void setHtml(const QString &h);

signals:
    // Emit if you later want to handle link activations externally.
    void linkActivated(const QUrl &url);

private slots:
    // Font and basic inline formatting
    void onFontFamilyChanged(const QFont &f);
    void onFontSizeChanged(int pts);
    void onBoldToggled(bool on);
    void onItalicToggled(bool on);

    // Paragraph alignment
    void alignLeft();
    void alignCenter();
    void alignRight();
    void alignJustify();

    // Lists
    void makeBulleted();
    void makeNumbered();

    // Super/Sub script
    void setSuperToggled(bool on);
    void setSubToggled(bool on);

    // Insertions
    void insertTable();
    void insertImage();
    void insertLink();

    // Table editing helpers
    void tableAddRow();
    void tableRemoveRow();
    void tableAddColumn();
    void tableRemoveColumn();
    void tableSetBorder();

    // Keep toolbar state synced
    void syncUiWithCursor();

private:
    bool eventFilter(QObject *obj, QEvent *event) override;

    // Helpers
    void mergeFormat(const QTextCharFormat &fmt);
    bool resizeImageAtCursor(QTextCursor cursor);
    void showResizeHandleForCursor(const QTextCursor &cursor);
    void hideResizeHandle();
    void repositionResizeHandle();
    bool applyImageSizeAtCursor(QTextCursor cursor, int newWidthPx);

    // UI
    QTextEdit     *m_edit;
    QToolBar      *m_toolbar;
    QFontComboBox *m_fontBox;
    QSpinBox      *m_sizeBox;

    // Actions we need to reference
    QAction *m_actBold;
    QAction *m_actItalic;

    QAction *m_actAlignLeft;
    QAction *m_actAlignCenter;
    QAction *m_actAlignRight;
    QAction *m_actAlignJustify;

    QAction *m_actBulleted;
    QAction *m_actNumbered;

    QAction *m_actSuper;
    QAction *m_actSub;

    QAction *m_actTable;
    QAction *m_actImage;
    QAction *m_actLink;
    // Table editing actions
    QAction *m_actTableAddRow;
    QAction *m_actTableRemoveRow;
    QAction *m_actTableAddCol;
    QAction *m_actTableRemoveCol;
    QAction *m_actTableBorder;

    // Drag-to-resize support for images
    QFrame *m_imageResizeHandle = nullptr;
    bool m_isResizingImage = false;
    QPoint m_resizeStartGlobalPos;
    int m_resizeStartWidthPx = 0;
    QSize m_resizeOriginalPxSize;
    QTextCursor m_resizeCursor;
};

#endif // RICHTEXTEDITOR_H
