#include "settingpage.h"

CSettingPage::CSettingPage(QWidget *parent): QWidget(parent)
{
#if 0
    m_groupbox_curePeriodSet = new QGroupBox(tr("period"));
    m_label_curePeriodSet = new QLabel();
    m_label_curePeriodSet->setGeometry(204,138,147,27);
    m_label_curePeriodIcon = new QLabel();
    m_label_curePeriodIcon->setGeometry(228,193,30,30);
    m_groupbox_curePeriodSet->setStyleSheet(
                    "QGroupBox"
                              "{"
                              "border: 2px solid black;"
                              "border-radius:8px;"
                              "margin-top:6px;"
                              "}"
                              "QGroupBox:title"
                              "{"
                              "color:black;"
                              "subcontrol-origin: margin;"
                              "left: 10px;"
                              "}"

                      );

    m_groupbox_cureTempSet = new QGroupBox(tr("temprate"));
    m_label_cureTempSet = new QLabel();
    m_label_cureTempSet->setGeometry(388,141,147,27);
    m_label_cureTempIcon = new QLabel();      
    m_label_cureTempIcon->setGeometry(383,166,14,30);
    m_groupbox_cureTempSet->setStyleSheet(
                    "QGroupBox"
                              "{"
                              "border: 2px solid black;"
                              "border-radius:8px;"
                              "margin-top:6px;"
                              "}"
                              "QGroupBox:title"
                              "{"
                              "color:black;"
                              "subcontrol-origin: margin;"
                              "left: 10px;"
                              "}"

                      );


    m_groupbox_curePowerSet = new QGroupBox(tr("power"));
    m_label_curePowerSet = new QLabel();
    m_label_curePowerSet->setGeometry(388,141,147,27);
    m_label_curePowerIcon = new QLabel();
    m_label_curePowerIcon->setGeometry(576,169,30,30);
    m_groupbox_curePowerSet->setStyleSheet(
                    "QGroupBox"
                              "{"
                              "border: 2px solid black;"
                              "border-radius:8px;"
                              "margin-top:6px;"
                              "}"
                              "QGroupBox:title"
                              "{"
                              "color:black;"
                              "subcontrol-origin: margin;"
                              "left: 10px;"
                              "}"

                      );


    m_groupbox_coldWaterSet = new QGroupBox(tr("water"));
    m_label_coldWaterSet = new QLabel();
    m_label_coldWaterSet->setGeometry(388,141,147,27);
    m_label_coldWaterIcon = new QLabel();
    m_label_coldWaterIcon->setGeometry(767,171,30,30);
    m_groupbox_coldWaterSet->setStyleSheet(
                    "QGroupBox"
                              "{"
                              "border: 2px solid black;"
                              "border-radius:8px;"
                              "margin-top:6px;"
                              "}"
                              "QGroupBox:title"
                              "{"
                              "color:black;"
                              "subcontrol-origin: margin;"
                              "left: 10px;"
                              "}"

                      );

    m_box_curePeriodSet = new QComboBox();
    m_box_cureTempSet = new QComboBox();
    m_box_curePowerSet = new QComboBox();
    m_box_coldWaterSet = new QComboBox();

    initQComboBox();

    vbox_period = new QVBoxLayout;
    vbox_period->addWidget(m_label_curePeriodSet);
    vbox_period->addWidget(m_label_curePeriodIcon);
    vbox_period->addWidget(m_box_curePeriodSet);
    m_groupbox_curePeriodSet->setLayout(vbox_period);

    vbox_temp = new QVBoxLayout;
    vbox_temp->addWidget(m_label_curePeriodSet);
    vbox_temp->addWidget(m_label_curePeriodIcon);
    vbox_temp->addWidget(m_box_curePeriodSet);
    m_groupbox_cureTempSet->setLayout(vbox_temp);

    vbox_power = new QVBoxLayout;
    vbox_power->addWidget(m_label_curePeriodSet);
    vbox_power->addWidget(m_label_curePeriodIcon);
    vbox_power->addWidget(m_box_curePeriodSet);
    m_groupbox_curePowerSet->setLayout(vbox_power);

    vbox_water = new QVBoxLayout;
    vbox_water->addWidget(m_label_curePeriodSet);
    vbox_water->addWidget(m_label_curePeriodIcon);
    vbox_water->addWidget(m_box_curePeriodSet);
    m_groupbox_coldWaterSet->setLayout(vbox_water);
#endif
}
#if 0
void CSettingPage::initQComboBox()
{
    m_box_curePeriodSet->addItem(QWidget::tr("1:00"));
    m_box_curePeriodSet->addItem(QWidget::tr("1:10"));
    m_box_curePeriodSet->addItem(QWidget::tr("1:20"));
    m_box_curePeriodSet->addItem(QWidget::tr("1:30"));
    m_box_curePeriodSet->addItem(QWidget::tr("1:40"));
    m_box_curePeriodSet->addItem(QWidget::tr("1:50"));
    m_box_curePeriodSet->addItem(QWidget::tr("2:00"));
    m_box_curePeriodSet->addItem(QWidget::tr("2:10"));
    m_box_curePeriodSet->addItem(QWidget::tr("2:20"));
    m_box_curePeriodSet->addItem(QWidget::tr("2:30"));
    m_box_curePeriodSet->addItem(QWidget::tr("2:40"));
    m_box_curePeriodSet->addItem(QWidget::tr("2:50"));
    m_box_curePeriodSet->addItem(QWidget::tr("3:00"));
    m_box_curePeriodSet->addItem(QWidget::tr("3:10"));
    m_box_curePeriodSet->addItem(QWidget::tr("3:20"));
    m_box_curePeriodSet->addItem(QWidget::tr("3:30"));
    m_box_curePeriodSet->addItem(QWidget::tr("3:40"));
    m_box_curePeriodSet->addItem(QWidget::tr("3:50"));
    m_box_curePeriodSet->addItem(QWidget::tr("4:00"));
    m_box_curePeriodSet->addItem(QWidget::tr("4:10"));
    m_box_curePeriodSet->addItem(QWidget::tr("4:20"));
    m_box_curePeriodSet->addItem(QWidget::tr("4:30"));
    m_box_curePeriodSet->addItem(QWidget::tr("4:40"));
    m_box_curePeriodSet->addItem(QWidget::tr("4:50"));
    m_box_curePeriodSet->addItem(QWidget::tr("5:00"));
    m_box_curePeriodSet->addItem(QWidget::tr("5:10"));
    m_box_curePeriodSet->addItem(QWidget::tr("5:20"));
    m_box_curePeriodSet->addItem(QWidget::tr("5:30"));
    m_box_curePeriodSet->addItem(QWidget::tr("5:40"));
    m_box_curePeriodSet->addItem(QWidget::tr("5:50"));
    m_box_curePeriodSet->addItem(QWidget::tr("6:00"));
    m_box_curePeriodSet->addItem(QWidget::tr("6:10"));
    m_box_curePeriodSet->addItem(QWidget::tr("6:20"));
    m_box_curePeriodSet->addItem(QWidget::tr("6:30"));
    m_box_curePeriodSet->addItem(QWidget::tr("6:40"));
    m_box_curePeriodSet->addItem(QWidget::tr("6:50"));
    m_box_curePeriodSet->addItem(QWidget::tr("7:00"));
    m_box_curePeriodSet->addItem(QWidget::tr("7:10"));
    m_box_curePeriodSet->addItem(QWidget::tr("7:20"));
    m_box_curePeriodSet->addItem(QWidget::tr("7:30"));
    m_box_curePeriodSet->addItem(QWidget::tr("7:40"));
    m_box_curePeriodSet->addItem(QWidget::tr("7:50"));
    m_box_curePeriodSet->addItem(QWidget::tr("8:00"));
    m_box_curePeriodSet->addItem(QWidget::tr("8:10"));
    m_box_curePeriodSet->addItem(QWidget::tr("8:20"));
    m_box_curePeriodSet->addItem(QWidget::tr("8:30"));
    m_box_curePeriodSet->addItem(QWidget::tr("8:40"));
    m_box_curePeriodSet->addItem(QWidget::tr("8:50"));
    m_box_curePeriodSet->addItem(QWidget::tr("9:00"));
    m_box_curePeriodSet->addItem(QWidget::tr("9:10"));
    m_box_curePeriodSet->addItem(QWidget::tr("9:20"));
    m_box_curePeriodSet->addItem(QWidget::tr("9:30"));
    m_box_curePeriodSet->addItem(QWidget::tr("9:40"));
    m_box_curePeriodSet->addItem(QWidget::tr("9:50"));
    m_box_curePeriodSet->addItem(QWidget::tr("10:00"));





    m_box_cureTempSet->addItem(QWidget::tr("50"));
    m_box_cureTempSet->addItem(QWidget::tr("51"));
    m_box_cureTempSet->addItem(QWidget::tr("52"));
    m_box_cureTempSet->addItem(QWidget::tr("53"));
    m_box_cureTempSet->addItem(QWidget::tr("54"));
    m_box_cureTempSet->addItem(QWidget::tr("55"));
    m_box_cureTempSet->addItem(QWidget::tr("56"));
    m_box_cureTempSet->addItem(QWidget::tr("57"));
    m_box_cureTempSet->addItem(QWidget::tr("58"));
    m_box_cureTempSet->addItem(QWidget::tr("59"));
    m_box_cureTempSet->addItem(QWidget::tr("60"));
    m_box_cureTempSet->addItem(QWidget::tr("61"));
    m_box_cureTempSet->addItem(QWidget::tr("62"));
    m_box_cureTempSet->addItem(QWidget::tr("63"));
    m_box_cureTempSet->addItem(QWidget::tr("64"));
    m_box_cureTempSet->addItem(QWidget::tr("65"));
    m_box_cureTempSet->addItem(QWidget::tr("66"));
    m_box_cureTempSet->addItem(QWidget::tr("67"));
    m_box_cureTempSet->addItem(QWidget::tr("68"));
    m_box_cureTempSet->addItem(QWidget::tr("69"));
    m_box_cureTempSet->addItem(QWidget::tr("70"));
    m_box_cureTempSet->addItem(QWidget::tr("71"));
    m_box_cureTempSet->addItem(QWidget::tr("72"));
    m_box_cureTempSet->addItem(QWidget::tr("73"));
    m_box_cureTempSet->addItem(QWidget::tr("74"));
    m_box_cureTempSet->addItem(QWidget::tr("75"));
    m_box_cureTempSet->addItem(QWidget::tr("76"));
    m_box_cureTempSet->addItem(QWidget::tr("77"));
    m_box_cureTempSet->addItem(QWidget::tr("78"));
    m_box_cureTempSet->addItem(QWidget::tr("79"));
    m_box_cureTempSet->addItem(QWidget::tr("80"));
    m_box_cureTempSet->addItem(QWidget::tr("81"));
    m_box_cureTempSet->addItem(QWidget::tr("82"));
    m_box_cureTempSet->addItem(QWidget::tr("83"));
    m_box_cureTempSet->addItem(QWidget::tr("84"));
    m_box_cureTempSet->addItem(QWidget::tr("85"));
    m_box_cureTempSet->addItem(QWidget::tr("86"));
    m_box_cureTempSet->addItem(QWidget::tr("87"));
    m_box_cureTempSet->addItem(QWidget::tr("88"));
    m_box_cureTempSet->addItem(QWidget::tr("89"));
    m_box_cureTempSet->addItem(QWidget::tr("90"));
    m_box_cureTempSet->addItem(QWidget::tr("91"));
    m_box_cureTempSet->addItem(QWidget::tr("92"));
    m_box_cureTempSet->addItem(QWidget::tr("93"));
    m_box_cureTempSet->addItem(QWidget::tr("94"));
    m_box_cureTempSet->addItem(QWidget::tr("95"));




    m_box_curePowerSet->addItem(QWidget::tr("0.1"));
    m_box_curePowerSet->addItem(QWidget::tr("0.5"));
    m_box_curePowerSet->addItem(QWidget::tr("1.0"));
    m_box_curePowerSet->addItem(QWidget::tr("1.5"));
    m_box_curePowerSet->addItem(QWidget::tr("2.0"));
    m_box_curePowerSet->addItem(QWidget::tr("2.5"));
    m_box_curePowerSet->addItem(QWidget::tr("3.0"));
    m_box_curePowerSet->addItem(QWidget::tr("3.5"));
    m_box_curePowerSet->addItem(QWidget::tr("4.0"));
    m_box_curePowerSet->addItem(QWidget::tr("4.5"));
    m_box_curePowerSet->addItem(QWidget::tr("5.0"));
    m_box_curePowerSet->addItem(QWidget::tr("5.5"));
    m_box_curePowerSet->addItem(QWidget::tr("6.0"));
    m_box_curePowerSet->addItem(QWidget::tr("6.5"));
    m_box_curePowerSet->addItem(QWidget::tr("7.0"));
    m_box_curePowerSet->addItem(QWidget::tr("7.5"));
    m_box_curePowerSet->addItem(QWidget::tr("8.0"));
    m_box_curePowerSet->addItem(QWidget::tr("8.5"));
    m_box_curePowerSet->addItem(QWidget::tr("9.0"));
    m_box_curePowerSet->addItem(QWidget::tr("9.5"));
    m_box_curePowerSet->addItem(QWidget::tr("10.0"));




    m_box_coldWaterSet->addItem(QWidget::tr("25.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("25.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("26.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("26.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("27.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("27.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("28.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("28.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("29.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("29.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("30.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("30.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("31.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("31.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("32.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("32.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("33.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("33.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("34.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("34.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("35.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("35.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("36.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("36.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("37.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("37.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("38.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("38.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("39.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("39.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("40.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("40.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("41.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("41.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("42.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("42.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("43.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("43.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("44.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("44.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("45.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("45.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("46.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("46.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("47.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("47.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("48.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("48.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("49.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("49.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("50.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("50.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("51.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("51.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("52.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("52.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("53.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("53.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("54.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("54.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("55.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("55.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("56.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("56.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("57.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("57.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("58.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("58.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("59.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("59.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("60.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("60.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("61.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("61.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("62.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("62.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("63.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("63.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("64.0"));
    m_box_coldWaterSet->addItem(QWidget::tr("64.5"));
    m_box_coldWaterSet->addItem(QWidget::tr("65.0"));

    //m_QComboBox_curepos->setEditable(true);
//    m_QComboBox_curepos->setMaxVisibleItems(10);
//     m_QComboBox_curepos->insertItem(81, tr("Insert item"));
//     m_QComboBox_curepos->insertSeparator(81);
}
#endif
