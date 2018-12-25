#include <QLayout>
#include <QRadioButton>
#include <QDateTimeEdit>
#include <QButtonGroup>

#include "DisEventVariationUI.h"
#include "WmpsInc/wmpsUIString.h"
#include "ColorControl/ColorControl.h"
#include "GraphicsComponent/GraphicsComponent.h"
#include "DisDataCenter/DisDataCenter.h"
#include "UIBase/uibase.h"

#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QStackedBarSeries>

#include <QtCharts/QHorizontalStackedBarSeries>

#include "DisDataCenter/DisDataCenter.h"
#include <QValueAxis>

#define IMAGE_WIDTH                     620
#define IMAGE_HEIGHT                    500

QT_CHARTS_USE_NAMESPACE

static QChart::AnimationOption chart_animation_opt = QChart::SeriesAnimations;
static QFont chart_title_font;
static QFont chart_axis_font;


DisEventVariationUI::DisEventVariationUI(QWidget *parent) :
    QDialog(parent)
{
    chart_title_font.setPointSize(13);
    chart_axis_font.setPointSize(10);
    Init();
}

bool DisEventVariationUI::GenEventVarImage(Wmps2D::DateType type, QDate sDate, QDate edate, const QString& eventImageFile, const QString& energyImageFile)
{
    SetRili2Visible(true);
    day_rbtn_->setVisible(false);
    week_rbtn_->setVisible(false);
    month_rbtn_->setVisible(false);
    start_date_edit_->setDate( sDate );
    end_date_edit_->setDate( edate );
    start_date_edit_2->setDate( sDate );
    end_date_edit_2->setDate(edate );
    switch(type)
    {
        case Wmps2D::DateType::DAY:
            this->setWindowTitle(STRING_MAINWINDOW_TWENTYFOUR_EVENT_VIEW);
            m_dateType = Wmps2D::DAY;
        break;
        case Wmps2D::DateType::WEEK:
            this->setWindowTitle(STRING_MAINWINDOW_WEEKLY_EVENT_VIEW);
            m_dateType = Wmps2D::WEEK;
        break;
    }

    chart_animation_opt =  QChart::NoAnimation;
    ShowEventStatistic();

    QPixmap::grabWidget(this, QRect(0, 0, this->width()/2, this->height())).save(eventImageFile);
    QPixmap::grabWidget(this, QRect(this->width()/2, 0, this->width()/2, this->height())).save(energyImageFile);
    return true;
}

void DisEventVariationUI::SetRili2Visible(bool visible)
{
    start_date_edit_2->setVisible(visible);
    end_date_edit_2->setVisible(visible);
    pLabelTo2->setVisible(visible);
}

void DisEventVariationUI::Show24HourEvent()
{
    start_date_edit_->setDate( DisDataCenter::GetInstance()->import_stime().date() );
    end_date_edit_->setDate( DisDataCenter::GetInstance()->import_etime().date());

    this->setWindowTitle(STRING_MAINWINDOW_TWENTYFOUR_EVENT_VIEW);
    //main_title_label_->setText(STRING_TIMEEVENTNUMCHART);
    m_dateType = Wmps2D::DAY;
    //day_rbtn_->setChecked(true);
    ShowEvent();
}

void DisEventVariationUI::ShowWeekEvent()
{
    start_date_edit_->setDate( DisDataCenter::GetInstance()->import_stime().date());
    end_date_edit_->setDate( DisDataCenter::GetInstance()->import_etime().date());

    this->setWindowTitle(STRING_MAINWINDOW_WEEKLY_EVENT_VIEW);
    //main_title_label_->setText(STRING_EVENTNUMCHART);
    m_dateType = Wmps2D::WEEK;
    week_rbtn_->setChecked(true);
    ShowEvent();
}

void DisEventVariationUI::ShowEvent()
 {
    chart_animation_opt =  QChart::SeriesAnimations;
    ShowEventStatistic();
    this->exec();
 }

