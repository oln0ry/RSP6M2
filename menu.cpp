#include"menu.h"
#include"ui_menu.h"

Menu::Menu(QWidget *parent) : QMainWindow(parent),ui(new Ui::Menu)
{
    ui->setupUi(this);
}

Menu::~Menu()
{
    delete ui;
}

void Menu::resizeEvent(QResizeEvent*E)
{
    ui->Background->setGeometry(0u,0u,E->size().width(),E->size().height());
}

void Menu::on_OpenDRLInterface_clicked()
{
    drl_view=new DRLView;
    drl_view->show();
}

void Menu::on_OpenDRLInterface_2_clicked()
{
    prl_view=new PRLView;
    prl_view->show();
}
