#ifndef INDICATOR_H
#define INDICATOR_H

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

#include<QTime>
#include<QGLWidget>
#include<QResizeEvent>
#include<QBasicTimer>
#include<QTimerEvent>
#include<qmath.h>

#ifndef GetRadianValue
#define GetRadianValue(degree) (M_PI*degree)/180
#endif

#ifndef ROUND_DEGREE
#define ROUND_DEGREE 360u
#endif


struct Polar
{
    qreal x,y,angle;
};

class Indicator : public QGLWidget
{
    Q_OBJECT
    public:
        explicit Indicator(QWidget *parent=0);
        ~Indicator();
        bool IsActive(void)const;
        void ChangeFPS(qreal fps);

    signals:

    public slots:

    protected:
        void timerEvent(QTimerEvent*);
        void initializeGL();
        void resizeGL(int width,int height);
        void paintGL();
        int heightForWidth(int)const;
        virtual void ContinueSearch(void);
        QImage background,bg;
        QPainter *Painter;
        GLuint terrain;
        QPoint position;
        void GenerationRadians();
        void GenerationRayPath();
        void DrawRay()const;
        Polar radians[ROUND_DEGREE];
        QVector<Polar>circle;
        QVector<Polar*>ray;
        QVector<Polar*>::const_iterator ray_position;
        QBasicTimer timer;
        QColor ray_color;

    private:
        int width,
            height;
};

#endif // INDICATOR_H
