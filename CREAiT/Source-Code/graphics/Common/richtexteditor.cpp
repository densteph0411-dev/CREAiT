#include "richtexteditor.h"

#include <QtWidgets>
#include <QtGui>
#include <QtCore>

static QSize imageSizeFromDocumentResource(QTextDocument *doc, const QString &name)
{
    if (!doc || name.isEmpty()) {
        return {};
    }

    const QVariant res = doc->resource(QTextDocument::ImageResource, QUrl(name));
    if (res.canConvert<QImage>()) {
        return qvariant_cast<QImage>(res).size();
    }
    if (res.canConvert<QPixmap>()) {
        return qvariant_cast<QPixmap>(res).size();
    }

    // If the HTML contains a data URL, QTextDocument may not expose it via resource().
    if (name.startsWith(QStringLiteral("data:image/"))) {
        int start = name.indexOf(QStringLiteral("base64,"));
        if (start >= 0) {
            start += 7;
        } else {
            start = name.indexOf(',');
            start = (start >= 0) ? (start + 1) : -1;
        }
        if (start > 0 && start < name.size()) {
            const QByteArray payload = name.mid(start).toLatin1();
            const QByteArray bytes = QByteArray::fromBase64(payload);
            QImage img;
            img.loadFromData(bytes);
            return img.size();
        }
    }

    return {};
}

static bool ensureCursorOnImageChar(QTextCursor &cursor)
{
    if (cursor.charFormat().isImageFormat()) {
        return true;
    }

    // QTextCursor can often end up positioned *after* the image replacement character.
    // Try one character back.
    QTextCursor prev = cursor;
    if (prev.movePosition(QTextCursor::PreviousCharacter) && prev.charFormat().isImageFormat()) {
        cursor = prev;
        return true;
    }

    QTextCursor next = cursor;
    if (next.movePosition(QTextCursor::NextCharacter) && next.charFormat().isImageFormat()) {
        cursor = next;
        return true;
    }
    return false;
}

static bool selectImageCharAtOrNear(QTextEdit *edit, QTextCursor baseCursor, QTextCursor &outSelection)
{
    Q_UNUSED(edit);
    QTextCursor c = baseCursor;
    c.clearSelection();

    // Try selecting the character to the right (cursor is before the image char).
    QTextCursor right = c;
    if (right.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor)) {
        if (right.charFormat().isImageFormat()) {
            outSelection = right;
            return true;
        }
    }

    // Try selecting the character to the left (cursor is after the image char).
    QTextCursor left = c;
    if (left.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor)) {
        if (left.charFormat().isImageFormat()) {
            outSelection = left;
            return true;
        }
    }

    // As a fallback, normalize and try again.
    if (ensureCursorOnImageChar(c)) {
        QTextCursor t = c;
        t.clearSelection();
        QTextCursor r2 = t;
        if (r2.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor) && r2.charFormat().isImageFormat()) {
            outSelection = r2;
            return true;
        }
        QTextCursor l2 = t;
        if (l2.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor) && l2.charFormat().isImageFormat()) {
            outSelection = l2;
            return true;
        }
    }

    return false;
}

static QRect imageRectInViewport(QTextEdit *edit, QTextCursor imageCursor)
{
    if (!edit) {
        return {};
    }
    if (!ensureCursorOnImageChar(imageCursor)) {
        return {};
    }

    // Make sure we compute from the actual image character selection.
    QTextCursor sel;
    if (!selectImageCharAtOrNear(edit, imageCursor, sel)) {
        return {};
    }
    QTextCursor start = sel;
    start.clearSelection();

    // Use cursor rectangles around the image replacement character to get the actual
    // laid-out width (works reliably after reload / different HTML backends).
    QTextCursor end = start;
    end.movePosition(QTextCursor::NextCharacter);

    const QRect r1 = edit->cursorRect(start);
    const QRect r2 = edit->cursorRect(end);

    const int left = qMin(r1.left(), r2.left());
    const int top = qMin(r1.top(), r2.top());
    int w = qAbs(r2.left() - r1.left());

    const QTextImageFormat imf = sel.charFormat().toImageFormat();
    const QSize originalPx = imageSizeFromDocumentResource(edit->document(), imf.name());

    if (w <= 0) {
        if (imf.width() > 0) {
            w = qRound(imf.width());
        } else if (originalPx.isValid()) {
            w = originalPx.width();
        }
    }

    int h = 0;
    if (imf.height() > 0) {
        h = qRound(imf.height());
    } else if (originalPx.isValid() && originalPx.width() > 0) {
        const qreal aspect = qreal(originalPx.height()) / qreal(originalPx.width());
        h = qMax(1, qRound(w * aspect));
    } else {
        h = qMax(r1.height(), r2.height());
    }

    return QRect(left, top, w, h);
}

