#include"indicator.h"
#include<QDebug>

Indicator::Indicator(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers),parent)
{
    qsrand(QTime(0u,0u,0u).secsTo(QTime::currentTime()));
    ray_color=QColor(120u,183u,255u);
    background.load(":/images/DRL_IKO.png");
    //background=QGLWidget::convertToGLFormat(background);
    position=QPoint(0u,0u);
    Painter=new QPainter;
    GenerationRadians();
Polar p;
    for(Polar*i=radians,*end=radians+ROUND_DEGREE;i<end;i+=3) //Получаем координаты для отрисовки фона индикатора
    {
        p.x=i->x;
        p.y=i->y;
        p.angle=i->angle;
        circle.append(p);
    }

    GenerationRayPath();
}

Indicator::~Indicator()
{

}

int Indicator::heightForWidth(int)const
{
    return width;
}

void Indicator::initializeGL()
{
    glMatrixMode(GL_PROJECTION); //Устанавливаем матрицу
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

void Indicator::resizeGL(int w,int h)
{
    width=w;
    height=h;
    bg=background.scaled(width,height,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    glEnable(GL_MULTISAMPLE);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(.0f,.0f,.0f,1.0,1.0,-1.0f);
    glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
}

void Indicator::paintGL()
{
    glLoadIdentity(); // загружаем матрицу
    glPushMatrix();
    //glTranslatef(.0f,0.12f,.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // чистим буфер изображения и буфер глубины
    terrain=bindTexture(bg);
    Painter->begin(this);
    drawTexture(position,terrain);
    Painter->end();
    /*
    if(width>height)
        glViewport(static_cast<GLint>(0u),static_cast<GLint>(0u),static_cast<GLint>(height),static_cast<GLint>(height));
    else
        glViewport(static_cast<GLint>(0u),static_cast<GLint>(0u),static_cast<GLint>(width),static_cast<GLint>(width));
*/
    glViewport(static_cast<GLint>(0),static_cast<GLint>(height/4),static_cast<GLint>(width),static_cast<GLint>(3*height/4));
    glScalef(.85f,.85f,1.0f);
    glTranslatef(-.01f,-.04f,.0f);

    glTranslatef(.02f,-.14f,.0f);
    /*
    qglColor(Qt::black);
    glLineWidth(2.0f);
    glBegin(GL_TRIANGLE_FAN);
        for(QVector<Polar>::const_iterator it=circle.begin();it<circle.end();it++)
            glVertex2d(it->x,it->y);
    glEnd();
*/
    glRotatef(90.0f,.0f,.0f,1.0f);
    DrawRay();
    glPopMatrix();
}

void Indicator::timerEvent(QTimerEvent*E)
{
    if(timer.timerId()==E->timerId())
        ContinueSearch();
    QWidget::timerEvent(E);
}

bool Indicator::IsActive(void)const
{
    return timer.isActive();
}

void Indicator::ChangeFPS(qreal fps)
{
    if(fps<=.0f && IsActive())
        timer.stop();
    if(fps>.0f)
    {
        if(IsActive())
            timer.stop();
        timer.start(fps,this);
    }
}

/**
 * Обновление остаточного изображения
 */
void Indicator::ContinueSearch(void)
{
    updateGL();
    if(ray_position==ray.end()-1u)
    {
        ray_position=ray.begin();
    }
    ray_position++;
}

void Indicator::GenerationRadians(void)
{
    /*
    for(quint16 i=0u;i<ROUND_DEGREE;i++)
    {
        radians[i].angle=GetRadianValue(i);
        radians[i].x=qFastCos(radians[i].angle);
        radians[i].y=qFastSin(radians[i].angle);
    }
    */
    for(quint16 i=0u;i<ROUND_DEGREE;i++)
    {
        radians[i].angle=GetRadianValue(i);
        radians[i].x=qFastCos(radians[i].angle);
        radians[i].y=qFastSin(radians[i].angle);
/*
        if(radians[i].x>0)
            radians[i].x*=1.14;
        else
            radians[i].x*=0.8;*/
    }
}

void Indicator::GenerationRayPath()
{
    ray.clear();
    Polar*i=radians,*end=radians+ROUND_DEGREE;
    //while(i<end)ray.append(clockwise ? end-- : i++);
    while(i<end)ray.prepend(i++);
    ray_position=ray.begin(); //Устанавливаем стартовую позицию луча
}

void Indicator::DrawRay()const
{
    //QColor color=Color;
    //color.setAlphaF(settings["system"]["brightness"].toDouble());
    //qglColor(color);

    qglColor(ray_color);
    glLineWidth(3.0f);
    if((*ray_position)->x>0)
        glScalef(1.13f,1.0f,1.0f);
    else
        glScalef(0.85f,0.99f,1.0f);
    glBegin(GL_LINES);
        glVertex2d(static_cast<GLdouble>(.0f),static_cast<GLdouble>(.0f));
        glVertex2d((*ray_position)->x,(*ray_position)->y);
    glEnd();
}