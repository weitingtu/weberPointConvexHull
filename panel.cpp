#include "panel.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QComboBox>

Panel::Panel(QWidget *parent) : QWidget(parent),
    _font_size(new QSpinBox(this)),
    _clear(new QPushButton(tr("&Clear"), this)),
    _input_size(new QSpinBox(this)),
    _generate(new QPushButton(tr("&Generate"), this)),
    _hexagonal(new QPushButton(tr("&Hexagonal"), this)),
    _convex_hull(new QPushButton(tr("&ConvexHull"), this)),
    _cdt(new QPushButton(tr("&DT"), this)),
    _decompose(new QPushButton(tr("&Decompose"), this)),
    _accomplish(new QPushButton(tr("&Accomplish"), this)),
    _difference(new QComboBox(this))
{
    _font_size->setValue(9);
    _input_size->setMinimum(0);
    _input_size->setMaximum(2000);
    _input_size->setValue(100);
    _difference->addItem("1%",      0.01);
    _difference->addItem("0.1%",    0.001);
    _difference->addItem("0.01%",   0.0001);
    _difference->addItem("0.001%",  0.00001);
    _difference->addItem("0.0001%", 0.000001);
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(_font_size);
    vbox->addWidget(_clear);
    vbox->addWidget(_createInputGroup());
    vbox->addWidget(_hexagonal);
    vbox->addWidget(_convex_hull);
    vbox->addWidget(_cdt);
    vbox->addWidget(_decompose);
    vbox->addWidget(_accomplish);
    vbox->addWidget(_difference);
    vbox->addStretch(1);
    setLayout(vbox);
}

QGroupBox* Panel::_createInputGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Input Setup"));

    QRadioButton *radio1 = new QRadioButton(tr("&Random"));
    QRadioButton *radio2 = new QRadioButton(tr("&Manual"));

    connect(radio1, SIGNAL(toggled(bool)), this, SLOT(_random_input(bool)));
    connect(radio2, SIGNAL(toggled(bool)), this, SLOT(_manual_input(bool)));

    radio1->setChecked(true);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(radio1);
    vbox->addWidget(_input_size);
    vbox->addWidget(_generate);
    vbox->addWidget(radio2);
    vbox->addStretch(1);
    groupBox->setLayout(vbox);

    return groupBox;
}

int Panel::get_input_size() const
{
    return _input_size->value();
}

void Panel::_random_input( bool checked ) const
{
    if( checked )
    {
        emit mode_changed( MODE::RANDOM_INPUT);
    }
}

void Panel::_manual_input( bool checked ) const
{
    if( checked )
    {
        emit mode_changed( MODE::MANUAL_INPUT);
    }
}
