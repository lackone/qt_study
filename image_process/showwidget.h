#ifndef SHOWWIDGET_H
#define SHOWWIDGET_H

#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QWidget>

class ShowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowWidget(QWidget *parent = nullptr);
    void       setImage(QPixmap img);
    QTextEdit *getTxtEdit();
    QLabel *   getImgLabel();

private:
    QLabel *     m_image_label;
    QTextEdit *  m_txt_edit;
    QHBoxLayout *m_layout;
signals:
};

#endif  // SHOWWIDGET_H
