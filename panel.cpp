#include "panel.h"
#include <QVBoxLayout>
#include <QPushButton>

Panel::Panel(QWidget *parent) : QWidget(parent),
    _clear(new QPushButton(tr("&Clear"), this))
{
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(_clear);
    vbox->addStretch(1);
    setLayout(vbox);
}
