#include "recordtreeitem.h"

RecordTreeItem::RecordTreeItem(const QString &text, const QString& type)
    : QStandardItem(text), m_type(type)
{
    updateIcon();
    setSizeHint(QSize(0, 28));
    setFlags(flags() | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
}

bool RecordTreeItem::isDeleted()
{
    return m_deleted;
}

void RecordTreeItem::setDeleted(bool isDeleted)
{
    m_deleted = isDeleted;
}

void RecordTreeItem::updateIcon()
{
    // if (m_type == "Directory") {
    //     setIcon(QIcon(":/images/Project/folder-icon.svg"));
    // } else if (m_type == "System Requirement")
    //     setIcon(QIcon(":/images/Project/record-sys-req-icon.svg"));
    // else if (m_type == "System Test Case")
    //     setIcon(QIcon(":/images/Project/record-sys-test-cases-icon.svg"));
    // else if (m_type == "System Test")
    //     setIcon(QIcon(":/images/Project/record-sys-tests-icon.svg"));
    // else if (m_type == "Software Requirement")
    //     setIcon(QIcon(":/images/Project/record-software-req-icon.svg"));
    // else if (m_type == "Software Test Case")
    //     setIcon(QIcon(":/images/Project/record-software-test-case-icon.svg"));
    // else
    //     setIcon(QIcon(":/images/Project/record-code-icon.svg"));
}
