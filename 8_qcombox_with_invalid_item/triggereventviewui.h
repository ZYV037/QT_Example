#ifndef TRIGGEREVENTVIEWUI_H
#define TRIGGEREVENTVIEWUI_H

#include <QDialog>
#include <QLabel>
#include <QRadioButton>
#include <QToolButton>
#include <QPushButton>
#include <QDateTimeEdit>

#include "triggereventviewhandler.h"

namespace QtCharts
{
    class QChartView;
    class QBarSet;
    class QStackedBarSeries;
    class QBarCategoryAxis;
    class QChart;
}

class QComboBox;
class QListWidget;

class TriggerEventViewUi : public QDialog
{
    Q_OBJECT
public:
    explicit TriggerEventViewUi(QWidget *parent = 0);
    void UpdateData();
    void UpdateTrigger();

signals:
    
public slots:
    void ShowDialog();
    void DateLinkChanged();
    void OnStartDateChanged();
    void OnEndDateChanged();
    void OnTrigggerSelectChanged();
    void OnPriviousBtnClicked();
    void OnNextBtnClicked();
    void UpdateImage();
private:
    void Init();
    void InitWidget();
    void InitLayout();
    void InitConnector();
private:
    QDateEdit* s_date_edit_;
    QDateEdit* e_date_edit_;
    QComboBox* trigger_select_cbox_;
    QListWidget* trigger_select_list;

    QRadioButton* day_rbtn_;
    QRadioButton* week_rbtn_;
    QRadioButton* month_rbtn_;
    QToolButton* privious_tbtn_;
    QToolButton* next_tbtn_;
//    QPushButton* close_btn_;
    //QLabel* image_label_;
    QFrame* tool_frame_;
//    QFrame* upper_frame_;
//    QFrame* bottom_frame_;

    UiInfo info_;
    TriggerEventViewHandler* handler_;

    QtCharts::QChartView* chartView;
    //bool bLastModifyIsStartDateOrNot;
};

#endif // TRIGGEREVENTVIEWUI_H
