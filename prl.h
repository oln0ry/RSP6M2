#ifndef PRL_H
#define PRL_H
#include"indicator.h"

class PRL : public Indicator
{
    Q_OBJECT
    public:
        explicit PRL(QWidget *parent=0);
        ~PRL();

        enum Azimuth{A_NO=0u,A_FIRST=10u,A_SECOND=30u};
        enum Range{R_NO=0u,R_FIRST=10u,R_SECOND=50u};
        enum Scale{S_SMALL=45u,S_MIDDLE=90u,S_LARGE=150u};
        enum WorkMode{WM_AKT=0u,WM_PASS,WM_SDC};

        Azimuth GetCurrentAzimuthMode(void)const;
        void SetCurrentAzimuthMode(const Azimuth);
        Range GetCurrentRangeMode(void)const;
        void SetCurrentRangeMode(const Range);
        Scale GetCurrentScaleMode(void)const;
        void SetCurrentScaleMode(const Scale);
        signals:

    public slots:
    protected:
        qreal CalcAlpha(qreal angle)const;
        void ContinueSearch(void);
        void GenerationRange(void);
        void DrawRange(void)const;
        void GenerationAzimuth(void);
        void DrawAzimuth(void)const;
        void GenerationRayPath(void);
        void DrawRay(void)const;
        void GenerationRadians(void);

        Azimuth azimuth=Azimuth::A_NO;
        Range range=Range::R_NO;
        Scale scale=Scale::S_SMALL;
        template<typename T>T CalcScaleValue(const T value,Scale scale)const;
        template<typename T>T CalcScaleValue(const T value)const;

};

#endif // PRL_H
