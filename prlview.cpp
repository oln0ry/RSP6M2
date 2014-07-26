#include"prlview.h"
#include"ui_prlview.h"
#include<QDesktopWidget>

PRLView::PRLView(QWidget *parent) : QMainWindow(parent),ui(new Ui::PRLView)
{
    ui->setupUi(this);
}

PRLView::~PRLView()
{
    delete ui;
}

void PRLView::resizeEvent(QResizeEvent*E)
{
    QDesktopWidget dw;
    QRect rec=dw.availableGeometry(dw.primaryScreen());
    quint16 w=E->size().width(),
            h=E->size().height();
    qreal d=static_cast<qreal>(3)/4,
          width_start=(rec.width()-d*h)/2;
    if(static_cast<qreal>(w)/h>d)
        ui->RenderPRL->setGeometry(width_start,0u,d*h,h);
    else
        ui->RenderPRL->setGeometry(0u,0u,w,h);
    QWidget::resizeEvent(E);
}
