#pragma once
#include <QComboBox>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QToolButton>

class WtUserComboBox : public QComboBox
{
    Q_OBJECT
public:
    WtUserComboBox(QWidget *parent = 0);
    ~WtUserComboBox();
public:
    void increaseItem(QString text);//添加下拉项
    void removeItem(QString account);//删除下拉项,返回该项的位置
    int getListWidgetCount();//获取下拉项的个数
    bool findItem(QString account);
    QStringList getListStr();
    void SetUsers(const QString& users);
signals:
    void SignalShowAccount(QString account);
    void SignalRemoveItem(int i);private slots :
    void SlotShowAccount(QString);
    void SlotRemoveAccount(QString);

public slots:
    void itemEntered( QListWidgetItem *item );

private:
    QListWidget *m_pListWidget;
};

//自定义的下拉项
class AccountItem : public QWidget
{
    Q_OBJECT
public:
    AccountItem(QWidget *parent);
    ~AccountItem();

public:	void setAccountNumber(QString account_text);

    QString getAccountNumber();
private:
    void mousePressEvent(QMouseEvent *event);
signals:
    void SignalRemoveAccount(QString);
    void SignalShowAccount(QString);

private slots:
    void removeAccount();

private:
    bool m_Mouse_press;
    QLabel * m_pAccount_number;
    QToolButton * m_pDelede_button;

    friend class WtUserComboBox;
};