static QString embedDocumentImagesAsDataUrls(QTextDocument *doc)
{
    if (!doc) {
        return {};
    }

    QString html = doc->toHtml();

    QSet<QString> imageNames;
    for (QTextBlock block = doc->begin(); block.isValid(); block = block.next()) {
        for (QTextBlock::Iterator it = block.begin(); !it.atEnd(); ++it) {
            const QTextFragment frag = it.fragment();
            if (!frag.isValid()) {
                continue;
            }

            const QTextCharFormat fmt = frag.charFormat();
            if (!fmt.isImageFormat()) {
                continue;
            }

            const QTextImageFormat imf = fmt.toImageFormat();
            const QString name = imf.name();
            if (!name.isEmpty()) {
                imageNames.insert(name);
            }
        }
    }

    for (const QString &name : imageNames) {
        const QUrl url(name);
        const QVariant res = doc->resource(QTextDocument::ImageResource, url);
        if (!res.isValid()) {
            continue;
        }

        QImage img;
        if (res.canConvert<QImage>()) {
            img = qvariant_cast<QImage>(res);
        } else if (res.canConvert<QPixmap>()) {
            img = qvariant_cast<QPixmap>(res).toImage();
        }

        if (img.isNull()) {
            continue;
        }

        QByteArray bytes;
        QBuffer buffer(&bytes);
        buffer.open(QIODevice::WriteOnly);
        img.save(&buffer, "PNG");

        const QString dataUrl = QStringLiteral("data:image/png;base64,%1")
                                    .arg(QString::fromLatin1(bytes.toBase64()));

        // Replace common quoting styles.
        html.replace(QStringLiteral("src=\"%1\"").arg(name),
                     QStringLiteral("src=\"%1\"").arg(dataUrl));
        html.replace(QStringLiteral("src='%1'").arg(name),
                     QStringLiteral("src='%1'").arg(dataUrl));
    }

    return html;
}

