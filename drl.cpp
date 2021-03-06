#include"drl.h"
#include<QDebug>

DRL::DRL(QWidget *parent) : Indicator(parent)
{
    GenerationRadians();
    GenerationRayPath();
    background.load(":/images/DRL_IKO.png","PNG");
    background=QGLWidget::convertToGLFormat(background);
    S.range.clear();
    S.azimuth.clear();
    SetCurrentRangeMode(Range::R_FIRST);
    SetCurrentScaleMode(Scale::S_SMALL);
    SetCurrentAzimuthMode(Azimuth::A_FIRST);
}

DRL::~DRL()
{

}

/**
 * Обновление остаточного изображения
 */
void DRL::ContinueSearch(void)
{
    updateGL();
    if(ray_position==ray.end()-1u)
    {
        ray_position=ray.begin();
    }
    ray_position++;
}


/**
 * Режим вывода отметок азимута
 */
DRL::Azimuth DRL::GetCurrentAzimuthMode(void)const
{
    return azimuth;
}

void DRL::SetCurrentAzimuthMode(const DRL::Azimuth a)
{
    azimuth=a;
    GenerationAzimuth();
}

/**
 * Режим вывода отметок дальности
 */
DRL::Range DRL::GetCurrentRangeMode(void)const
{
    return range;
}

void DRL::SetCurrentRangeMode(const DRL::Range r)
{
    range=r;
    GenerationRange();
}

/**
 * Масштаб
 */
DRL::Scale DRL::GetCurrentScaleMode(void)const
{
    return scale;
}

void DRL::SetCurrentScaleMode(const DRL::Scale s)
{
    scale=s;
    GenerationRange();
    //GenerationTrash();
    //GenerationLocalItems();
    //GenerationMeteo();
}

/**
 * Генерация координат отметок дальности
 */
void DRL::GenerationRange(void)
{
    S.range[scale].clear();
    quint8 j=0u,d=0u;
    qreal delta=CalcScaleValue(static_cast<qreal>(range));

    switch(range)
    {
        case Range::R_NO:
            return;
        case Range::R_FIRST:
            j=5u;
            break;
        case Range::R_SECOND:
        default:
            j=1u;
    }

    RoundLine cache;
    quint16 c=0u;
    for(qreal r=.0f;r<=1.0f;r+=delta,d++)
    {
        cache.width=d%j==0u ? 3.5f : 1.0f;
        cache.Coordinates=new Points[ROUND_DEGREE];
        c=0u;
        for(Points *i=radians,*e=radians+ROUND_DEGREE;i<e;i++,c++)
        {
            cache.Coordinates[c].angle=i->angle;
            cache.Coordinates[c].x=r*i->x;
            cache.Coordinates[c].y=r*i->y;
        }
        S.range[scale].append(cache);
    }
    Current.range=&S.range[scale];
}

/**
 * Отрисовка отметок дальности
 */
void DRL::DrawRange(void)const
{
    if(Current.range->isEmpty())
        return;
    qreal alpha;
          //focus=settings["system"]["focus"].toDouble(),
          //brightness=settings["brightness"]["range"].isValid() ? settings["brightness"]["range"].toDouble() : 1.0f;
    //brightness*=settings["system"]["brightness"].toDouble();
    QColor color=ray_color;
    for(QVector<RoundLine>::const_iterator it=(*Current.range).begin(),end=(*Current.range).end();it<end;it++)
    {
        glLineWidth(it->width/**focus*/);
        glBegin(GL_LINE_STRIP);
            for(Points *i=it->Coordinates,*e=it->Coordinates+ROUND_DEGREE;i<e;i++)
            {
                alpha=CalcAlpha(i->angle);
                if(alpha>.0f)
                {
                    //alpha=alpha<settings["system"]["lightning"].toDouble() ? 1.0f : settings["system"]["lightning"].toDouble()/alpha;
                    alpha=0.1/alpha;
                    if(alpha>1.0f)
                        alpha=1.0f;
                    color.setAlphaF(alpha/**brightness*/);
                    qglColor(color);
                    glVertex2f(i->x,i->y);
                }
            }
        glEnd();
    }
    /*
    for(QVector<RoundLine>::const_iterator it=Cache.range[scale].begin(),end=Cache.range[scale].end();it<end;it++)
    {
        glLineWidth(it->width);
        glBegin(GL_LINE_STRIP);
            for(Points *i=it->Coordinates,*e=it->Coordinates+ROUND_DEGREE;i<e;i++)
                glVertex2f(i->x,i->y);
        glEnd();
    }
    */
}

/**
 * Генерация координат отметок азимута
 */
