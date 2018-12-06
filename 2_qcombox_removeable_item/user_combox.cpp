#include "user_combox.h"

WtUserComboBox::WtUserComboBox(QWidget *parent)	: QComboBox(parent)
{
    m_pListWidget = new QListWidget(this);
    this->setModel(m_pListWidget->model());
    this->setView(m_pListWidget);
    connect( m_pListWidget, SIGNAL(itemEntered(QListWidgetItem* )), this, SLOT(itemEntered(QListWidgetItem *)));
}

WtUserComboBox::~WtUserComboBox()
{
}

void WtUserComboBox::increaseItem(QString text)
{
    AccountItem *account_item = new AccountItem(this);
    account_item->setAccountNumber(text);
    connect(account_item, SIGNAL(SignalShowAccount(QString)), this, SLOT(SlotShowAccount(QString)));
    connect(account_item, SIGNAL(SignalRemoveAccount(QString)), this, SLOT(SlotRemoveAccount(QString)));
    QListWidgetItem *list_item = new QListWidgetItem(m_pListWidget);
    m_pListWidget->setItemWidget(list_item, account_item);
}

void WtUserComboBox::removeItem(QString account)
{
    int list_count = getListWidgetCount();
    for (int i = 0; i < list_count; i++)
    {
        QListWidgetItem *item = m_pListWidget->item(i);
        AccountItem *account_item = dynamic_cast<AccountItem *>(m_pListWidget->itemWidget(item));
        QString account_number = account_item->getAccountNumber();
        if (account == account_number)
        {
            m_pListWidget->takeItem(i);
            delete item;
            emit SignalRemoveItem(i);
        }
    }
}

int WtUserComboBox::getListWidgetCount()
{
    return m_pListWidget->count();
}

void WtUserComboBox::SlotShowAccount(QString account)
{
    setEditText(account);
    hidePopup();
    emit SignalShowAccount(account.trimmed());
}

void WtUserComboBox::SlotRemoveAccount(QString account)
{
    hidePopup();

    int list_count = getListWidgetCount();
    for (int i = 0; i < list_count; i++)
    {
        QListWidgetItem *item = m_pListWidget->item(i);
        AccountItem *account_item = dynamic_cast<AccountItem *>(m_pListWidget->itemWidget(item));
        QString account_number = account_item->getAccountNumber();
        if (account == account_number)
        {
            m_pListWidget->takeItem(i);
            delete item;
            emit SignalRemoveItem(i);
            break;
        }
    }
}


void WtUserComboBox::itemEntered(QListWidgetItem *item)
{
    AccountItem *account_item = dynamic_cast<AccountItem *>(m_pListWidget->itemWidget(item));
    if(account_item)
    {
        account_item->m_pAccount_number->setMargin(1);
    }
}

bool  WtUserComboBox::findItem(QString account)
{
    int list_count = getListWidgetCount();
    for (int i = 0; i < list_count; i++)
    {
        QListWidgetItem *item = m_pListWidget->item(i);
        AccountItem *account_item = dynamic_cast<AccountItem *>(m_pListWidget->itemWidget(item));
        QString account_number = account_item->getAccountNumber();
        if (account == account_number)
        {
            return true;
        }
    }
    return false;
}

QStringList WtUserComboBox::getListStr()
{
    QStringList lst;
    QSet<QString> usersSet;
    if(false == currentText().trimmed().isEmpty())
    {
        usersSet.insert( currentText().trimmed() );
        lst.push_back( currentText().trimmed() );
    }

    int list_count = getListWidgetCount();
    for (int i = 0; i < list_count; i++)
    {
        QListWidgetItem *item = m_pListWidget->item(i);
        AccountItem *account_item = dynamic_cast<AccountItem *>(m_pListWidget->itemWidget(item));
        QString account_number = account_item->getAccountNumber();
        if(false == account_number.trimmed().isEmpty() && false == usersSet.contains(account_number.trimmed()) )
        {
            usersSet.insert(account_number.trimmed());
            lst.push_back( account_number.trimmed() );
        }
    }
    return lst;
}

void WtUserComboBox::SetUsers(const QString& users)
{
    QStringList userList = users.split(",");
    foreach(QString userName, userList)
    {
        if(false == userName.trimmed().isEmpty())
        {
            increaseItem(userName.trimmed());
        }
    }

    setEditText(userList.first());
}

AccountItem::AccountItem(QWidget *parent): QWidget(parent)
{
    m_Mouse_press = false;
    m_pAccount_number = new QLabel(this);
    m_pAccount_number->setAlignment(Qt::AlignLeft);
    m_pAccount_number->setFixedHeight(24);
    m_pAccount_number->setStyleSheet("QLabel{ background-color:#FFFFFF }");
    m_pDelede_button = new QToolButton(this);
    m_pDelede_button->setFixedWidth(20);
    //m_pDelede_button->setStyleSheet("QToolButton{background-color:#FFFFFF} QToolButton:!hover:!pressed{ border-image:url(\"./images/close_button.png\");}");
    m_pDelede_button->setStyleSheet("QToolButton{background-color:#FFFFFF} QToolButton::!hover{ border-image:url(\"./images/close_button.png\");} QToolButton:hover{ border-image:url(\"./images/close_button_hovered.png\");}");
    connect(m_pDelede_button, SIGNAL( clicked()), this, SLOT(removeAccount()));
    QHBoxLayout *main_layout = new QHBoxLayout();
    main_layout->addWidget(m_pAccount_number, 0, Qt::AlignLeft);
    main_layout->addWidget(m_pDelede_button);
    main_layout->setContentsMargins(0,0,0,0);
    main_layout->setSpacing(0);
    this->setLayout(main_layout);
}

AccountItem::~AccountItem()
{
}

void AccountItem::setAccountNumber(QString account_text)
{
    m_pAccount_number->setText(account_text);
    m_pAccount_number->setAlignment(Qt::AlignCenter);
}

QString AccountItem::getAccountNumber()
{
    QString account_number_text = m_pAccount_number->text();
    return account_number_text;
}

void AccountItem::removeAccount()
{
    QString account_number_text = m_pAccount_number->text();
    emit SignalRemoveAccount(account_number_text);
}

void AccountItem::mousePressEvent(QMouseEvent */*event*/)
{
    emit SignalShowAccount(m_pAccount_number->text());
}
