#ifndef IMGPROCESS_H
#define IMGPROCESS_H

#include <QActionGroup>
#include <QColorDialog>
#include <QFileDialog>
#include <QFontComboBox>
#include <QMainWindow>
#include <QPainter>
#include <QPrintDialog>
#include <QPrinter>
#include <QTextCharFormat>
#include <QTextList>
#include <QToolBar>
#include <QToolButton>

#include "showwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class ImgProcess;
}
QT_END_NAMESPACE

class ImgProcess : public QMainWindow
{
    Q_OBJECT

public:
    ImgProcess(QWidget *parent = nullptr);
    ~ImgProcess();
    void createActions();   //创建动作
    void createMenus();     //创建菜单
    void createToolBars();  //创建工具栏
    void loadFile(QString filename);
    void mergeFormat(QTextCharFormat);

private:
    Ui::ImgProcess *ui;
    QMenu *         m_fileMenu;  //文件菜单
    QMenu *         m_zoomMenu;
    QMenu *         m_rotateMenu;
    QMenu *         m_mirrorMenu;
    QImage          m_image;     //图片
    QString         m_fileName;  //文件名
    ShowWidget *    m_showWidget;
    QAction *       m_openFileAction;
    QAction *       m_newFileAction;
    QAction *       m_printTextAction;
    QAction *       m_printImageAction;
    QAction *       m_exitAction;
    QAction *       m_copyAction;  //编辑菜单项
    QAction *       m_cutAction;
    QAction *       m_pasteAction;
    QAction *       m_aboutAction;
    QAction *       m_zoomInAction;
    QAction *       m_zoomOutAction;
    QAction *       m_rotate90Action;  //旋转菜单项
    QAction *       m_rotate180Action;
    QAction *       m_rotate270Action;
    QAction *       m_mirrorVerticalAction;  //镜像菜单项
    QAction *       m_mirrorHorizontalAction;
    QAction *       m_undoAction;
    QAction *       m_redoAction;
    QToolBar *      m_fileTool;  //工具栏
    QToolBar *      m_zoomTool;
    QToolBar *      m_rotateTool;
    QToolBar *      m_mirrorTool;
    QToolBar *      m_doToolBar;
    QLabel *        m_fontLabel1;  //字体设置项
    QFontComboBox * m_fontComboBox;
    QLabel *        m_fontLabel2;
    QComboBox *     m_sizeComboBox;
    QToolButton *   m_boldBtn;
    QToolButton *   m_italicBtn;
    QToolButton *   m_underlineBtn;
    QToolButton *   m_colorBtn;
    QToolBar *      m_fontToolBar;  //字体工具栏
    QLabel *        m_listLabel;    //排序设置项
    QComboBox *     m_listComboBox;
    QActionGroup *  m_actGrp;
    QAction *       m_leftAction;
    QAction *       m_rightAction;
    QAction *       m_centerAction;
    QAction *       m_justifyAction;
    QToolBar *      m_listToolBar;  //排序工具栏

protected slots:
    void ShowNewFile();
    void ShowOpenFile();
    void ShowPrintText();
    void ShowPrintImage();
    void ShowZoomIn();
    void ShowZoomOut();
    void ShowRotate90();
    void ShowRotate180();
    void ShowRotate270();
    void ShowMirrorVertical();
    void ShowMirrorHorizontal();
    void ShowFontComboBox(QFont comboFont);
    void ShowSizeSpinBox(QString spinValue);
    void ShowBoldBtn();
    void ShowItalicBtn();
    void ShowUnderlineBtn();
    void ShowColorBtn();
    void ShowCurrentFormatChanged(const QTextCharFormat &fmt);
    void ShowList(int);
    void ShowAlignment(QAction *act);
    void ShowCursorPositionChanged();
};
#endif  // IMGPROCESS_H
