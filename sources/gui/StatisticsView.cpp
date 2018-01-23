#include "StatisticsView.h"
#include "ThemeManager.h"

StatisticsView::StatisticsView(QWidget *parent) : QWidget(parent)
{
    _Heading.setText("Statistics");

    _Layout.addWidget(&_Heading);
    _Layout.setMargin(5);

    setLayout(&_Layout);

    setMinimumHeight(480);
    setMinimumWidth(640);
    setMaximumHeight(480);
    setMaximumWidth(640);
}

void StatisticsView::resizeEvent(QResizeEvent *event)
{
    _Heading.resize(60, width());
}
