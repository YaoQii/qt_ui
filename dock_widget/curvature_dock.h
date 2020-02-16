#ifndef CURVATURE_DOCK_H
#define CURVATURE_DOCK_H

#include "MDockAncestor.h"

#include "ui_CurvatureForm.h"


class CurvatureDock : public MDockAncestor, public Ui_CurvatureForm
{
    Q_OBJECT
  public:
    CurvatureDock(QWidget *parent = 0);
    void updateXY(const QVector<double>& x, const QVector<double>& y);
  private:
    void changeEvent(QEvent*);
    void retranslateUi();
  private slots:
    void titleDoubleClick(QMouseEvent *event);
    void axisLabelDoubleClick(QCPAxis* axis, QCPAxis::SelectablePart part);
    void legendDoubleClick(QCPLegend* legend, QCPAbstractLegendItem* item);
    void selectionChanged();
    void mousePress();
    void mouseWheel();
    void addRandomGraph();
    void removeSelectedGraph();
    void removeAllGraphs();
    void contextMenuRequest(QPoint pos);
    void moveLegend();
    void graphClicked(QCPAbstractPlottable *plottable, int dataIndex);

};

#endif // CURVATURE_DOCK_H
