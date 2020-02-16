#include "curvature_dock.h"

CurvatureDock::CurvatureDock(QWidget* parent)
  : MDockAncestor(parent)
{
  setupUi(getWidget());
  setObjectName("CurvatureWindow");
  retranslateUi();
  curvatureWidget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                  QCP::iSelectLegend | QCP::iSelectPlottables);
  curvatureWidget->xAxis->setRange(-8, 8);
  curvatureWidget->yAxis->setRange(-5, 5);
  curvatureWidget->axisRect()->setupFullAxesBox();

  curvatureWidget->plotLayout()->insertRow(0);
  QCPTextElement *title = new QCPTextElement(curvatureWidget, "Path curvature", QFont("sans", 17, QFont::Bold));
  curvatureWidget->plotLayout()->addElement(0, 0, title);

  curvatureWidget->xAxis->setLabel("index Axis");
  curvatureWidget->yAxis->setLabel("curvature Axis");
  curvatureWidget->legend->setVisible(true);
  QFont legendFont = font();
  legendFont.setPointSize(10);
  curvatureWidget->legend->setFont(legendFont);
  curvatureWidget->legend->setSelectedFont(legendFont);
  curvatureWidget->legend->setSelectableParts(QCPLegend::spItems); // legend box shall not be selectable, only legend items

  addRandomGraph();
  addRandomGraph();
  addRandomGraph();
  addRandomGraph();
  curvatureWidget->addGraph();
  curvatureWidget->graph()->setName(QString("Curature"));
//  curvatureWidget->graph()->setData(x, y);
  curvatureWidget->graph()->setLineStyle(QCPGraph::lsLine);
  curvatureWidget->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,5));

  curvatureWidget->xAxis->setRange(0, 50);
  curvatureWidget->yAxis->setRange(-0.15, 0.15);
  QPen graphPen;
  graphPen.setColor(QColor(19, 92, 69));
  graphPen.setWidthF(2);
  curvatureWidget->graph()->setPen(graphPen);
  curvatureWidget->replot();

  curvatureWidget->rescaleAxes();

  // connect slot that ties some axis selections together (especially opposite axes):
  connect(curvatureWidget, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
  // connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
  connect(curvatureWidget, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
  connect(curvatureWidget, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));

  // make bottom and left axes transfer their ranges to top and right axes:
  connect(curvatureWidget->xAxis, SIGNAL(rangeChanged(QCPRange)), curvatureWidget->xAxis2, SLOT(setRange(QCPRange)));
  connect(curvatureWidget->yAxis, SIGNAL(rangeChanged(QCPRange)), curvatureWidget->yAxis2, SLOT(setRange(QCPRange)));

  // connect some interaction slots:
  connect(curvatureWidget, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
  connect(curvatureWidget, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));
  connect(title, SIGNAL(doubleClicked(QMouseEvent*)), this, SLOT(titleDoubleClick(QMouseEvent*)));

  // connect slot that shows a message in the status bar when a graph is clicked:
  connect(curvatureWidget, SIGNAL(plottableClick(QCPAbstractPlottable*,int,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*,int)));
  // setup policy and connect slot for context menu popup:
  curvatureWidget->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(curvatureWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));

}

void CurvatureDock::changeEvent(QEvent * event)
{
    if (event->type() == QEvent::LanguageChange)
        retranslateUi();
    MDockAncestor::changeEvent(event);
}

void CurvatureDock::retranslateUi()
{
    Ui_CurvatureForm::retranslateUi(getWidget());

    setWindowTitle(tr("curvature"));

}

void CurvatureDock::titleDoubleClick(QMouseEvent* event)
{
  Q_UNUSED(event)
  if (QCPTextElement *title = qobject_cast<QCPTextElement*>(sender()))
  {
    // Set the plot title by double clicking on it
    bool ok;
    QString newTitle = QInputDialog::getText(this, "QCustomPlot example", "New plot title:", QLineEdit::Normal, title->text(), &ok);
    if (ok)
    {
      title->setText(newTitle);
      curvatureWidget->replot();
    }
  }
}