RichTextEditor::RichTextEditor(QWidget *parent)
    : QWidget(parent),
    m_edit(new QTextEdit(this)),
    m_toolbar(new QToolBar(this)),
    m_fontBox(new QFontComboBox(m_toolbar)),
    m_sizeBox(new QSpinBox(m_toolbar))
{
    // --- QTextEdit setup
    m_edit->setAcceptRichText(true);
    m_edit->setPlaceholderText(QStringLiteral("Type here…"));

    // Enable double-click image resize + drag handle image resize.
    m_edit->viewport()->installEventFilter(this);

    // Bottom-right drag handle (child of the viewport, positioned over the image).
    m_imageResizeHandle = new QFrame(m_edit->viewport());
    m_imageResizeHandle->setFrameShape(QFrame::Box);
    m_imageResizeHandle->setLineWidth(1);
    m_imageResizeHandle->setFixedSize(10, 10);
    m_imageResizeHandle->setCursor(Qt::SizeFDiagCursor);
    m_imageResizeHandle->hide();
    m_imageResizeHandle->installEventFilter(this);

    // Keep handle positioned as the cursor/viewport changes.
    connect(m_edit, &QTextEdit::cursorPositionChanged, this, &RichTextEditor::repositionResizeHandle);
    if (m_edit->verticalScrollBar()) {
        connect(m_edit->verticalScrollBar(), &QScrollBar::valueChanged, this, &RichTextEditor::repositionResizeHandle);
    }
    if (m_edit->horizontalScrollBar()) {
        connect(m_edit->horizontalScrollBar(), &QScrollBar::valueChanged, this, &RichTextEditor::repositionResizeHandle);
    }

    // --- Toolbar: Font Family & Size (optional)
    m_toolbar->addWidget(m_fontBox);
    connect(m_fontBox, &QFontComboBox::currentFontChanged,
            this, &RichTextEditor::onFontFamilyChanged);

    m_sizeBox->setRange(6, 96);
    m_sizeBox->setValue(12);
    m_toolbar->addWidget(m_sizeBox);
    connect(m_sizeBox, &QSpinBox::valueChanged,
            this, &RichTextEditor::onFontSizeChanged);

    m_toolbar->addSeparator();

        // Prefer icon-only tool buttons for the formatting toolbar
        m_toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);

    // --- Bold / Italic
    m_actBold = m_toolbar->addAction(QIcon(":/images/Common/Format/format_bold.png"), "");
    m_actBold->setCheckable(true);
    m_actBold->setShortcut(QKeySequence::Bold);
    connect(m_actBold, &QAction::toggled, this, &RichTextEditor::onBoldToggled);

    m_actItalic = m_toolbar->addAction(QIcon(":/images/Common/Format/format_italic.png"), "");
    m_actItalic->setCheckable(true);
    m_actItalic->setShortcut(QKeySequence::Italic);
    connect(m_actItalic, &QAction::toggled, this, &RichTextEditor::onItalicToggled);

    m_toolbar->addSeparator();

    // --- Alignment group
    auto *alignGroup = new QActionGroup(this);
    m_actAlignLeft    = m_toolbar->addAction(QIcon(":/images/Common/Format/format_align_left.png"), "");
    m_actAlignCenter  = m_toolbar->addAction(QIcon(":/images/Common/Format/format_align_center.png"), "");
    m_actAlignRight   = m_toolbar->addAction(QIcon(":/images/Common/Format/format_align_right.png"), "");
    m_actAlignJustify = m_toolbar->addAction(QIcon(":/images/Common/Format/format_align_justify.png"), "");
    for (auto a : {m_actAlignLeft, m_actAlignCenter, m_actAlignRight, m_actAlignJustify}) {
        a->setCheckable(true);
        alignGroup->addAction(a);
    }
    m_actAlignLeft->setChecked(true);

    connect(m_actAlignLeft,    &QAction::triggered, this, &RichTextEditor::alignLeft);
    connect(m_actAlignCenter,  &QAction::triggered, this, &RichTextEditor::alignCenter);
    connect(m_actAlignRight,   &QAction::triggered, this, &RichTextEditor::alignRight);
    connect(m_actAlignJustify, &QAction::triggered, this, &RichTextEditor::alignJustify);

    m_toolbar->addSeparator();

    // --- Lists
    m_actBulleted = m_toolbar->addAction(QIcon(":/images/Common/Format/format_list_bulleted.png"), "");
    connect(m_actBulleted, &QAction::triggered, this, &RichTextEditor::makeBulleted);

    m_actNumbered = m_toolbar->addAction(QIcon(":/images/Common/Format/format_list_numbered.png"), "");
    connect(m_actNumbered, &QAction::triggered, this, &RichTextEditor::makeNumbered);

    m_toolbar->addSeparator();

    // --- Super/Sub
    m_actSuper = m_toolbar->addAction(QIcon(":/images/Common/Format/superscript.png"), "");
    m_actSuper->setCheckable(true);
    connect(m_actSuper, &QAction::toggled, this, &RichTextEditor::setSuperToggled);

    m_actSub = m_toolbar->addAction(QIcon(":/images/Common/Format/subscript.png"), "");
    m_actSub->setCheckable(true);
    connect(m_actSub, &QAction::toggled, this, &RichTextEditor::setSubToggled);

    m_toolbar->addSeparator();

    // --- Insert Table / Image / Link
    m_actTable = m_toolbar->addAction(QIcon(":/images/Common/Format/table_insert.svg"), "");
    connect(m_actTable, &QAction::triggered, this, &RichTextEditor::insertTable);

    // --- Table edit actions: add/remove rows/cols and border
    m_actTableAddRow = m_toolbar->addAction(QIcon(":/images/Common/Format/table_row_add.svg"), "");
    m_actTableAddRow->setToolTip(tr("Add table row"));
    connect(m_actTableAddRow, &QAction::triggered, this, &RichTextEditor::tableAddRow);

    m_actTableRemoveRow = m_toolbar->addAction(QIcon(":/images/Common/Format/table_row_remove.svg"), "");
    m_actTableRemoveRow->setToolTip(tr("Remove table row"));
    connect(m_actTableRemoveRow, &QAction::triggered, this, &RichTextEditor::tableRemoveRow);

    m_actTableAddCol = m_toolbar->addAction(QIcon(":/images/Common/Format/table_column_add.svg"), "");
    m_actTableAddCol->setToolTip(tr("Add table column"));
    connect(m_actTableAddCol, &QAction::triggered, this, &RichTextEditor::tableAddColumn);

    m_actTableRemoveCol = m_toolbar->addAction(QIcon(":/images/Common/Format/table_column_remove.svg"), "");
    m_actTableRemoveCol->setToolTip(tr("Remove table column"));
    connect(m_actTableRemoveCol, &QAction::triggered, this, &RichTextEditor::tableRemoveColumn);

    m_actTableBorder = m_toolbar->addAction(QIcon(":/images/Common/Format/table_border_formatting.svg"), "");
    m_actTableBorder->setToolTip(tr("Set table border"));
    connect(m_actTableBorder, &QAction::triggered, this, &RichTextEditor::tableSetBorder);

    m_actImage = m_toolbar->addAction(QIcon(":/images/Common/Format/insert_photo.png"), "");
    connect(m_actImage, &QAction::triggered, this, &RichTextEditor::insertImage);

    m_actLink = m_toolbar->addAction(QIcon(":/images/Common/Format/insert_link.png"), "");
    connect(m_actLink, &QAction::triggered, this, &RichTextEditor::insertLink);

    // --- Layout
    auto *lay = new QVBoxLayout(this);
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);
    lay->addWidget(m_toolbar);
    lay->addWidget(m_edit);
    setLayout(lay);

    // Keep toolbar state in sync with caret changes
    connect(m_edit, &QTextEdit::cursorPositionChanged,
            this, &RichTextEditor::syncUiWithCursor);
}

