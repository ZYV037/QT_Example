#include <QTableWidget>
#include <QHBoxLayout>

QTableWidget table_;
QWidget* checkBoxWidget = new QWidget(table_);
QCheckBox* cbox = new QCheckBox(checkBoxWidget);
QHBoxLayout* layoutCheckBox = new QHBoxLayout(checkBoxWidget);
layoutCheckBox->addWidget(cbox);
layoutCheckBox->setAlignment(Qt::AlignCenter);
layoutCheckBox->setContentsMargins(0,0,0,0);
table_->setCellWidget(row, 0, checkBoxWidget);

//Use
QCheckBox* check_item = dynamic_cast<QCheckBox*>(table_->cellWidget(i,0)->layout()->itemAt(0)->widget());