void DisEventVariationUI::ShowEventStatistic()
{
    this->setFixedSize(IMAGE_WIDTH*2, IMAGE_HEIGHT + 30);
    QMap<QString, EventStatistic> eventSt;
    QMap<QString, double> energys;
    DisDataCenter::GetInstance()->GetStatisticData( start_date_edit_->date(), end_date_edit_->date(), eventSt, energys, m_dateType);

    QList<QBarSet*> barSets;

    QBarSet* bar1 = new QBarSet("＜0.0 ");// use "<" has a undisplay bug in qchart
    bar1->setColor(Qt::blue);
    QBarSet* bar2 = new QBarSet("≥0.0 ");
    bar2->setColor(Qt::green);
    QBarSet* bar3 = new QBarSet("≥1.0 ");
    bar3->setColor(Qt::yellow);
    QBarSet* bar4 = new QBarSet("≥2.0 ");
    bar4->setColor(Qt::red);
    barSets << bar1 << bar2 << bar3 << bar4;

    QStringList categories;
    int maxCount = 0;
    if(m_dateType == Wmps2D::DAY)
    {
        for(int i = 0; i<24; ++i)
        {
            *bar1 << eventSt[QString::number(i)].lessThan0;
            maxCount = maxCount < eventSt[QString::number(i)].lessThan0?eventSt[QString::number(i)].lessThan0:maxCount;
            *bar2 << eventSt[QString::number(i)].moreThan0;
            maxCount = maxCount < eventSt[QString::number(i)].moreThan0?eventSt[QString::number(i)].moreThan0:maxCount;
            *bar3 << eventSt[QString::number(i)].moreThan1;
            maxCount = maxCount < eventSt[QString::number(i)].moreThan1?eventSt[QString::number(i)].moreThan1:maxCount;
            *bar4 << eventSt[QString::number(i)].moreThan2;
            maxCount = maxCount < eventSt[QString::number(i)].moreThan2?eventSt[QString::number(i)].moreThan2:maxCount;
            categories << QString::number(i);
        }
    }
    else if(m_dateType == Wmps2D::WEEK)
    {
        for(int i = 0; i < 7; ++i)
        {
            *bar1 << eventSt[DisDataCenter::dayToWeek(start_date_edit_->date().addDays(i))].lessThan0;
            maxCount = maxCount < eventSt[DisDataCenter::dayToWeek(start_date_edit_->date().addDays(i))].lessThan0?eventSt[DisDataCenter::dayToWeek(start_date_edit_->date().addDays(i))].lessThan0:maxCount;
            *bar2 << eventSt[DisDataCenter::dayToWeek(start_date_edit_->date().addDays(i))].moreThan0;
            maxCount = maxCount < eventSt[DisDataCenter::dayToWeek(start_date_edit_->date().addDays(i))].moreThan0?eventSt[DisDataCenter::dayToWeek(start_date_edit_->date().addDays(i))].moreThan0:maxCount;
            *bar3 << eventSt[DisDataCenter::dayToWeek(start_date_edit_->date().addDays(i))].moreThan1;
            maxCount = maxCount < eventSt[DisDataCenter::dayToWeek(start_date_edit_->date().addDays(i))].moreThan1?eventSt[DisDataCenter::dayToWeek(start_date_edit_->date().addDays(i))].moreThan1:maxCount;
            *bar4 << eventSt[DisDataCenter::dayToWeek(start_date_edit_->date().addDays(i))].moreThan2;
            maxCount = maxCount < eventSt[DisDataCenter::dayToWeek(start_date_edit_->date().addDays(i))].moreThan2?eventSt[DisDataCenter::dayToWeek(start_date_edit_->date().addDays(i))].moreThan2:maxCount;
            categories << DisDataCenter::dayToWeek(start_date_edit_->date().addDays(i));
        }
    }
    else if(m_dateType == Wmps2D::MONTH)
    {
        QDate edt = end_date_edit_->date();
        for(QDate dt =  start_date_edit_->date(); dt <= edt; dt = dt.addDays(1) )
        {
            *bar1 << eventSt[QString::number(dt.day())].lessThan0;
            maxCount = maxCount < eventSt[QString::number(dt.day())].lessThan0?eventSt[QString::number(dt.day())].lessThan0:maxCount;
            *bar2 << eventSt[QString::number(dt.day())].moreThan0;
            maxCount = maxCount < eventSt[QString::number(dt.day())].moreThan0?eventSt[QString::number(dt.day())].moreThan0:maxCount;
            *bar3 << eventSt[QString::number(dt.day())].moreThan1;
            maxCount = maxCount < eventSt[QString::number(dt.day())].moreThan1?eventSt[QString::number(dt.day())].moreThan1:maxCount;
            *bar4 << eventSt[QString::number(dt.day())].moreThan2;
            maxCount = maxCount < eventSt[QString::number(dt.day())].moreThan2?eventSt[QString::number(dt.day())].moreThan2:maxCount;
            categories << QString::number(dt.day());
        }
    }

    QStackedBarSeries* series = new QStackedBarSeries();
    series->append(barSets);
    QChart* chart = new QChart();
    chart->removeAllSeries();
    chart->addSeries(series);
    chart->setTitle(STRING_EVENTNUMCHART_IMAGE);
    chart->setTitleFont(chart_title_font);
    chart->setAnimationOptions(chart_animation_opt);

    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    if(m_dateType == Wmps2D::DAY)
    {
        axisX->setTitleText(STRING_GRAPHIC_TIMEOFDAY);
    }
    else if(m_dateType == Wmps2D::WEEK)
    {
         axisX->setTitleText(STRING_GRAPHIC_DAYOFWEEK);
    }
    axisX->setTitleFont(chart_axis_font);
    chart->setAxisX(axisX, series);

    QValueAxis* axisY = new QValueAxis;
    axisY->setLabelFormat("%u");
    if( maxCount <= 5)
    {
        axisY->setRange(0,4);
        axisY->setTickCount(5);
    }

    axisY->setTitleText(STRING_GRAPHIC_SUM_NUMBERS);
    axisY->setTitleFont(chart_title_font);

    chart->setAxisY(axisY, series);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->legend()->setFont(chart_title_font);
    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setFixedSize(IMAGE_WIDTH, IMAGE_HEIGHT);

    QBarSet* bar5 = new QBarSet("");
    QStringList categories2;
    if(m_dateType == Wmps2D::DAY)
    {
        for(int i = 0; i<24; ++i)
        {
            *bar5 << energys[QString::number(i)];
            categories2 << QString::number(i);
        }
    }
    else if(m_dateType == Wmps2D::WEEK)
    {
        for(int i = 0; i < 7; ++i)
        {
            *bar5 << energys[DisDataCenter::dayToWeek(start_date_edit_->date().addDays(i))];
            categories2 << DisDataCenter::dayToWeek(start_date_edit_->date().addDays(i));
        }
    }
    else if(m_dateType == Wmps2D::MONTH)
    {
        QDate edt = end_date_edit_->date();

        for(QDate dt =  start_date_edit_->date(); dt <= edt; dt = dt.addDays(1) )
        {
            *bar5 << energys[QString::number(dt.day())];
            categories2 << QString::number(dt.day());
        }
    }

    QStackedBarSeries *series2 = new QStackedBarSeries();
    series2->append(bar5);

    QChart *chart2 = new QChart();
    chart2->addSeries(series2);
    chart2->setTitle(STRING_ENERGYCHART_IMAGE);
    chart2->setTitleFont(chart_title_font);
    chart2->setAnimationOptions(chart_animation_opt);


    QBarCategoryAxis *axis2 = new QBarCategoryAxis();
    axis2->append(categories2);
    chart2->createDefaultAxes();
    chart2->setAxisX(axis2, series2);
    chart2->axisY(series2)->setTitleText(STRING_GRAPHIC_ACCUMULATEENERGY);
    if(m_dateType == Wmps2D::DAY)
    {
        chart2->axisX(series2)->setTitleText(STRING_GRAPHIC_TIMEOFDAY);
    }
    else if(m_dateType == Wmps2D::WEEK)
    {
         chart2->axisX(series2)->setTitleText(STRING_GRAPHIC_DAYOFWEEK);
    }
    chart2->axisX(series2)->setTitleFont(chart_axis_font);
    chart2->axisY(series2)->setTitleFont(chart_title_font);

    chart2->legend()->setVisible(true);
    chart2->legend()->setAlignment(Qt::AlignBottom);
    chart2->legend()->setFont(chart_title_font);

    chartView2->setChart(chart2);
    chartView2->setRenderHint(QPainter::Antialiasing);
    chartView2->setFixedSize(IMAGE_WIDTH, IMAGE_HEIGHT);
}

