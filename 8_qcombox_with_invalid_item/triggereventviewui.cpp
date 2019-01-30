#include <QLayout>
#include <QButtonGroup>
#include "triggereventviewui.h"
#include "UIBase/uibase.h"
#include "WmpsInc/DataStruct.h"
#include "GraphicsComponent/GraphicsComponent.h"
#include "WmpsInc/wmpsUIString.h"

#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QStackedBarSeries>

#include <QtCharts/QHorizontalStackedBarSeries>

#include "DisDataCenter/DisDataCenter.h"
#include <QValueAxis>
#include <QComboBox>
#include <QListWidget>


#define IMAGE_TRIGGER_EVENT_VIEW_PRIVIOUS_CURRENT   "a_1.png"
#define IMAGE_TRIGGER_EVENT_VIEW_PRIVIOUS_HOVER     "a_2.PNG"
#define IMAGE_TRIGGER_EVENT_VIEW_PRIVIOUS_CLICKED   "a_3.PNG"
#define IMAGE_TRIGGER_EVENT_VIEW_NEXT_CURRENT       "b_1.png"
#define IMAGE_TRIGGER_EVENT_VIEW_NEXT_HOVER         "b_2.png"
#define IMAGE_TRIGGER_EVENT_VIEW_NEXT_CLICKED       "b_3.png"

QT_CHARTS_USE_NAMESPACE

TriggerEventViewUi::TriggerEventViewUi(QWidget *parent) :
    QDialog(parent)/*,bLastModifyIsStartDateOrNot(true)*/
{
    Init();
}

void TriggerEventViewUi::DateLinkChanged()
{
//    if(bLastModifyIsStartDateOrNot)
//    {
        if(day_rbtn_->isChecked())
        {
            e_date_edit_->setDate(s_date_edit_->date());
        }
        else if(week_rbtn_->isChecked())
        {
            e_date_edit_->setDate(s_date_edit_->date().addDays(6));
        }
        else if(month_rbtn_->isChecked())
        {
            e_date_edit_->setDate(s_date_edit_->date().addMonths(1).addDays(-1));
        }
//    }
//    else
//    {
//        if(day_rbtn_->isChecked())
//        {
//            s_date_edit_->setDate(e_date_edit_->date());
//        }
//        else if(week_rbtn_->isChecked())
//        {
//            s_date_edit_->setDate(e_date_edit_->date().addDays(-6));
//        }
//        else if(month_rbtn_->isChecked())
//        {
//            s_date_edit_->setDate(e_date_edit_->date().addMonths(-1));
//        }
//    }
}

void TriggerEventViewUi::OnStartDateChanged()
{
    //bLastModifyIsStartDateOrNot = true;
    DateLinkChanged();

    if (info_.sdate != s_date_edit_->date())
    {
        info_.sdate = s_date_edit_->date();
        handler_->GetUiData(info_);
    }

    UpdateImage();
}

void TriggerEventViewUi::OnEndDateChanged()
{
    //bLastModifyIsStartDateOrNot = false;
    DateLinkChanged();
    if (info_.edate != e_date_edit_->date())
    {
        info_.edate = e_date_edit_->date();
        handler_->GetUiData(info_);
    }
    UpdateImage();
}

void TriggerEventViewUi::OnTrigggerSelectChanged()
{
    int index = trigger_select_cbox_->currentIndex();

    int cnt = info_.trigger_eventinfos.keys().count() - 1;

    privious_tbtn_->setEnabled(index > 0);
    next_tbtn_->setEnabled(index != cnt);

    if (index < 0)
    {
        return;
    }

    UpdateImage();
}

void TriggerEventViewUi::OnPriviousBtnClicked()
{
    int index = trigger_select_cbox_->currentIndex() - 1;
    if (index == 0)
    {
        privious_tbtn_->setEnabled(false);
    }
    else
    {

        privious_tbtn_->setEnabled(true);
        next_tbtn_->setEnabled(true);
    }

    trigger_select_cbox_->setCurrentIndex(index);
    UpdateImage();
}

void TriggerEventViewUi::OnNextBtnClicked()
{
    int index = trigger_select_cbox_->currentIndex() + 1;
    int cnt = info_.trigger_eventinfos.keys().count() - 1;

    if (index == cnt)
    {
        next_tbtn_->setEnabled(false);
    }
    else
    {
        privious_tbtn_->setEnabled(true);
        next_tbtn_->setEnabled(true);
    }

    trigger_select_cbox_->setCurrentIndex(index);
    UpdateImage();
}

void TriggerEventViewUi::UpdateData()
{
    handler_->GetUiData(info_, true);

    s_date_edit_->setDate(info_.sdate);
    s_date_edit_->setDateRange(info_.sdate, info_.edate);

    //e_date_edit_->setDate(info_.edate);

    UpdateTrigger();

    UpdateImage();
}

