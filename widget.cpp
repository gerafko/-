#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    checkRepeat = 1;

    objectText = new QTextEdit();
    objectText -> setFont(QFont("Courier",12));

    relationText = new QTextEdit();
    relationText -> setFont(QFont("Courier",12));

    propertiesText = new QTextEdit();
    propertiesText -> setFont(QFont("Courier",12));

    evidenceText = new QTextEdit();
    evidenceText -> setFont(QFont("Courier",12));

    ok = new QPushButton("ok");
    connect(ok,SIGNAL(clicked()),this,SLOT(creatNew()));


    resultText = new QLabel();
    resultText -> setStyleSheet("background: white;");
    resultText -> setFont(QFont("Courier",12));
    resultText -> setMinimumSize(200,700);
    scrollresultText = new QScrollArea;
    scrollresultText -> setWidget(resultText);
    scrollresultText -> setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollresultText -> setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    QVBoxLayout* layoutV = new QVBoxLayout();

    layoutV -> addWidget(new QLabel("Object"));
    layoutV -> addWidget(objectText);
    layoutV -> addWidget(new QLabel("Relation (0 - Contains, 1 - PartOf, 2 - IsA)"));
    layoutV -> addWidget(relationText);
    layoutV -> addWidget(new QLabel("Properties"));
    layoutV -> addWidget(propertiesText);
    layoutV -> addWidget(new QLabel("EvidenceCriterion"));
    layoutV -> addWidget(evidenceText);
    layoutV -> addWidget(ok);

    QHBoxLayout* layout = new QHBoxLayout();
    layout -> addLayout(layoutV);
    layout -> addWidget(scrollresultText);

    setLayout(layout);
}

Widget::~Widget()
{

}

void Widget::logic(QString relations)
{
    if (relations.size() != 5) return;
    QString from = relations.at(0);
    QString to = relations.at(2);
    QString type = relations.at(4);

    switch (type.toInt())
    {
    case 0://ifContainsType
    {
        if (!isExist(QString(to + "," + from + "," + "1")))
            relation.push_back(QString(to + "," + from + "," + "1"));

        for (int i = 0; i < relation.size(); i++)
        {
            if (relation.at(i).size() != 5) continue;
            if (relation.at(i).at(0) == to)
            {
                if (((relation.at(i).at(4) == QString("0")))||((relation.at(i).at(4) == QString("2"))))
                {
                    if (!isExist(QString(from + "," + relation.at(i).at(2) + "," +  "0")))
                        relation.push_back(QString(from + "," + relation.at(i).at(2) + "," +  "0"));
                }
            }

        }

        for (int i = 0; i < properties.size(); i++)
        {
            if (properties.at(i).size() < 1) continue;
            if (properties.at(i).at(0) == to)
            {
                QString property;
                property.push_back(from);
                for (int k = 1; k < properties.at(i).size(); k++)
                    property.push_back(properties.at(i).at(k));
                if (!isExist(property))
                    properties.push_back(property);
            }
        }
    }
        break;

    case 1://ifPartOfType
    {
        if (!isExist(QString(to + "," + from + "," + "0")))
            relation.push_back(QString(to + "," + from + "," + "0"));

        for (int i = 0; i < relation.size(); i++)
        {
            if (relation.at(i).size() != 5) continue;
            if (relation.at(i).at(0) == to)
            {
                if (relation.at(i).at(4) == QString("1"))
                {
                    if(!isExist(QString(from + "," + relation.at(i).at(2) + "," +  "1")))
                        relation.push_back(QString(from + "," + relation.at(i).at(2) + "," +  "1"));
                }
            }
        }
    }
        break;

    case 2://ifIsA
    {

        if (!isExist(QString(to + "," + from + "," + "2")))
            relation.push_back(QString(to + "," + from + "," + "2"));

        for (int i = 0; i < properties.size(); i++)
        {
            if (properties.at(i).size() < 1) continue;
            if (properties.at(i).at(0) == to)
            {
                QString property;
                property.push_back(from);
                for (int k = 1; k < properties.at(i).size(); k++)
                    property.push_back(properties.at(i).at(k));
                if (!isExist(property))
                    properties.push_back(property);
            }
        }

        for (int i = 0; i < relation.size(); i++)
        {
            if (relation.at(i).size() != 5) continue;
            if (relation.at(i).at(0) == to)
            {
                if(!isExist(QString(from + "," + relation.at(i).at(2) + "," +  relation.at(i).at(4))))
                    relation.push_back(QString(from + "," + relation.at(i).at(2) + "," +  relation.at(i).at(4)));
            }

        }
    }
        break;

    default:
        break;
    }
}

void Widget::creatObjectWithRP()
{
    if (!objectText -> toPlainText().isEmpty()) object = objectText -> toPlainText().split("\n");

    if (!relationText -> toPlainText().isEmpty()) relation = relationText -> toPlainText().split("\n");

    if (!propertiesText -> toPlainText().isEmpty()) properties = propertiesText -> toPlainText().split("\n");

    if (!evidenceText -> toPlainText().isEmpty()) evidenceCriterion = evidenceText -> toPlainText().split("\n");
}

bool Widget::isExist(QString propertyOrRelation)
{
    for (int i = 0; i < relation.size(); i++)
    {
        if (propertyOrRelation == relation.at(i))
            return true;
    }
    for (int i = 0; i < properties.size(); i++)
    {
        if (propertyOrRelation == properties.at(i))
            return true;
    }
    checkRepeat++;
    return false;
}

void Widget::creatNew()
{
    resultText -> clear();

    object.clear();
    properties.clear();
    relation.clear();
    evidenceCriterion.clear();
    checkRepeat = 1;

    creatObjectWithRP();

    while (checkRepeat != 0)
    {
        checkRepeat = 0;
        for (int i = 0; i < object.size(); i++)
        {
            if (object.at(i).size() < 1) continue;
            for (int j = 0; j < relation.size(); j++)
            {
                if (relation.at(j).size() != 5) continue;
                if (relation.at(j).at(0) == object.at(i).at(0))
                    logic(relation.at(j));
            }
        }
    }

    QString textOutput;

    for (int i = 0; i < object.size(); i++)
    {
        textOutput.push_back(object.at(i) + "\n");
    }

    for (int i = 0; i < relation.size(); i++)
    {
        if (relation.at(i).size() != 5) continue;
        QString type = "";
        if (relation.at(i).at(4) == QString("0")) type = " Contains";
        else if (relation.at(i).at(4) == QString("1")) type = " PartOf";
        else
            type = " IsA";
        textOutput.push_back(relation.at(i) + type + "\n");
    }

    for (int i = 0; i < properties.size(); i++)
    {
        textOutput.push_back(properties.at(i) + "\n");
    }

    int check = 0;

    for (int k = 0; k < evidenceCriterion.size(); k++)
    {
        for (int j = 0; j < relation.size(); j++)
        {
            if (relation.at(j) == evidenceCriterion.at(k))
            {
                check++;
            }
        }
        for (int j = 0; j < properties.size(); j++)
        {
            if (properties.at(j) == evidenceCriterion.at(k))
            {
                check++;
            }
        }
    }

    if ((check == evidenceCriterion.size())&&(evidenceCriterion.size() != 0))
        textOutput.push_back("Proven");
    else
        textOutput.push_back("Not provwn");

    resultText -> setText(textOutput);
    resultText -> resize(resultText -> sizeHint());
}