void DisEventVariationUI::DateTypeChanged()
{
    if(day_rbtn_->isChecked())
    {
        //m_dateType = Wmps2D::DAY;
        end_date_edit_->setDate(start_date_edit_->date());
    }
    else if(week_rbtn_->isChecked())
    {
        //m_dateType = Wmps2D::WEEK;
        end_date_edit_->setDate( start_date_edit_->date().addDays(6) );
    }
    else if(month_rbtn_->isChecked())
    {
        //m_dateType = Wmps2D::MONTH;
        end_date_edit_->setDate( start_date_edit_->date().addMonths(1) );
    }
    else
    {
        m_dateType = Wmps2D::DEFAULT;
    }

    //ShowEventStatistic();
}

void DisEventVariationUI::ShowDialog()
{
    start_date_edit_->setDate( DisDataCenter::GetInstance()->import_stime().date() );
    end_date_edit_->setDate( DisDataCenter::GetInstance()->import_stime().date() );
}

void DisEventVariationUI::OnDateChanged()
{
    //m_dateType = Wmps2D::DEFAULT;
    day_rbtn_->setChecked(false);
    week_rbtn_->setChecked(false);
    month_rbtn_->setChecked(false);
    ShowEventStatistic();
}

void DisEventVariationUI::Init()
{
    InitWidget();
    InitLayout();
    InitConnect();
}