void TriggerEventViewUi::UpdateTrigger()
{
    trigger_select_cbox_->clear();
    QMapIterator<int, TriggerEventInfo> iter(info_.trigger_eventinfos);
    while (iter.hasNext())
    {
        iter.next();
        //trigger_select_cbox_->addItem(iter.value().trigger_name);

        QListWidgetItem *pItem = new QListWidgetItem(iter.value().trigger_name, trigger_select_list);\

        if((iter.value().station_flags & 1) ||(iter.value().station_flags & 2) )
        {
            //trigger_select_cbox_
            //Qt::ItemFlags flag = pItem->flags();
            pItem->setFlags( Qt::NoItemFlags );
        }

        trigger_select_list->addItem(pItem);
    }

    if (info_.trigger_eventinfos.count() > 1)
    {
        privious_tbtn_->setEnabled(false);
        next_tbtn_->setEnabled(true);
    }
}

void TriggerEventViewUi::ShowDialog()
{
    UpdateData();
    this->exec();
}

void TriggerEventViewUi::UpdateImage()
{
    int index = trigger_select_cbox_->currentIndex();
    if (index == -1) {
        return;
    }

    int cid = info_.trigger_eventinfos.keys().at(index);
    TriggerEventInfo tr_info = info_.trigger_eventinfos[cid];

    QBarSet* bar = new QBarSet("");
    QStringList categories2;

    Wmps2D::DateType dataType;
    int maxCount = 0;
    if (day_rbtn_->isChecked())
    {
        dataType= Wmps2D::DAY;
        for (int i = 0; i < DAILY_LENTH; i++)
        {
            //data.values.push_back(tr_info.daily[i]);
            bar->append(tr_info.daily[i]);
            maxCount = maxCount < tr_info.daily[i]?tr_info.daily[i]:maxCount;
            categories2 << QString::number(i);
        }
    }
    else if (week_rbtn_->isChecked())
    {
        dataType = Wmps2D::WEEK;
        for (int i = 0; i < WEEK_LENTH; i++){
            //data.values.push_back(tr_info.week[i]);
            bar->append(tr_info.week[i]);
            maxCount = maxCount < tr_info.week[i]?tr_info.week[i]:maxCount;
            categories2 << DisDataCenter::dateToWeek(s_date_edit_->date().addDays(i));
        }
    }
    else if ( month_rbtn_->isChecked() )
    {
        dataType = Wmps2D::MONTH;
        int day_of_month = (tr_info.month.size() > 0)? tr_info.month.size():28;
        for (int i = 0; i < day_of_month; i++){
            //data.values.push_back(tr_info.month[i]);

            if(tr_info.month.size() > 0)
            {
                bar->append(tr_info.month[i]);
                maxCount = maxCount < tr_info.month[i]?tr_info.month[i]:maxCount;
            }
            else {
                 bar->append(0);
            }

            categories2 << QString::number( s_date_edit_->date().addDays(i).day());
        }
    }


    QStackedBarSeries *series2 = new QStackedBarSeries();
    series2->append(bar);

    QChart *chart2 = new QChart();
    chart2->addSeries(series2);
    //chart2->setTitle(STRING_ENERGYCHART_IMAGE);
    chart2->setAnimationOptions(QChart::SeriesAnimations);


    QBarCategoryAxis *axis2 = new QBarCategoryAxis();
    axis2->append(categories2);
    chart2->createDefaultAxes();
    if(dataType == Wmps2D::DAY)
    {
       axis2->setTitleText(STRING_GRAPHIC_TIMEOFDAY);
    }
    else if(dataType == Wmps2D::WEEK)
    {
       axis2->setTitleText(STRING_GRAPHIC_DAYOFWEEK);
    }
    else if(dataType == Wmps2D::MONTH)
    {
       axis2->setTitleText(STRING_MONTH + "(" +STRING_DAY +")");
    }
    chart2->setAxisX(axis2, series2);


    QValueAxis* axisY = new QValueAxis;
    axisY->setLabelFormat("%u");
    if( maxCount <= 5)
    {
        axisY->setRange(0,4);
        axisY->setTickCount(5);
    }
    axisY->setTitleText(STRING_EVENT_NUM);
    chart2->setAxisY(axisY, series2);

    chart2->legend()->setVisible(false);
    chartView->setChart(chart2);
    chartView->setRenderHint(QPainter::Antialiasing);
 }

void TriggerEventViewUi::Init()
{
    InitWidget();
    InitLayout();
    InitConnector();
}

