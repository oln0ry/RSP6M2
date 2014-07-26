#ifndef PRLVIEW_H
#define PRLVIEW_H

#include<QMainWindow>

namespace Ui
{
    class PRLView;
}

class PRLView : public QMainWindow
{
    Q_OBJECT

    public:
        explicit PRLView(QWidget *parent = 0);
        ~PRLView();
    protected:
        void resizeEvent(QResizeEvent* event);
    private:
        Ui::PRLView *ui;
};

#endif // PRLVIEW_H
