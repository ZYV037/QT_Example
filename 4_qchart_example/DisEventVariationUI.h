#ifndef DISEVENTVARIATIONUI_H
#define DISEVENTVARIATIONUI_H

#include <QDialog>
#include <QLabel>
#include <QDate>

#include "GraphicsComponent/GraphicsDataStruct.h"

class QDateTimeEdit;
class QRadioButton;
class QLabel;
namespace QtCharts
{
    class QChartView;
    class QBarSet;
    class QStackedBarSeries;
    class QBarCategoryAxis;
    class QChart;
}

class DisEventVariationUI : public QDialog
{
    Q_OBJECT
public:
    DisEventVariationUI(QWidget* parent = NULL);
    bool GenEventVarImage(Wmps2D::DateType type, QDate sDate, QDate edate, const QString& eventImageFile, const QString& energyImageFile);
    void SetRili2Visible(bool visible);

public slots:
    void Show24HourEvent();
    void ShowWeekEvent();
    void ShowEvent();
    void ShowEventStatistic();
    void DateTypeChanged();
    void ShowDialog();

    void OnDateChanged();
private:
    void Init();
    void InitWidget();
    void InitLayout();
    void InitConnect();
private:
    QDateTimeEdit* start_date_edit_;
    QDateTimeEdit* end_date_edit_;
    QDateTimeEdit* start_date_edit_2;
    QDateTimeEdit* end_date_edit_2;
    QRadioButton* day_rbtn_;
    QRadioButton* week_rbtn_;
    QRadioButton* month_rbtn_;
    QLabel* pLabelTo2;

    //QLabel* main_title_label_;

    QtCharts::QChartView* chartView;
    QtCharts::QChartView* chartView2;

    Wmps2D::DateType m_dateType;
};

#endif // DISEVENTVARIATIONUI_H
