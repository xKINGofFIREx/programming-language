#ifndef MAINWIDGET_HPP_
#define MAINWIDGET_HPP_

#include "interpreter.h"
#include <QWidget>

class QPushButton;
class QTextBrowser;
class QTextEdit;

class MainWidget : public QWidget {
    Q_OBJECT
  public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

  private slots:
    void handleButton();

  private:
    QPushButton *m_button;
    QTextBrowser *m_text_browser_out;
    QTextEdit *m_text_edit;
    Interpreter ip;
};

#endif // MAINWIDGET_HPP_
