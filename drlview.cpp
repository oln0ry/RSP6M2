#include"drlview.h"
#include"ui_drlview.h"
#include<QDesktopWidget>
#include<QDebug>

DRLView::DRLView(QWidget *parent) : QMainWindow(parent),ui(new Ui::DRLView)
{
    ui->setupUi(this);
    ui->RenderIKO->ChangeFPS(static_cast<qreal>(1000)/24);
}

DRLView::~DRLView()
{
    delete ui;
}

void DRLView::resizeEvent(QResizeEvent*E)
{
    QDesktopWidget dw;
    QRect rec=dw.availableGeometry(dw.primaryScreen());
    quint16 w=E->size().width(),
            h=E->size().height();
    qreal d=static_cast<qreal>(3)/4,
          width_start=(rec.width()-d*h)/2;
    if(static_cast<qreal>(w)/h>d)
        ui->RenderIKO->setGeometry(width_start,0u,d*h,h);
    else
        ui->RenderIKO->setGeometry(0u,0u,w,h);
    QWidget::resizeEvent(E);
}

int DRLView::heightForWidth(int w)const
{
    qDebug()<<2;
    return w;
}