bool RichTextEditor::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_imageResizeHandle) {
        if (event->type() == QEvent::MouseButtonPress) {
            auto *me = static_cast<QMouseEvent*>(event);
            if (me->button() == Qt::LeftButton) {
                // Start resizing the currently selected image.
                if (!m_resizeCursor.isNull()) {
                    QTextCursor imageCursor = m_resizeCursor;
                    if (!ensureCursorOnImageChar(imageCursor)) {
                        return true;
                    }

                    QTextCursor selection;
                    if (!selectImageCharAtOrNear(m_edit, imageCursor, selection)) {
                        return true;
                    }

                    QTextImageFormat imf = selection.charFormat().toImageFormat();
                    const QString name = imf.name();
                    m_resizeOriginalPxSize = imageSizeFromDocumentResource(m_edit->document(), name);

                    const int currentW = (imf.width() > 0)
                        ? qRound(imf.width())
                        : (m_resizeOriginalPxSize.isValid() ? m_resizeOriginalPxSize.width() : 1);

                    if (currentW > 0) {
                        m_isResizingImage = true;
                        m_resizeStartGlobalPos = me->globalPosition().toPoint();
                        m_resizeStartWidthPx = currentW;
                        m_resizeCursor = selection;
                        // Since we consume the press event in the filter, explicitly grab the mouse
                        // so we still receive move/release while dragging.
                        m_imageResizeHandle->grabMouse();
                        return true;
                    }
                }
            }
        } else if (event->type() == QEvent::MouseMove) {
            if (m_isResizingImage) {
                auto *me = static_cast<QMouseEvent*>(event);
                const QPoint globalPos = me->globalPosition().toPoint();
                const int dx = globalPos.x() - m_resizeStartGlobalPos.x();
                const int newW = qMax(1, m_resizeStartWidthPx + dx);
                applyImageSizeAtCursor(m_resizeCursor, newW);
                repositionResizeHandle();
                return true;
            }
        } else if (event->type() == QEvent::MouseButtonRelease) {
            auto *me = static_cast<QMouseEvent*>(event);
            if (me->button() == Qt::LeftButton && m_isResizingImage) {
                m_isResizingImage = false;
                m_imageResizeHandle->releaseMouse();
                return true;
            }
        }
        return QWidget::eventFilter(obj, event);
    }

    if (obj == m_edit->viewport() && event->type() == QEvent::MouseButtonDblClick) {
        auto *me = static_cast<QMouseEvent*>(event);
        const QPoint pos = me->position().toPoint();
        QTextCursor c = m_edit->cursorForPosition(pos);
        ensureCursorOnImageChar(c);
        if (resizeImageAtCursor(c)) {
            return true; // handled
        }
    }

    if (obj == m_edit->viewport() && event->type() == QEvent::MouseButtonPress) {
        auto *me = static_cast<QMouseEvent*>(event);
        if (me->button() == Qt::LeftButton) {
            const QPoint pos = me->position().toPoint();
            QTextCursor c = m_edit->cursorForPosition(pos);
            if (ensureCursorOnImageChar(c)) {
                showResizeHandleForCursor(c);
            } else if (!m_isResizingImage) {
                hideResizeHandle();
            }
        }
    }

    // Open links on click (keeps the editor editable; clicking an anchor will launch the URL).
    if (obj == m_edit->viewport() && event->type() == QEvent::MouseButtonRelease) {
        auto *me = static_cast<QMouseEvent*>(event);
        if (me->button() == Qt::LeftButton && !m_isResizingImage) {
            const QPoint pos = me->position().toPoint();
            const QString href = m_edit->anchorAt(pos);
            if (!href.isEmpty()) {
                QDesktopServices::openUrl(QUrl(href));
                return true;
            }
        }
    }

    if (obj == m_edit->viewport() && (event->type() == QEvent::Resize || event->type() == QEvent::Wheel)) {
        repositionResizeHandle();
    }
    return QWidget::eventFilter(obj, event);
}