void DRL::GenerationAzimuth(void)
{
    S.azimuth.clear();
    if(azimuth==Azimuth::A_NO)
        return;

    CenterStraightLine cache;
    for(Points *i=radians,*e=radians+ROUND_DEGREE;i<e;i+=azimuth)
    {
        cache.width=(i-radians)%A_SECOND>0u ? 1.0f : 2.0f; //3.5f;
        cache.Coordinates.angle=i->angle;
        cache.Coordinates.x=i->x;
        cache.Coordinates.y=i->y;
        S.azimuth.append(cache);
    }
    Current.azimuth=&S.azimuth;
}

/**
 * Отрисовка координат отметок азимута
 */
void DRL::DrawAzimuth(void)const
{
    if(Current.azimuth->isEmpty())
        return;
    qreal alpha;
          /*focus=settings["system"]["focus"].toDouble(),
          brightness=settings["brightness"]["azimuth"].isValid() ? settings["brightness"]["azimuth"].toDouble() : 1.0f;
    brightness*=settings["system"]["brightness"].toDouble();
    */
        QColor color=ray_color;
    for(QVector<CenterStraightLine>::const_iterator it=Current.azimuth->begin(),end=Current.azimuth->end();it<end;it++)
    {
        alpha=CalcAlpha(it->Coordinates.angle);
        if(alpha>.0f)
        {
            //alpha=alpha<settings["system"]["lightning"].toDouble() ? 1.0f : settings["system"]["lightning"].toDouble()/alpha;
            alpha=0.1/alpha;
            if(alpha>1.0f)
                alpha=1.0f;
            color.setAlphaF(/*brightness**/alpha);
            qglColor(color);
            glLineWidth(it->width/**focus*/);
            glBegin(GL_LINES);
                glVertex2f(.0f,.0f);
                glVertex2f(it->Coordinates.x,it->Coordinates.y);
            glEnd();
        }
    }
    /*
    for(QVector<CenterStraightLine>::const_iterator it=Cache.azimuth.begin(),end=Cache.azimuth.end();it<end;it++)
    {
        glLineWidth(it->width);
        glBegin(GL_LINES);
            glVertex2f(.0f,.0f);
            glVertex2f(it->Coordinates.x,it->Coordinates.y);
        glEnd();
    }*/
}

qreal DRL::CalcAlpha(qreal angle)const
{
    qreal alpha;
    if(IsAllVisible())
        alpha=1.0f;
    else
    {
        alpha=-1*((*ray_position)->angle-angle);
        if(alpha<.0f)
            alpha+=2u*M_PI;
    }
    return alpha;
}

void DRL::GenerationRadians(void)
{
    radians=new Points[ROUND_DEGREE];
    for(quint16 i=0u;i<ROUND_DEGREE;i++)
    {
        radians[i].degree=i;
        radians[i].angle=GetRadianValue(i);
        radians[i].x=qFastCos(radians[i].angle);
        radians[i].y=qFastSin(radians[i].angle);
    }
}

void DRL::GenerationRayPath(void)
{
    GenerationRayPath(ROUND_DEGREE);
}

void DRL::GenerationRayPath(quint16 angle)
{
    ray.clear();
    Points*i=radians,*end=radians+angle;
    //while(i<end)ray.append(clockwise ? end-- : i++);
    while(i<end)ray.prepend(i++);
    ray_position=ray.begin(); //Устанавливаем стартовую позицию луча
}

void DRL::DrawRay(void)const
{
    //QColor color=Color;
    //color.setAlphaF(settings["system"]["brightness"].toDouble());
    //qglColor(color);

    qglColor(ray_color);
    glPointSize(4.0f);
    glLineWidth(0.3f);
    if((*ray_position)->x>0)
        glScalef(1.13f,1.0f,1.0f);
    else
        glScalef(.85f,.99f,1.0f);

    glBegin(GL_LINES);
        glVertex2d(static_cast<GLdouble>(.0f),static_cast<GLdouble>(.0f));
        glVertex2d((*ray_position)->x,(*ray_position)->y);
    glEnd();

    qglColor(QColor(69u,121u,178u));
    for(QVector<RoundLine>::const_iterator it=(*Current.range).begin(),end=(*Current.range).end();it<end;it++)
    {
        glBegin(GL_POINTS);
        //glLineWidth(it->width/**focus*/);
        Points *i=it->Coordinates+(*ray_position)->degree;
        glVertex2f(i->x,i->y);
        glEnd();
    }
}

/**
 * Конвертация значения относительно масштаба
 */
template<typename T>T DRL::CalcScaleValue(const T value,DRL::Scale scale)const
{
    return static_cast<T>(value)/scale;
}

template<typename T>T DRL::CalcScaleValue(const T value)const
{
    return CalcScaleValue(value,scale);
}
