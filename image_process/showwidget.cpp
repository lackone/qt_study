#include "showwidget.h"

ShowWidget::ShowWidget(QWidget *parent) : QWidget{parent}
{
    m_image_label = new QLabel(this);
    //缩放其内容以填充所有可用空间
    m_image_label->setScaledContents(true);
    //文本编辑
    m_txt_edit = new QTextEdit(this);
    //布局
    m_layout = new QHBoxLayout(this);
    m_layout->addWidget(m_image_label);
    m_layout->addWidget(m_txt_edit);
}

void ShowWidget::setImage(QPixmap img) { m_image_label->setPixmap(img); }

QTextEdit *ShowWidget::getTxtEdit() { return m_txt_edit; }

QLabel *ShowWidget::getImgLabel() { return m_image_label; }
