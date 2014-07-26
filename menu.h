#ifndef MENU_H
#define MENU_H

#include<QMainWindow>
#include<QPointer>
#include<QResizeEvent>
#include"drlview.h"
#include"prlview.h"

namespace Ui
{
    class Menu;
}

class Menu : public QMainWindow
{
    Q_OBJECT

    public:
        explicit Menu(QWidget *parent=0);
        ~Menu();

    protected:
        void resizeEvent(QResizeEvent* event);

    private slots:
        void on_OpenDRLInterface_clicked();

        void on_OpenDRLInterface_2_clicked();

private:
        Ui::Menu *ui;
        QPointer<DRLView>drl_view;
        QPointer<PRLView>prl_view;
};

#endif // MENU_H
