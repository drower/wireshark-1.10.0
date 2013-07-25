/* label_stack.cpp
 *
 * $Id: label_stack.cpp 46863 2012-12-30 19:33:05Z gerald $
 *
 * Wireshark - Network traffic analyzer
 * By Gerald Combs <gerald@wireshark.org>
 * Copyright 1998 Gerald Combs
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "label_stack.h"
#include <QMouseEvent>
#include <QContextMenuEvent>

#include "tango_colors.h"

/* Temporary message timeouts */
const int temporary_interval_ = 1000;
const int temporary_msg_timeout_ = temporary_interval_ * 9;
const int temporary_flash_timeout_ = temporary_interval_ / 5;
const int num_flashes_ = 3;

LabelStack::LabelStack(QWidget *parent) :
    QLabel(parent)
{
#ifdef Q_WS_MAC
    setAttribute(Qt::WA_MacSmallSize, true);
#endif
    temporary_ctx_ = -1;
    fillLabel();

    connect(&temporary_timer_, SIGNAL(timeout()), this, SLOT(updateTemporaryStatus()));
}

void LabelStack::setTemporaryContext(int ctx) {
    temporary_ctx_ = ctx;
}

void LabelStack::fillLabel() {
    StackItem *si;
    QString style_sheet;

    style_sheet =
            "QLabel {"
            "  margin-left: 0.5em;";

    if (labels_.isEmpty()) {
        clear();
        return;
    }

    si = labels_.first();

    if (si->ctx == temporary_ctx_) {
        style_sheet += QString(
                    "  border-radius: 0.25em;"
                    "  color: #%1;"
                    "  background-color: #%2;"
                    )
                .arg(ws_css_warn_text, 6, 16, QChar('0'))
                .arg(ws_css_warn_background, 6, 16, QChar('0'));
    }

    style_sheet += "}";
    setStyleSheet(style_sheet);
    setText(si->text);
}

void LabelStack::pushText(QString &text, int ctx) {
    StackItem *si = new StackItem;

    if (ctx == temporary_ctx_) {
        temporary_timer_.stop();
        popText(temporary_ctx_);

        temporary_epoch_.start();
        temporary_timer_.start(temporary_flash_timeout_);
        emit toggleTemporaryFlash(true);
    }

    si->text = text;
    si->ctx = ctx;
    labels_.prepend(si);
    fillLabel();
}

void LabelStack::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        emit mousePressedAt(QPoint(event->globalPos()), Qt::LeftButton);
}

void LabelStack::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void LabelStack::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void LabelStack::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void LabelStack::contextMenuEvent(QContextMenuEvent *event)
{
    emit mousePressedAt(QPoint(event->globalPos()), Qt::RightButton);
}

void LabelStack::popText(int ctx) {
    QMutableListIterator<StackItem *> iter(labels_);

    while (iter.hasNext()) {
        if (iter.next()->ctx == ctx) {
            iter.remove();
            break;
        }
    }

    fillLabel();
}

void LabelStack::updateTemporaryStatus() {
    if (temporary_epoch_.elapsed() >= temporary_msg_timeout_) {
        popText(temporary_ctx_);
        emit toggleTemporaryFlash(false);
        temporary_timer_.stop();
    } else {
        for (int i = (num_flashes_ * 2); i > 0; i--) {
            if (temporary_epoch_.elapsed() >= temporary_flash_timeout_ * i) {
                emit toggleTemporaryFlash(i % 2);
                break;
            }
        }
    }
}

/*
 * Editor modelines
 *
 * Local Variables:
 * c-basic-offset: 4
 * tab-width: 8
 * indent-tabs-mode: nil
 * End:
 *
 * ex: set shiftwidth=4 tabstop=8 expandtab:
 * :indentSize=4:tabSize=8:noTabs=true:
 */