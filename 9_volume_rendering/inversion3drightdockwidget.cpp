#include "inversion3drightdockwidget.h"
#include "wdd_ts_string.h"
#include <QVBoxLayout>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGroupBox>
#include "rangecolorsetdialog.h"
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include "rangecolordefine.h"
#include "trtparser.h"
#include "volumetric.h"
#include <QScrollArea>

Inversion3dRightDockWidget::Inversion3dRightDockWidget(QWidget* parent, VolumetricModifier* modifier):\
     WtgeoDockWidget(TS_INVERSION_3DIMAGING_RIGHTDOCKWIDGET, parent),modifier_(modifier)
{
    QWidget* mainWidget = new QWidget(this);
    QVBoxLayout *vLayout = new QVBoxLayout(mainWidget);

    if(rangeColorDialog_ == nullptr)
    {
        rangeColorDialog_ = new RangeColorSetDialog(this);
    }

    QCheckBox *sliceXCheckBox = new QCheckBox(this);
    sliceXCheckBox->setText(QStringLiteral("Slice X"));
    sliceXCheckBox->setChecked(false);
    QCheckBox *sliceYCheckBox = new QCheckBox(this);
    sliceYCheckBox->setText(QStringLiteral("Slice Y"));
    sliceYCheckBox->setChecked(false);
    QCheckBox *sliceZCheckBox = new QCheckBox(this);
    sliceZCheckBox->setText(QStringLiteral("Slice Z"));
    sliceZCheckBox->setChecked(false);

    QSlider *sliceXSlider = new QSlider(Qt::Horizontal, this);
    QSlider *sliceYSlider = new QSlider(Qt::Horizontal, this);
    QSlider *sliceZSlider = new QSlider(Qt::Horizontal, this);

    QLabel *sliceImageXLabel = new QLabel(this);
    QLabel *sliceImageYLabel = new QLabel(this);
    QLabel *sliceImageZLabel = new QLabel(this);

    sliceImageXLabel->setMinimumSize(QSize(400, 100));
    sliceImageYLabel->setMinimumSize(QSize(400, 100));
    sliceImageZLabel->setMinimumSize(QSize(100, 100));

    sliceImageXLabel->setMaximumSize(QSize(400, 100));
    sliceImageYLabel->setMaximumSize(QSize(400, 100));
    sliceImageZLabel->setMaximumSize(QSize(100, 100));

    sliceImageXLabel->setFrameShape(QFrame::Box);
    sliceImageYLabel->setFrameShape(QFrame::Box);
    sliceImageZLabel->setFrameShape(QFrame::Box);

    sliceImageXLabel->setScaledContents(true);
    sliceImageYLabel->setScaledContents(true);
    sliceImageZLabel->setScaledContents(true);

    QCheckBox *drawSliceFramesCheckBox = new QCheckBox(this);
    drawSliceFramesCheckBox->setText(QStringLiteral("Draw slice frames"));
    drawSliceFramesCheckBox->setChecked(false);

    setMinimumWidth(420);

    colorSetButton_ = new QPushButton("", this);

    QLabel* trtTitle = new QLabel("Trt文件", this);
    trtFolder_ = new QLineEdit(this);
    QPushButton* selectFolderButton = new QPushButton("浏览",this);
    connect(selectFolderButton, &QPushButton::clicked, [this](){
        QFileDialog *fileDialog = new QFileDialog(this);
        fileDialog->setWindowTitle(tr("选择Trt文件"));
        fileDialog->setNameFilterDetailsVisible(true);
        fileDialog->setNameFilter( "*.trt *.txt" );
        fileDialog->setFileMode( QFileDialog::ExistingFile );

        if(QDialog::Rejected == fileDialog->exec())
        {
            return;
        }

        QStringList fileNames = fileDialog->selectedFiles();
        if(fileNames.size() > 0)
        {
            this->trtFolder_->setText(fileNames.at(0));
        }

        if( true == OpenTrtFile(trtFolder_->text()) )
        {
            genVectorUChar(customData_, nullptr, X_SIZE, Y_SIZE, Z_SIZE, this->rangeColorDialog_);
            modifier_->toggleCustomData(customData_);
        }
    });

    auto openTrtLayout = new QHBoxLayout;
    openTrtLayout->addWidget(trtTitle);
    openTrtLayout->addWidget(trtFolder_);
    openTrtLayout->addWidget(selectFolderButton);

    auto sliceXLayout = new QHBoxLayout;
    sliceXLayout->addWidget(sliceXCheckBox);
    sliceXLayout->addWidget(sliceXSlider);
    vLayout->addLayout(sliceXLayout);
    vLayout->addWidget(sliceImageXLabel);

    auto sliceYLayout = new QHBoxLayout;
    sliceYLayout->addWidget(sliceYCheckBox);
    sliceYLayout->addWidget(sliceYSlider);
    vLayout->addLayout(sliceYLayout);
    vLayout->addWidget(sliceImageYLabel);


    auto sliceZLayout = new QHBoxLayout;
    sliceZLayout->addWidget(sliceZCheckBox);
    sliceZLayout->addWidget(sliceZSlider);
    vLayout->addLayout(sliceZLayout);
    vLayout->addWidget(sliceImageZLabel);
    vLayout->addSpacing(20);
    vLayout->addWidget(drawSliceFramesCheckBox, 1, Qt::AlignTop);
    vLayout->addSpacing(20);
    vLayout->addWidget(colorSetButton_);
    vLayout->addLayout(openTrtLayout);

    modifier_->setSliceSliders(sliceXSlider, sliceYSlider, sliceZSlider);
    modifier_->setSliceLabels(sliceImageXLabel, sliceImageYLabel, sliceImageZLabel);

    QObject::connect(sliceXCheckBox, &QCheckBox::stateChanged, modifier_,
                     &VolumetricModifier::sliceX);
    QObject::connect(sliceYCheckBox, &QCheckBox::stateChanged, modifier_,
                     &VolumetricModifier::sliceY);
    QObject::connect(sliceZCheckBox, &QCheckBox::stateChanged, modifier_,
                     &VolumetricModifier::sliceZ);
    QObject::connect(sliceXSlider, &QSlider::valueChanged, modifier_,
                     &VolumetricModifier::adjustSliceX);
    QObject::connect(sliceYSlider, &QSlider::valueChanged, modifier_,
                     &VolumetricModifier::adjustSliceY);
    QObject::connect(sliceZSlider, &QSlider::valueChanged, modifier_,
                     &VolumetricModifier::adjustSliceZ);

    QObject::connect(drawSliceFramesCheckBox, &QCheckBox::stateChanged, modifier_,
                     &VolumetricModifier::setDrawSliceFrames);

    QObject::connect(colorSetButton_, &QPushButton::clicked, [this]
    {
        this->rangeColorDialog_->exec();
        genVectorUChar(customData_, nullptr, X_SIZE, Y_SIZE, Z_SIZE, this->rangeColorDialog_);
        this->modifier_->toggleCustomData(customData_);
        updateColorsetButton();
    });

    scrollArea_->setWidget(mainWidget);
    setWidget(scrollArea_);

#ifdef QT_DEBUG

    OpenTrtFile("1.trt");
    trtFolder_->setText("1.trt");

    //rangeColorDialog_->addControlPoints(XmlManager.getColorPoints());

    genVectorUChar(customData_, nullptr, X_SIZE,  Y_SIZE, Z_SIZE, this->rangeColorDialog_);
    modifier_->toggleCustomData(customData_);
#endif
}