void DisEventVariationUI::InitWidget()
{
//    QFont font;
//    font.setPointSize(16);
//    main_title_label_ = new QLabel(this);
//    main_title_label_->setFont(font);

    QString rili_icon = IMAGE_DIR + "rili.png";

    start_date_edit_ = new QDateEdit(QDate::currentDate(), this);
    UIBase::CreateDateEdit(rili_icon, start_date_edit_);
    start_date_edit_->setDisplayFormat(QString("yyyy/MM/dd"));

    end_date_edit_ = new QDateEdit(QDate::currentDate(), this);
    UIBase::CreateDateEdit(rili_icon, end_date_edit_);
    end_date_edit_->setDisplayFormat(QString("yyyy/MM/dd"));
    pLabelTo2 = new QLabel("--",this);

    start_date_edit_2 = new QDateEdit(QDate::currentDate(), this);
    UIBase::CreateDateEdit(rili_icon, start_date_edit_2);
    start_date_edit_2->setDisplayFormat(QString("yyyy/MM/dd"));

    end_date_edit_2 = new QDateEdit(QDate::currentDate(), this);
    UIBase::CreateDateEdit(rili_icon, end_date_edit_2);
    end_date_edit_2->setDisplayFormat(QString("yyyy/MM/dd"));

    day_rbtn_ = new QRadioButton(STRING_DAY, this);
    week_rbtn_ = new QRadioButton(STRING_WEEK, this);
    month_rbtn_ = new QRadioButton(STRING_MONTH, this);
//    QButtonGroup* btn_group = new QButtonGroup(this);
//    btn_group->addButton(day_rbtn_);
//    btn_group->addButton(week_rbtn_);
//    btn_group->addButton(month_rbtn_);
//    day_rbtn_->setChecked(true);

    chartView = new QChartView;
    chartView2 = new QChartView;

    SetRili2Visible(false);
}

void DisEventVariationUI::InitLayout()
{
    QVBoxLayout* main_layout_g = new QVBoxLayout(this);

    QHBoxLayout* topLayout = new QHBoxLayout(this);
    QHBoxLayout* riliLayout = new QHBoxLayout(this);
    QLabel* pLabelTo = new QLabel("--");
    riliLayout->addSpacing(10);
    riliLayout->addWidget(start_date_edit_);
    riliLayout->addSpacing(4);
    riliLayout->addWidget(pLabelTo);
    riliLayout->addSpacing(4);
    riliLayout->addWidget(end_date_edit_);
    riliLayout->addSpacing(6);
    riliLayout->addWidget(day_rbtn_);
    riliLayout->addWidget(week_rbtn_);
    riliLayout->addWidget(month_rbtn_);
    riliLayout->addStretch(1);

    QHBoxLayout* riliLayout2 = new QHBoxLayout(this);
    riliLayout2->addSpacing(10);
    riliLayout2->addWidget(start_date_edit_2);
    riliLayout2->addSpacing(4);
    riliLayout2->addWidget(pLabelTo2);
    riliLayout2->addSpacing(4);
    riliLayout2->addWidget(end_date_edit_2);
    riliLayout2->addSpacing(6);
    riliLayout2->addStretch(1);

    topLayout->addLayout(riliLayout);
    topLayout->addLayout(riliLayout2);

    //topLayout->addWidget(main_title_label_, 0, Qt::AlignLeft);
    //topLayout->addStretch(1);

    QHBoxLayout* chartLayout = new QHBoxLayout;
    chartLayout->addWidget(chartView);
    chartLayout->addWidget(chartView2);
    chartLayout->setMargin(0);
    main_layout_g->addLayout(topLayout);
    main_layout_g->addLayout(chartLayout);

    main_layout_g->setMargin(0);
    main_layout_g->setSpacing(0);

    setLayout(main_layout_g);
}

 void DisEventVariationUI::InitConnect()
 {
     connect(start_date_edit_, SIGNAL(dateChanged(QDate)), this, SLOT(OnDateChanged()));
     connect(end_date_edit_, SIGNAL(dateChanged(QDate)), this, SLOT(OnDateChanged()));
     connect(day_rbtn_, SIGNAL(clicked()), this, SLOT(DateTypeChanged()));
     connect(week_rbtn_, SIGNAL(clicked()), this, SLOT(DateTypeChanged()));
     connect(month_rbtn_, SIGNAL(clicked()), this, SLOT(DateTypeChanged()));
 }

