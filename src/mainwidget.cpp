#include "mainwidget.h"
#include <QtWidgets>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent) {
    m_button = new QPushButton(tr("Запуск"));
    m_text_browser_out = new QTextBrowser();
    m_text_edit = new QTextEdit();

    m_text_edit->setFontPointSize(16);
    m_text_browser_out->setFontPointSize(16);

    m_text_edit->setText("начало\n"
                         "целое число1 := 1\n"
                         "вещественное число2 := 20.1\n"
                         "если число1 == 10\n"
                         "печать число1\n"
                         "печать число2\n"
                         "число1 := число1 + 5\n"
                         "печать число1\n"
                         "конецесли\n"
                         "вещественное вещ1 := 15.42\n"
                         "целое цел1 := 18\n"
                         "вещ1 := число2 + вещ1\n"
                         "печать вещ1\n"
                         "конец");

    connect(m_button, &QPushButton::clicked, this, &MainWidget::handleButton);
    QGridLayout *main_layout = new QGridLayout;
    main_layout->addWidget(m_text_edit, 0, 0);
    main_layout->addWidget(m_button, 1, 0);
    main_layout->addWidget(m_text_browser_out, 2, 0);
    setFixedSize(600, 1000);

    setLayout(main_layout);
    setWindowTitle(tr("Ильин А.В. язык программирования"));
}

MainWidget::~MainWidget() {
    delete m_button;
    delete m_text_browser_out;
    delete m_text_edit;
}

void MainWidget::handleButton() {
    std::string text = m_text_edit->toPlainText().toStdString();
    ip.set_text(text);
    m_text_browser_out->setText(QString::fromStdString(ip.compile()));
}
