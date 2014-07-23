#ifndef DRLVIEW_H
#define DRLVIEW_H

#include<QMainWindow>

namespace Ui
{
    class DRLView;
}

class DRLView : public QMainWindow
{
    Q_OBJECT

    public:
        explicit DRLView(QWidget *parent=0);
        ~DRLView();

    protected:
        void resizeEvent(QResizeEvent* event);
        int heightForWidth(int w)const;

    private:
        Ui::DRLView *ui;
};

#endif // DRLVIEW_H
