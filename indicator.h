#ifndef INDICATOR_H
#define INDICATOR_H

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

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

class Indicator : public QGLWidget
{
    Q_OBJECT
    public:
        explicit Indicator(QWidget *parent=0);
        ~Indicator();
        bool IsActive(void)const;
        bool IsAllVisible(void)const;
        void SetAllVisible(bool);
        void ChangeFPS(qreal fps);
        struct Points
        {
            qreal x,y,angle;
            quint16 degree;
        }*radians;

        struct PointsR : public Points
        {
            qreal r;
        };
        struct RoundLine
        {
            qreal width;
            Points *Coordinates=nullptr;
        };

        struct RoundLineR
        {
            qreal width;
            PointsR *Coordinates=nullptr;
        };

        struct CenterStraightLine
        {
            qreal width;
            Points Coordinates;
        };

        struct TargetsStorage
        {
            qreal angle;
            qreal range;
        };

        enum Errors{TEXTURE_UNLOAD=0u};

    protected:
        struct Storage
        {
            QHash<quint16,QVector<PointsR> >trash,local_items;
            QHash<quint8,QHash<quint16,QVector<PointsR> > >meteo;
            QHash<quint16,QVector<RoundLine> >range;
            QHash<quint16,QVector<RoundLineR> >active_answer_trash;
            QHash<quint16,QHash<quint8,QVector<RoundLineR> > >active_insync_trash,targets;
            QVector<CenterStraightLine>azimuth;
            QVector<RoundLine>active_noise_trash;
        }S;

        struct Pointer
        {
            QVector<RoundLine>*range=nullptr;
            QVector<CenterStraightLine>*azimuth=nullptr;
        }Cache,Current;

        void timerEvent(QTimerEvent*);
        void initializeGL();
        void resizeGL(int width,int height);
        void paintGL();

        virtual qreal CalcAlpha(qreal angle)const=0;
        virtual void GenerationRange(void)=0;
        virtual void DrawRange(void)const=0;
        virtual void GenerationAzimuth(void)=0;
        virtual void DrawAzimuth(void)const=0;
        virtual void ContinueSearch(void)=0;
        virtual void GenerationRayPath(void)=0;
        virtual void DrawRay(void)const=0;
        virtual void GenerationRadians(void)=0;

        QImage background,bg;
        GLuint terrain[1];
        QColor ray_color;
        bool show=false;
        QVector<Points*>::const_iterator ray_position;
        QBasicTimer timer;
        QVector<Points>circle;
        QVector<Points*>ray;
        int width,height;

};

#endif // INDICATOR_H
