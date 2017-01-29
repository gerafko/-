#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QScrollArea>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
private:
    QPushButton* ok;
    QTextEdit* objectText;
    QTextEdit* relationText;
    QTextEdit* propertiesText;
    QTextEdit* evidenceText;
    QLabel* resultText;
    QStringList object;
    QStringList relation;
    QStringList properties;
    QStringList evidenceCriterion;
    QScrollArea* scrollresultText;
    void logic(QString relations);
    void creatObjectWithRP();
    bool isExist(QString propertyOrRelation);
    int checkRepeat;
private slots:
    void creatNew();
};

#endif // WIDGET_H