void RichTextEditor::showResizeHandleForCursor(const QTextCursor &cursor)
{
    if (!m_edit || !m_imageResizeHandle) {
        return;
    }

    QTextCursor c = cursor;
    if (!ensureCursorOnImageChar(c)) {
        hideResizeHandle();
        return;
    }

    m_resizeCursor = c;
    m_imageResizeHandle->show();
    repositionResizeHandle();
}

void RichTextEditor::hideResizeHandle()
{
    m_resizeCursor = QTextCursor();
    if (m_imageResizeHandle) {
        m_imageResizeHandle->hide();
    }
}

void RichTextEditor::repositionResizeHandle()
{
    if (!m_edit || !m_imageResizeHandle || m_resizeCursor.isNull() || !m_imageResizeHandle->isVisible()) {
        return;
    }

    QTextCursor c = m_resizeCursor;
    if (!ensureCursorOnImageChar(c)) {
        hideResizeHandle();
        return;
    }

    const QRect imgRect = imageRectInViewport(m_edit, c);
    if (!imgRect.isValid() || imgRect.isNull()) {
        return;
    }

    const int hw = m_imageResizeHandle->width();
    const int hh = m_imageResizeHandle->height();

    QPoint p = imgRect.bottomRight() - QPoint(hw, hh);

    // Keep it inside viewport bounds.
    p.setX(qBound(0, p.x(), m_edit->viewport()->width() - hw));
    p.setY(qBound(0, p.y(), m_edit->viewport()->height() - hh));
    m_imageResizeHandle->move(p);
}

