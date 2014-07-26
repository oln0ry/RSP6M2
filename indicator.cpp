#include"indicator.h"
#include<QDebug>

Indicator::Indicator(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers),parent)
{
    qsrand(QTime(0u,0u,0u).secsTo(QTime::currentTime()));
    //ray_color=QColor(120u,183u,255u);
    ray_color=QColor(55u,80u,100u);
    /*
    Points p;
    for(Points*i=radians,*end=radians+ROUND_DEGREE;i<end;i+=3) //Получаем координаты для отрисовки фона индикатора
    {
        p.x=i->x;
        p.y=i->y;
        p.angle=i->angle;
        circle.append(p);
    }
    */
}

Indicator::~Indicator()
{
    glDeleteTextures(1,terrain);
}

void Indicator::initializeGL()
{
    glMatrixMode(GL_PROJECTION); //Устанавливаем матрицу
    glShadeModel(GL_SMOOTH);//GL_FLAT
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glGenTextures(1,terrain);
}

void Indicator::resizeGL(int w,int h)
{
    width=w;
    height=h;
    bg=background.scaled(width,height,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    glEnable(GL_MULTISAMPLE);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(.0f,.0f,.0f,1.0f,1.0f,-1.0f);
    glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
}

void Indicator::paintGL()
{
    glLoadIdentity(); // загружаем матрицу
    glPushMatrix();
    //glTranslatef(.0f,0.12f,.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // чистим буфер изображения и буфер глубины
    glEnable(GL_TEXTURE_2D);
    glAlphaFunc(GL_GREATER,0.1f);
    glEnable(GL_ALPHA_TEST);
    glBindTexture(GL_TEXTURE_2D,terrain[0]);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,bg.width(),bg.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,bg.bits());
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR );

    glColor3d(1u,1u,1u);
    glViewport(0u,0u,width,height);
    glBegin(GL_QUADS);

    //Bottom Left
    glTexCoord2f(.0f,.0f);
    glVertex2f(-1.0f,-1.0f);

    //Top Left
    //glColor3f(0.2,0.7,0);
    glTexCoord2f(.0f,1.0f);
    glVertex2f(-1.0f,1.0f);

    //Top Right
    //glColor3f(0.9,0.1,0.5);
    glTexCoord2f(1.0f,1.0f);
    glVertex2f(1.0f,1.0f);

    //Bottom Right
    //glColor3f(0.6,0.6,0.6);
    glTexCoord2f(1.0f,.0f);
    glVertex2f(1.0f,-1.0f);

    glEnd();
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_TEXTURE_2D);



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
        for(QVector<Points>::const_iterator it=circle.begin();it<circle.end();it++)
            glVertex2d(it->x,it->y);
    glEnd();
*/
    glRotatef(90.0f,.0f,.0f,1.0f);
    DrawRay();
    DrawRange();
    DrawAzimuth();
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

bool Indicator::IsAllVisible(void)const
{
    return show;
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