void TriggerEventViewUi::InitWidget()
{
    s_date_edit_ = new QDateEdit(QDate::currentDate());
    e_date_edit_ = new QDateEdit(QDate::currentDate());
    QString rili_icon = IMAGE_DIR + "rili.png";
    UIBase::CreateDateEdit(rili_icon, s_date_edit_);
    UIBase::CreateDateEdit(rili_icon, e_date_edit_);
    s_date_edit_->setDisplayFormat(QString("yyyy/MM/dd"));
    e_date_edit_->setDisplayFormat(QString("yyyy/MM/dd"));

    trigger_select_list = new QListWidget(this);
    trigger_select_cbox_ = new QComboBox(this);
    trigger_select_cbox_->setModel( trigger_select_list->model() );
    trigger_select_cbox_->setView( trigger_select_list );
    trigger_select_cbox_->setEditable(true);
    trigger_select_cbox_->setFocusPolicy(Qt::NoFocus);

    day_rbtn_ = new QRadioButton(STRING_DAY);
    week_rbtn_ = new QRadioButton(STRING_WEEK);
    month_rbtn_ = new QRadioButton(STRING_MONTH);
    QButtonGroup* btn_group = new QButtonGroup;
    btn_group->addButton(day_rbtn_);
    btn_group->addButton(week_rbtn_);
    btn_group->addButton(month_rbtn_);
    day_rbtn_->setChecked(true);

    privious_tbtn_ = new QToolButton;
    UIBase::CreateToolBtn(privious_tbtn_,
                  STRING_TRIGGER_EVENT_VIEW_PRIVIOUS_STATION,
                  IMAGE_DIR+IMAGE_TRIGGER_EVENT_VIEW_PRIVIOUS_CURRENT,
                  IMAGE_DIR+IMAGE_TRIGGER_EVENT_VIEW_PRIVIOUS_HOVER,
                  IMAGE_DIR+IMAGE_TRIGGER_EVENT_VIEW_PRIVIOUS_CLICKED
                  );

    next_tbtn_ = new QToolButton;
    UIBase::CreateToolBtn(next_tbtn_,
                          STRING_TRIGGER_EVENT_VIEW_NEXT_STATION,
                  IMAGE_DIR+IMAGE_TRIGGER_EVENT_VIEW_NEXT_CURRENT,
                  IMAGE_DIR+IMAGE_TRIGGER_EVENT_VIEW_NEXT_HOVER,
                  IMAGE_DIR+IMAGE_TRIGGER_EVENT_VIEW_NEXT_CLICKED
                  );
    privious_tbtn_->setEnabled(false);
    next_tbtn_->setEnabled(false);
//    close_btn_ = new QPushButton(STRING_CLOSE);
    //image_label_ = new QLabel;
    //image_label_->setFixedSize(900, 400);
    tool_frame_ = new QFrame;

    chartView = new QChartView(this);
    chartView->setFixedSize(900, 400);

    handler_ = new TriggerEventViewHandler;
    this->setWindowTitle(STRING_TRIGGER_EVENT_VIEW_TITLE);
    this->setFixedSize(900, 450);
}

void TriggerEventViewUi::InitLayout()
{
    QHBoxLayout* tool_layout_h = new QHBoxLayout;
    tool_layout_h->addWidget(s_date_edit_);
    tool_layout_h->addWidget(e_date_edit_);
    tool_layout_h->addWidget(trigger_select_cbox_);
    tool_layout_h->addWidget(day_rbtn_);
    tool_layout_h->addWidget(week_rbtn_);
    tool_layout_h->addWidget(month_rbtn_);
    tool_layout_h->addStretch(1);
    tool_layout_h->addWidget(privious_tbtn_);
    tool_layout_h->addWidget(next_tbtn_);
    tool_frame_->setLayout(tool_layout_h);

    QVBoxLayout* main_layout_v = new QVBoxLayout;
    main_layout_v->addWidget(tool_frame_);
    main_layout_v->addWidget(chartView);
    main_layout_v->setMargin(0);
    main_layout_v->setSpacing(5);

    this->setLayout(main_layout_v);
}

void TriggerEventViewUi::InitConnector()
{
    connect(s_date_edit_, SIGNAL(dateChanged(QDate)), this, SLOT(OnStartDateChanged()));
    connect(e_date_edit_, SIGNAL(dateChanged(QDate)), this, SLOT(OnEndDateChanged()));
    connect(trigger_select_cbox_, SIGNAL(currentIndexChanged(int)), this, SLOT(OnTrigggerSelectChanged()));
    connect(day_rbtn_, SIGNAL(clicked()), this, SLOT(DateLinkChanged()));
    connect(week_rbtn_, SIGNAL(clicked()), this, SLOT(DateLinkChanged()));
    connect(month_rbtn_, SIGNAL(clicked()), this, SLOT(DateLinkChanged()));
    connect(privious_tbtn_, SIGNAL(clicked()), this, SLOT(OnPriviousBtnClicked()));
    connect(next_tbtn_, SIGNAL(clicked()), this, SLOT(OnNextBtnClicked()));

//    connect(close_btn_, SIGNAL(clicked()), this, SLOT(close()));
}