bool RichTextEditor::applyImageSizeAtCursor(QTextCursor cursor, int newWidthPx)
{
    if (!m_edit) {
        return false;
    }

    QTextCursor selection;
    if (!selectImageCharAtOrNear(m_edit, cursor, selection)) {
        return false;
    }

    QTextImageFormat imf = selection.charFormat().toImageFormat();
    const QString name = imf.name();

    QSize originalSize = m_resizeOriginalPxSize;
    if (!originalSize.isValid()) {
        originalSize = imageSizeFromDocumentResource(m_edit->document(), name);
    }
    const int w = qMax(1, newWidthPx);

    int h = 0;
    if (originalSize.isValid() && originalSize.width() > 0 && originalSize.height() > 0) {
        const qreal aspect = qreal(originalSize.height()) / qreal(originalSize.width());
        h = qMax(1, qRound(w * aspect));
    } else if (imf.width() > 0 && imf.height() > 0) {
        const qreal aspect = imf.height() / imf.width();
        h = qMax(1, qRound(w * aspect));
    } else {
        // Last resort: keep existing height (or set square).
        h = (imf.height() > 0) ? qRound(imf.height()) : w;
    }

    imf.setWidth(w);
    imf.setHeight(h);
    selection.setCharFormat(imf);
    m_edit->viewport()->update();
    return true;
}

bool RichTextEditor::resizeImageAtCursor(QTextCursor cursor)
{
    if (!m_edit) {
        return false;
    }

    QTextCursor selection;
    if (!selectImageCharAtOrNear(m_edit, cursor, selection)) {
        return false;
    }

    QTextCharFormat cf = selection.charFormat();
    if (!cf.isImageFormat()) {
        return false;
    }

    QTextImageFormat imf = cf.toImageFormat();
    const QString name = imf.name();
    if (name.isEmpty()) {
        return false;
    }

    // Determine the original pixel size from the stored resource.
    QSize originalSize = imageSizeFromDocumentResource(m_edit->document(), name);
    if (!originalSize.isValid()) {
        // Fallback 1: try current format's size, if any.
        originalSize = QSize(qRound(imf.width()), qRound(imf.height()));
    }

    int currentW = (imf.width() > 0) ? qRound(imf.width()) : 0;
    if (currentW <= 0) {
        const QRect r = imageRectInViewport(m_edit, cursor);
        currentW = r.isValid() ? r.width() : 0;
    }
    if (currentW <= 0 && originalSize.isValid()) {
        currentW = originalSize.width();
    }
    if (currentW <= 0) {
        currentW = 200; // last-resort default
    }
    bool ok = false;
    const int newW = QInputDialog::getInt(
        this,
        tr("Resize Image"),
        tr("Width (px):"),
        currentW,
        1,
        4096,
        1,
        &ok);
    if (!ok) {
        return true; // user canceled, but we did handle the image double-click
    }

    int newH = 0;
    if (originalSize.isValid() && originalSize.width() > 0 && originalSize.height() > 0) {
        const qreal aspect = qreal(originalSize.height()) / qreal(originalSize.width());
        newH = qMax(1, qRound(newW * aspect));
    } else if (imf.width() > 0 && imf.height() > 0) {
        const qreal aspect = imf.height() / imf.width();
        newH = qMax(1, qRound(newW * aspect));
    } else {
        newH = qMax(1, qRound(newW * 0.75));
    }

    imf.setWidth(newW);
    imf.setHeight(newH);
    selection.setCharFormat(imf);

    // Keep the handle usable and correctly positioned after the dialog closes.
    showResizeHandleForCursor(selection);

    return true;
}

QString RichTextEditor::html() const {
    return embedDocumentImagesAsDataUrls(m_edit ? m_edit->document() : nullptr);
}

void RichTextEditor::setHtml(const QString &h) {
    m_edit->setHtml(h);
    // Clear any stale cursor/handle state from previous document contents.
    hideResizeHandle();
    m_resizeOriginalPxSize = QSize();
}

// ----------------- Slots & helpers -----------------

void RichTextEditor::onFontFamilyChanged(const QFont &f) {
    QTextCharFormat fmt;
    QStringList families;
    families << f.family();
    fmt.setFontFamilies(families);
    mergeFormat(fmt);
}

void RichTextEditor::onFontSizeChanged(int pts) {
    QTextCharFormat fmt;
    fmt.setFontPointSize(pts);
    mergeFormat(fmt);
}

void RichTextEditor::onBoldToggled(bool on) {
    QTextCharFormat fmt;
    fmt.setFontWeight(on ? QFont::Bold : QFont::Normal);
    mergeFormat(fmt);
}