void CurvatureDock::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part)
{
  // Set an axis label by double clicking on it
  if (part == QCPAxis::spAxisLabel) // only react when the actual axis label is clicked, not tick label or axis backbone
  {
    bool ok;
    QString newLabel = QInputDialog::getText(this, "QCustomPlot example", "New axis label:", QLineEdit::Normal, axis->label(), &ok);
    if (ok)
    {
      axis->setLabel(newLabel);
      curvatureWidget->replot();
    }
  }
}

void CurvatureDock::legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item)
{
  // Rename a graph by double clicking on its legend item
  Q_UNUSED(legend)
  if (item) // only react if item was clicked (user could have clicked on border padding of legend where there is no item, then item is 0)
  {
    QCPPlottableLegendItem *plItem = qobject_cast<QCPPlottableLegendItem*>(item);
    bool ok;
    QString newName = QInputDialog::getText(this, "QCustomPlot example", "New graph name:", QLineEdit::Normal, plItem->plottable()->name(), &ok);
    if (ok)
    {
      plItem->plottable()->setName(newName);
      curvatureWidget->replot();
    }
  }
}

void CurvatureDock::selectionChanged()
{
  /*
   normally, axis base line, axis tick labels and axis labels are selectable separately, but we want
   the user only to be able to select the axis as a whole, so we tie the selected states of the tick labels
   and the axis base line together. However, the axis label shall be selectable individually.

   The selection state of the left and right axes shall be synchronized as well as the state of the
   bottom and top axes.

   Further, we want to synchronize the selection of the graphs with the selection state of the respective
   legend item belonging to that graph. So the user can select a graph by either clicking on the graph itself
   or on its legend item.
  */

  // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (curvatureWidget->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || curvatureWidget->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      curvatureWidget->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || curvatureWidget->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    curvatureWidget->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    curvatureWidget->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (curvatureWidget->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || curvatureWidget->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      curvatureWidget->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || curvatureWidget->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    curvatureWidget->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    curvatureWidget->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }

  // synchronize selection of graphs with selection of corresponding legend items:
  for (int i=0; i<curvatureWidget->graphCount(); ++i)
  {
    QCPGraph *graph = curvatureWidget->graph(i);
    QCPPlottableLegendItem *item = curvatureWidget->legend->itemWithPlottable(graph);
    if (item->selected() || graph->selected())
    {
      item->setSelected(true);
      graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
    }
  }
}