bool Inversion3dRightDockWidget::OpenTrtFile(const QString& trtFile)
{
    int xLength = 0;
    int yLength = 0;
    int zLength = 0;
    float* data = nullptr;
    float min;
    float max;

    if(trtFile.endsWith(".trt"))
    {
        return readTrtFile(trtFile, data, xLength, yLength, zLength, min, max);
    }

    if (trtFile.endsWith(".txt"))
    {
         return readTxtTypeFile(trtFile, data, xLength, yLength, zLength, min, max);
    }

    return false;
}

bool Inversion3dRightDockWidget::saveColorPoints()
{
    //XmlManager.setColorPoints(rangeColorDialog_->getColorPoints());
    return true;
}

void Inversion3dRightDockWidget::resizeEvent(QResizeEvent *event)
{
    updateColorsetButton();
    WtgeoDockWidget::resizeEvent(event);
}

void Inversion3dRightDockWidget::updateColorsetButton()
{
    if(colorSetButton_)
    {
        auto size = colorSetButton_->size();
        size.setHeight(25);
        auto iconSize = QSize(size.width()-5, 20);
        QPixmap preColorPixmap = rangeColorDialog_->toQPixmap(iconSize);
        colorSetButton_->setFixedSize(size);
        colorSetButton_->setIconSize(preColorPixmap.size());
        colorSetButton_->setIcon(preColorPixmap);
    }
}