void RichTextEditor::onItalicToggled(bool on) {
    QTextCharFormat fmt;
    fmt.setFontItalic(on);
    mergeFormat(fmt);
}

void RichTextEditor::alignLeft()    { m_edit->setAlignment(Qt::AlignLeft); }
void RichTextEditor::alignCenter()  { m_edit->setAlignment(Qt::AlignHCenter); }
void RichTextEditor::alignRight()   { m_edit->setAlignment(Qt::AlignRight); }
void RichTextEditor::alignJustify() { m_edit->setAlignment(Qt::AlignJustify); }

void RichTextEditor::makeBulleted() {
    QTextListFormat lf;
    lf.setStyle(QTextListFormat::ListDisc);
    m_edit->textCursor().createList(lf);
}

void RichTextEditor::makeNumbered() {
    QTextListFormat lf;
    lf.setStyle(QTextListFormat::ListDecimal);
    m_edit->textCursor().createList(lf);
}

void RichTextEditor::setSuperToggled(bool on) {
    if (on) m_actSub->setChecked(false);
    QTextCharFormat f;
    f.setVerticalAlignment(on ? QTextCharFormat::AlignSuperScript : QTextCharFormat::AlignNormal);
    mergeFormat(f);
}

void RichTextEditor::setSubToggled(bool on) {
    if (on) m_actSuper->setChecked(false);
    QTextCharFormat f;
    f.setVerticalAlignment(on ? QTextCharFormat::AlignSubScript : QTextCharFormat::AlignNormal);
    mergeFormat(f);
}

void RichTextEditor::insertTable() {
    bool ok1 = false, ok2 = false;
    const int rows = QInputDialog::getInt(this, tr("Insert Table"), tr("Rows:"), 2, 1, 100, 1, &ok1);
    if (!ok1) return;
    const int cols = QInputDialog::getInt(this, tr("Insert Table"), tr("Columns:"), 2, 1, 50, 1, &ok2);
    if (!ok2) return;

    QTextTableFormat tf;
    tf.setCellPadding(4);
    tf.setCellSpacing(0);
    // set a visible border (use integer width and a brush)
    tf.setBorder(1);
    tf.setBorderBrush(QBrush(Qt::black));

    // Insert the table and ensure table format is applied
    QTextTable *table = m_edit->textCursor().insertTable(rows, cols, tf);
    if (table) {
        // ensure each cell has no extra spacing and is visible; apply a minimal cell format
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                QTextTableCell cell = table->cellAt(r, c);
                QTextCursor cellCursor = cell.firstCursorPosition();
                QTextBlockFormat bf = cellCursor.blockFormat();
                bf.setBottomMargin(0);
                bf.setTopMargin(0);
                cellCursor.setBlockFormat(bf);
            }
        }
    }
}

void RichTextEditor::insertImage() {
    const QString path = QFileDialog::getOpenFileName(
        this, tr("Insert Image"), QString(),
        tr("Images (*.png *.jpg *.jpeg *.bmp *.gif)")
        );
    if (path.isEmpty()) return;

    // Embed image bytes into the document so it’s portable
    QImage img(path);
    if (img.isNull()) return;

    const QUrl key(QStringLiteral("img-%1").arg(QUuid::createUuid().toString(QUuid::WithoutBraces)));
    m_edit->document()->addResource(QTextDocument::ImageResource, key, img);

    QTextImageFormat im;
    im.setName(key.toString());

    QTextCursor c = m_edit->textCursor();
    c.insertImage(im);
    c.insertBlock();
    m_edit->setTextCursor(c);
}

