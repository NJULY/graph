#include "curve.h"

Curve::Curve()
{
    pointVector.clear();
    shape = shapeCurve;
    color = QColor(0,200,200);
}
Curve::Curve(QColor color)
{
    shape = shapeCurve;
    this->color = color;
}
void Curve::paint_Bezier(QPixmap *canvas)
{
    QPainter* canvasPainter=new QPainter(canvas);
    QPen pen = canvasPainter->pen();
    pen.setColor(color);
    canvasPainter->setPen(pen);

    //直接使用基函数计算n次贝塞尔曲线
    int n = pointVector.size() - 1;
    //计算二项式系数
    long long int *fact = new long long int[n+1];
    fact[0] = 1;
    for(int i = 1; i <= n; i++){
        fact[i] = fact[i-1] * i;
    }
    long long int *Cni = new long long int[n+1];
    for(int i = 0; i <= n; i++){
        Cni[i] = fact[n] / (fact[i] * fact[n-i]);
    }

    for(double u = 0.0; u <= 1.0; u += 0.00005){
        double x = 0.0, y = 0.0;
        for(int k = 0; k <= n; k++){
            x += Cni[k]*pow(u,k)*pow(1-u,n-k)*pointVector[k].x();
            y += Cni[k]*pow(u,k)*pow(1-u,n-k)*pointVector[k].y();
        }
        canvasPainter->drawPoint(QPoint(round(x),round(y)));
    }

    delete[]Cni;
    delete[]fact;
    delete canvasPainter;
}
void Curve::paint_B_Spline(QPixmap *canvas)
{
    QPainter* canvasPainter=new QPainter(canvas);
    QPen pen = canvasPainter->pen();
    pen.setColor(color);
    canvasPainter->setPen(pen);

    k = 3;//画3次B样条
    int n = pointVector.size()-1;
    U = new double[n+k+2];
    double delta = 1.0/(double)(n+k+1);
    U[0] = 0.0;
    for(int i = 1; i <= n+k+1; i++){
        U[i] = U[i-1]+delta;
        //printf("%f ",U[i]);
    }
    for(double u =U[k]; u <= U[n+1]; u+=0.00005){
        double x = 0.0, y = 0.0;
        for(int i = 0; i <= n; i++){
            double bik = Bik(i,k,u);
            x += bik*pointVector[i].x();
            y += bik*pointVector[i].y();
            //printf("%f ",bik);
        }
        canvasPainter->drawPoint(QPoint(round(x),round(y)));
    }

    delete[]U;
    delete canvasPainter;
}
double Curve::Bik(int i, int k, double u){
    if(k == 0){
        if(u>=U[i] && u<U[i+1])
            return 1.0;
        else
            return 0.0;
    }
    else{
        double delta1 = u - U[i];
        double delta2 = U[i+k] - U[i];
        if(fabs(delta2) < 1e-9){//重节点
            delta2 = 1.0;
            delta1 = 0.0;
        }
        double delta3 = U[i+k+1] - u;
        double delta4 = U[i+k+1] - U[i+1];
        if(fabs(delta4) < 1e-9){
            delta4 = 1.0;
            delta3 = 0.0;
        }
       // printf("%f %f %f %f",delta1,delta2,delta3,delta4);
        return (delta1/delta2)*Bik(i,k-1,u) + (delta3/delta4)*Bik(i+1,k-1,u);
    }
}