void CurvatureDock::mousePress()
{
  // if an axis is selected, only allow the direction of that axis to be dragged
  // if no axis is selected, both directions may be dragged

  if (curvatureWidget->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    curvatureWidget->axisRect()->setRangeDrag(curvatureWidget->xAxis->orientation());
  else if (curvatureWidget->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    curvatureWidget->axisRect()->setRangeDrag(curvatureWidget->yAxis->orientation());
  else
    curvatureWidget->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void CurvatureDock::mouseWheel()
{
  // if an axis is selected, only allow the direction of that axis to be zoomed
  // if no axis is selected, both directions may be zoomed

  if (curvatureWidget->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    curvatureWidget->axisRect()->setRangeZoom(curvatureWidget->xAxis->orientation());
  else if (curvatureWidget->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    curvatureWidget->axisRect()->setRangeZoom(curvatureWidget->yAxis->orientation());
  else
    curvatureWidget->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void CurvatureDock::updateXY(const QVector<double>& x, const QVector<double>& y)
{
  curvatureWidget->graph()->setData(x, y);

  curvatureWidget->replot();
}

void CurvatureDock::addRandomGraph()
{
  int n = 50; // number of points in graph
  double xScale = (rand()/(double)RAND_MAX + 0.5)*2;
  double yScale = (rand()/(double)RAND_MAX + 0.5)*2;
  double xOffset = (rand()/(double)RAND_MAX - 0.5)*4;
  double yOffset = (rand()/(double)RAND_MAX - 0.5)*10;
  double r1 = (rand()/(double)RAND_MAX - 0.5)*2;
  double r2 = (rand()/(double)RAND_MAX - 0.5)*2;
  double r3 = (rand()/(double)RAND_MAX - 0.5)*2;
  double r4 = (rand()/(double)RAND_MAX - 0.5)*2;
  QVector<double> x(n), y(n);
  for (int i=0; i<n; i++)
  {
    x[i] = (i/(double)n-0.5)*10.0*xScale + xOffset;
    y[i] = (qSin(x[i]*r1*5)*qSin(qCos(x[i]*r2)*r4*3)+r3*qCos(qSin(x[i])*r4*2))*yScale + yOffset;
  }

  curvatureWidget->addGraph();
  curvatureWidget->graph()->setName(QString("New graph %1").arg(curvatureWidget->graphCount()-1));
  curvatureWidget->graph()->setData(x, y);
  curvatureWidget->graph()->setLineStyle((QCPGraph::LineStyle)(rand()%5+2));
  if (rand()%100 > 50)
    curvatureWidget->graph()->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(rand()%14+1)));
  QPen graphPen;
  graphPen.setColor(QColor(rand()%245+10, rand()%245+10, rand()%245+10));
  graphPen.setWidthF(rand()/(double)RAND_MAX*2+1);
  curvatureWidget->graph()->setPen(graphPen);
  curvatureWidget->replot();
}

void CurvatureDock::removeSelectedGraph()
{
  if (curvatureWidget->selectedGraphs().size() > 0)
  {
    curvatureWidget->removeGraph(curvatureWidget->selectedGraphs().first());
    curvatureWidget->replot();
  }
}

void CurvatureDock::removeAllGraphs()
{
  curvatureWidget->clearGraphs();
  curvatureWidget->replot();
}

void CurvatureDock::contextMenuRequest(QPoint pos)
{
  QMenu *menu = new QMenu(this);
  menu->setAttribute(Qt::WA_DeleteOnClose);

  if (curvatureWidget->legend->selectTest(pos, false) >= 0) // context menu on legend requested
  {
    menu->addAction("Move to top left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignLeft));
    menu->addAction("Move to top center", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignHCenter));
    menu->addAction("Move to top right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignRight));
    menu->addAction("Move to bottom right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignRight));
    menu->addAction("Move to bottom left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignLeft));
  } else  // general context menu on graphs requested
  {
    menu->addAction("Add random graph", this, SLOT(addRandomGraph()));
    if (curvatureWidget->selectedGraphs().size() > 0)
      menu->addAction("Remove selected graph", this, SLOT(removeSelectedGraph()));
    if (curvatureWidget->graphCount() > 0)
      menu->addAction("Remove all graphs", this, SLOT(removeAllGraphs()));
  }

  menu->popup(curvatureWidget->mapToGlobal(pos));
}

void CurvatureDock::moveLegend()
{
  if (QAction* contextAction = qobject_cast<QAction*>(sender())) // make sure this slot is really called by a context menu action, so it carries the data we need
  {
    bool ok;
    int dataInt = contextAction->data().toInt(&ok);
    if (ok)
    {
      curvatureWidget->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)dataInt);
      curvatureWidget->replot();
    }
  }
}

void CurvatureDock::graphClicked(QCPAbstractPlottable *plottable, int dataIndex)
{
  // since we know we only have QCPGraphs in the plot, we can immediately access interface1D()
  // usually it's better to first check whether interface1D() returns non-zero, and only then use it.
  double dataValue = plottable->interface1D()->dataMainValue(dataIndex);
  QString message = QString("Clicked on graph '%1' at data point #%2 with value %3.").arg(plottable->name()).arg(dataIndex).arg(dataValue);
}