void RichTextEditor::insertLink() {
    bool ok = false;
    const QString url = QInputDialog::getText(this, tr("Insert Link"),
                                              tr("URL (https://…):"),
                                              QLineEdit::Normal,
                                              QStringLiteral("https://"),
                                              &ok);
    if (!ok || url.isEmpty()) return;

    QTextCursor c = m_edit->textCursor();
    const QTextCharFormat prevFormat = m_edit->currentCharFormat();
    QString label = c.selectedText().trimmed();
    if (label.isEmpty()) label = url;

    QTextCharFormat fmt;
    fmt.setForeground(m_edit->palette().brush(QPalette::Link));
    fmt.setFontUnderline(true);
    fmt.setAnchor(true);
    fmt.setAnchorHref(url);

    if (!c.hasSelection()) {
        // Insert the text *with* the link format.
        c.insertText(label, fmt);
    } else {
        // Apply to selected text.
        c.mergeCharFormat(fmt);
        // Put the caret at the end so further typing doesn't continue the link.
        const int end = c.selectionEnd();
        c.setPosition(end);
    }

    m_edit->setTextCursor(c);
    // Restore previous typing format so normal text doesn't inherit link styling.
    m_edit->setCurrentCharFormat(prevFormat);
}

// ----------------- Table editing helpers -----------------

static QTextTable* currentTableForCursor(QTextEdit *edit) {
    QTextCursor c = edit->textCursor();
    return c.currentTable();
}

void RichTextEditor::tableAddRow() {
    QTextTable *table = currentTableForCursor(m_edit);
    if (!table) return;
    QTextCursor c = m_edit->textCursor();
    QTextTableCell cell = table->cellAt(c);
    int row = cell.row();
    table->insertRows(row + 1, 1);
}

void RichTextEditor::tableRemoveRow() {
    QTextTable *table = currentTableForCursor(m_edit);
    if (!table) return;
    if (table->rows() <= 1) return; // keep at least one row
    QTextCursor c = m_edit->textCursor();
    QTextTableCell cell = table->cellAt(c);
    int row = cell.row();
    // clamp
    if (row < 0 || row >= table->rows()) row = table->rows() - 1;
    table->removeRows(row, 1);
}

void RichTextEditor::tableAddColumn() {
    QTextTable *table = currentTableForCursor(m_edit);
    if (!table) return;
    QTextCursor c = m_edit->textCursor();
    QTextTableCell cell = table->cellAt(c);
    int col = cell.column();
    table->insertColumns(col + 1, 1);
}

void RichTextEditor::tableRemoveColumn() {
    QTextTable *table = currentTableForCursor(m_edit);
    if (!table) return;
    if (table->columns() <= 1) return; // keep at least one column
    QTextCursor c = m_edit->textCursor();
    QTextTableCell cell = table->cellAt(c);
    int col = cell.column();
    if (col < 0 || col >= table->columns()) col = table->columns() - 1;
    table->removeColumns(col, 1);
}

void RichTextEditor::tableSetBorder() {
    QTextTable *table = currentTableForCursor(m_edit);
    if (!table) return;

    bool ok = false;
    int w = QInputDialog::getInt(this, tr("Table Border"), tr("Border width (px):"), 1, 0, 10, 1, &ok);
    if (!ok) return;

    QColor color = QColorDialog::getColor(Qt::black, this, tr("Border color"));
    if (!color.isValid()) return;

    QTextTableFormat tf = table->format();
    tf.setBorder(w);
    tf.setBorderBrush(QBrush(color));
    table->setFormat(tf);
}

void RichTextEditor::syncUiWithCursor() {
    // Bold/Italic state
    const QFont cf = m_edit->currentFont();
    m_actBold->blockSignals(true);
    m_actItalic->blockSignals(true);
    m_actBold->setChecked(cf.weight() >= QFont::Bold);
    m_actItalic->setChecked(cf.italic());
    m_actBold->blockSignals(false);
    m_actItalic->blockSignals(false);

    // Font boxes
    m_fontBox->blockSignals(true);
    m_fontBox->setCurrentFont(cf);
    m_fontBox->blockSignals(false);

    m_sizeBox->blockSignals(true);
    const qreal sz = m_edit->fontPointSize();
    m_sizeBox->setValue(sz > 0 ? qRound(sz) : (cf.pointSize() > 0 ? cf.pointSize() : 12));
    m_sizeBox->blockSignals(false);
}

void RichTextEditor::mergeFormat(const QTextCharFormat &fmt) {
    QTextCursor c = m_edit->textCursor();
    if (!c.hasSelection())
        c.select(QTextCursor::WordUnderCursor);
    c.mergeCharFormat(fmt);
    m_edit->mergeCurrentCharFormat(fmt);
}
