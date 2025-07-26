#include "clipboardhelper.h"

Q_INVOKABLE void ClipboardHelper::copyText(const QString& text) {
    QClipboard *clipboard = QGuiApplication::clipboard();
    if (clipboard)
        clipboard->setText(text);
}
