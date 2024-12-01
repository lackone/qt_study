#include "imgprocess.h"

#include "ui_imgprocess.h"

ImgProcess::ImgProcess(QWidget *parent) : QMainWindow(parent), ui(new Ui::ImgProcess)
{
    ui->setupUi(this);

    setWindowTitle(tr("文本编辑器"));
    m_showWidget = new ShowWidget(this);
    setCentralWidget(m_showWidget);

    //在工具栏上嵌入控件
    //设置字体
    m_fontLabel1   = new QLabel(tr("字体:"));
    m_fontComboBox = new QFontComboBox;
    m_fontComboBox->setFontFilters(QFontComboBox::ScalableFonts);
    m_fontLabel2   = new QLabel(tr("字号:"));
    m_sizeComboBox = new QComboBox;
    QFontDatabase db;
    foreach (int size, db.standardSizes()) {
        m_sizeComboBox->addItem(QString::number(size));
    }
    m_boldBtn = new QToolButton;
    m_boldBtn->setIcon(QIcon(":/res/bold.png"));
    m_boldBtn->setCheckable(true);
    m_italicBtn = new QToolButton;
    m_italicBtn->setIcon(QIcon(":/res/italic.png"));
    m_italicBtn->setCheckable(true);
    m_underlineBtn = new QToolButton;
    m_underlineBtn->setIcon(QIcon(":/res/underline.png"));
    m_underlineBtn->setCheckable(true);
    m_colorBtn = new QToolButton;
    m_colorBtn->setIcon(QIcon(":/res/color.png"));
    m_colorBtn->setCheckable(true);
    //排序
    m_listLabel    = new QLabel(tr("排序"));
    m_listComboBox = new QComboBox;
    m_listComboBox->addItem("Standard");
    m_listComboBox->addItem("QTextListFormat::ListDisc");
    m_listComboBox->addItem("QTextListFormat::ListCircle");
    m_listComboBox->addItem("QTextListFormat::ListSquare");
    m_listComboBox->addItem("QTextListFormat::ListDecimal");
    m_listComboBox->addItem("QTextListFormat::ListLowerAlpha");
    m_listComboBox->addItem("QTextListFormat::ListUpperAlpha");
    m_listComboBox->addItem("QTextListFormat::ListLowerRoman");
    m_listComboBox->addItem("QTextListFormat::ListUpperRoman");

    //创建动作、菜单、工具栏
    createActions();
    createMenus();
    createToolBars();

    //加载图片
    if (m_image.load(":/res/image.png")) {
        m_showWidget->setImage(QPixmap::fromImage(m_image));
    }

    connect(m_fontComboBox, SIGNAL(currentFontChanged(QFont)), this, SLOT(ShowFontComboBox(QFont)));
    connect(m_sizeComboBox, SIGNAL(textActivated(QString)), this, SLOT(ShowSizeSpinBox(QString)));
    connect(m_boldBtn, SIGNAL(clicked()), this, SLOT(ShowBoldBtn()));
    connect(m_italicBtn, SIGNAL(clicked()), this, SLOT(ShowItalicBtn()));
    connect(m_underlineBtn, SIGNAL(clicked()), this, SLOT(ShowUnderlineBtn()));
    connect(m_colorBtn, SIGNAL(clicked()), this, SLOT(ShowColorBtn()));
    connect(m_showWidget->getTxtEdit(), SIGNAL(currentCharFormatChanged(QTextCharFormat &)), this, SLOT(ShowCurrentFormatChanged(QTextCharFormat &)));

    connect(m_listComboBox, SIGNAL(activated(int)), this, SLOT(ShowList(int)));
    connect(m_showWidget->getTxtEdit()->document(), SIGNAL(undoAvailable(bool)), m_redoAction, SLOT(setEnabled(bool)));
    connect(m_showWidget->getTxtEdit()->document(), SIGNAL(redoAvailable(bool)), m_redoAction, SLOT(setEnabled(bool)));
    connect(m_showWidget->getTxtEdit(), SIGNAL(cursorPositionChanged()), this, SLOT(ShowCursorPositionChanged()));
}

ImgProcess::~ImgProcess() { delete ui; }

void ImgProcess::createActions()
{
    //“打开”动作
    m_openFileAction = new QAction(QIcon(":/res/open.png"), tr("打开"), this);
    m_openFileAction->setShortcut(tr("Ctrl+O"));
    m_openFileAction->setStatusTip(tr("打开一个文件"));
    connect(m_openFileAction, SIGNAL(triggered()), this, SLOT(ShowOpenFile()));

    //“新建”动作
    m_newFileAction = new QAction(QIcon(":/res/new.png"), tr("新建"), this);
    m_newFileAction->setShortcut(tr("Ctrl+N"));
    m_newFileAction->setStatusTip(tr("新建一个文件"));
    connect(m_newFileAction, SIGNAL(triggered()), this, SLOT(ShowNewFile()));

    //“退出”动作
    m_exitAction = new QAction(tr("退出"), this);
    m_exitAction->setShortcut(tr("Ctrl+Q"));
    m_exitAction->setStatusTip(tr("退出程序"));
    connect(m_exitAction, SIGNAL(triggered()), this, SLOT(close()));

    //“复制”动作
    m_copyAction = new QAction(QIcon(":/res/copy.png"), tr("复制"), this);
    m_copyAction->setShortcut(tr("Ctrl+C"));
    m_copyAction->setStatusTip(tr("复制文件"));
    connect(m_copyAction, SIGNAL(triggered()), m_showWidget->getTxtEdit(), SLOT(copy()));

    //“剪切”动作
    m_cutAction = new QAction(QIcon(":/res/cut.png"), tr("剪切"), this);
    m_cutAction->setShortcut(tr("Ctrl+X"));
    m_cutAction->setStatusTip(tr("剪切文件"));
    connect(m_cutAction, SIGNAL(triggered()), m_showWidget->getTxtEdit(), SLOT(cut()));

    //“粘贴”动作
    m_pasteAction = new QAction(QIcon(":/res/paste.png"), tr("粘贴"), this);
    m_pasteAction->setShortcut(tr("Ctrl+V"));
    m_pasteAction->setStatusTip(tr("粘贴文件"));
    connect(m_pasteAction, SIGNAL(triggered()), m_showWidget->getTxtEdit(), SLOT(paste()));

    //“关于”动作
    m_aboutAction = new QAction(tr("关于"), this);
    connect(m_aboutAction, SIGNAL(triggered()), this, SLOT(QApplication::aboutQt()));

    //“打印文本”动作
    m_printTextAction = new QAction(QIcon(":/res/printText.png"), tr("打印文本"), this);
    m_printTextAction->setStatusTip(tr("打印一个文本"));
    connect(m_printTextAction, SIGNAL(triggered()), this, SLOT(ShowPrintText()));

    //“打印图像”动作
    m_printImageAction = new QAction(QIcon(":/res/printImage.png"), tr("打印图像"), this);
    m_printImageAction->setStatusTip(tr("打印一幅图像"));
    connect(m_printImageAction, SIGNAL(triggered()), this, SLOT(ShowPrintImage()));

    //“放大”动作
    m_zoomInAction = new QAction(QIcon(":/res/zoomin.png"), tr("放大"), this);
    m_zoomInAction->setStatusTip(tr("放大一张图片"));
    connect(m_zoomInAction, SIGNAL(triggered()), this, SLOT(ShowZoomIn()));

    //“缩小”动作
    m_zoomOutAction = new QAction(QIcon(":/res/zoomout.png"), tr("缩小"), this);
    m_zoomOutAction->setStatusTip(tr("缩小一张图片"));
    connect(m_zoomOutAction, SIGNAL(triggered()), this, SLOT(ShowZoomOut()));

    //实现图像旋转的动作（Action）
    //旋转90°
    m_rotate90Action = new QAction(QIcon(":/res/rotate90.png"), tr("旋转90°"), this);
    m_rotate90Action->setStatusTip(tr("将一幅图旋转90°"));
    connect(m_rotate90Action, SIGNAL(triggered()), this, SLOT(ShowRotate90()));

    //旋转180°
    m_rotate180Action = new QAction(QIcon(":/res/rotate180.png"), tr("旋转180°"), this);
    m_rotate180Action->setStatusTip(tr("将一幅图旋转180°"));
    connect(m_rotate180Action, SIGNAL(triggered()), this, SLOT(ShowRotate180()));

    //旋转270°
    m_rotate270Action = new QAction(QIcon(":/res/rotate270.png"), tr("旋转270°"), this);
    m_rotate270Action->setStatusTip(tr("将一幅图旋转270°"));
    connect(m_rotate270Action, SIGNAL(triggered()), this, SLOT(ShowRotate270()));

    //实现图像镜像的动作（Action）
    //纵向镜像
    m_mirrorVerticalAction = new QAction(QIcon(":/res/mirrorVertical.png"), tr("纵向镜像"), this);
    m_mirrorVerticalAction->setStatusTip(tr("对一幅图做纵向镜像"));
    connect(m_mirrorVerticalAction, SIGNAL(triggered()), this, SLOT(ShowMirrorVertical()));

    //横向镜像
    m_mirrorHorizontalAction = new QAction(QIcon(":/res/mirrorHorizontal.png"), tr("横向镜像"), this);
    m_mirrorHorizontalAction->setStatusTip(tr("对一幅图做横向镜像"));
    connect(m_mirrorHorizontalAction, SIGNAL(triggered()), this, SLOT(ShowMirrorHorizontal()));

    //排序：左对齐、右对齐、居中和两端对齐
    m_actGrp     = new QActionGroup(this);
    m_leftAction = new QAction(QIcon(":/res/left.png"), "左对齐", m_actGrp);
    m_leftAction->setCheckable(true);

    m_rightAction = new QAction(QIcon(":/res/right.png"), "右对齐", m_actGrp);
    m_rightAction->setCheckable(true);

    m_centerAction = new QAction(QIcon(":/res/center.png"), "居中", m_actGrp);
    m_centerAction->setCheckable(true);

    m_justifyAction = new QAction(QIcon(":/res/justify.png"), "两端对齐", m_actGrp);
    m_justifyAction->setCheckable(true);

    connect(m_actGrp, SIGNAL(triggered(QAction *)), this, SLOT(ShowAlignment(QAction *)));

    //实现撤销和恢复的动作（Action）
    //撤销和恢复
    m_undoAction = new QAction(QIcon(":/res/undo.png"), "撤销", this);
    connect(m_undoAction, SIGNAL(triggered()), m_showWidget->getTxtEdit(), SLOT(undo()));

    m_redoAction = new QAction(QIcon(":/res/redo.png"), "重做", this);
    connect(m_redoAction, SIGNAL(triggered()), m_showWidget->getTxtEdit(), SLOT(redo()));
}

void ImgProcess::createMenus()
{
    //文件菜单
    m_fileMenu = menuBar()->addMenu(tr("文件"));
    m_fileMenu->addAction(m_openFileAction);
    m_fileMenu->addAction(m_newFileAction);
    m_fileMenu->addAction(m_printTextAction);
    m_fileMenu->addAction(m_printImageAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAction);
    //缩放菜单
    m_zoomMenu = menuBar()->addMenu(tr("编辑"));
    m_zoomMenu->addAction(m_copyAction);
    m_zoomMenu->addAction(m_cutAction);
    m_zoomMenu->addAction(m_pasteAction);
    m_zoomMenu->addAction(m_aboutAction);
    m_zoomMenu->addSeparator();
    m_zoomMenu->addAction(m_zoomInAction);
    m_zoomMenu->addAction(m_zoomOutAction);
    //旋转菜单
    m_rotateMenu = menuBar()->addMenu(tr("旋转"));
    m_rotateMenu->addAction(m_rotate90Action);
    m_rotateMenu->addAction(m_rotate180Action);
    m_rotateMenu->addAction(m_rotate270Action);
    //镜像菜单
    m_mirrorMenu = menuBar()->addMenu(tr("镜像"));
    m_mirrorMenu->addAction(m_mirrorVerticalAction);
    m_mirrorMenu->addAction(m_mirrorHorizontalAction);
}

void ImgProcess::createToolBars()
{
    //文件工具条
    m_fileTool = addToolBar("File");
    m_fileTool->addAction(m_openFileAction);
    m_fileTool->addAction(m_newFileAction);
    m_fileTool->addAction(m_printTextAction);
    m_fileTool->addAction(m_printImageAction);
    //编辑工具条
    m_zoomTool = addToolBar("Edit");
    m_zoomTool->addAction(m_copyAction);
    m_zoomTool->addAction(m_cutAction);
    m_zoomTool->addAction(m_pasteAction);
    m_zoomTool->addSeparator();
    m_zoomTool->addAction(m_zoomInAction);
    m_zoomTool->addAction(m_zoomOutAction);
    //旋转工具条
    m_rotateTool = addToolBar("rotate");
    m_rotateTool->addAction(m_rotate90Action);
    m_rotateTool->addAction(m_rotate180Action);
    m_rotateTool->addAction(m_rotate270Action);
    //撤销和重做工具条
    m_doToolBar = addToolBar("doEdit");
    m_doToolBar->addAction(m_undoAction);
    m_doToolBar->addAction(m_redoAction);
    //字体工具条
    m_fontToolBar = addToolBar("Font");
    m_fontToolBar->addWidget(m_fontLabel1);
    m_fontToolBar->addWidget(m_fontComboBox);
    m_fontToolBar->addWidget(m_fontLabel2);
    m_fontToolBar->addWidget(m_sizeComboBox);
    m_fontToolBar->addSeparator();
    m_fontToolBar->addWidget(m_boldBtn);
    m_fontToolBar->addWidget(m_italicBtn);
    m_fontToolBar->addWidget(m_underlineBtn);
    m_fontToolBar->addSeparator();
    m_fontToolBar->addWidget(m_colorBtn);
    //排序工具条
    m_listToolBar = addToolBar("list");
    m_listToolBar->addWidget(m_listLabel);
    m_listToolBar->addWidget(m_listComboBox);
    m_listToolBar->addSeparator();
    m_listToolBar->addActions(m_actGrp->actions());
}

//新文件
void ImgProcess::ShowNewFile()
{
    ImgProcess *newImgProcessor = new ImgProcess;
    newImgProcessor->show();
}

//打开文件
void ImgProcess::ShowOpenFile()
{
    m_fileName = QFileDialog::getOpenFileName(this);
    if (!m_fileName.isEmpty()) {
        if (m_showWidget->getTxtEdit()->document()->isEmpty()) {
            loadFile(m_fileName);
        } else {
            ImgProcess *newImgProcessor = new ImgProcess;
            newImgProcessor->show();
            newImgProcessor->loadFile(m_fileName);
        }
    }
}

//加载文件
void ImgProcess::loadFile(QString filename)
{
    printf("file name:%s\n", filename.data());
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream textStream(&file);
        while (!textStream.atEnd()) {
            m_showWidget->getTxtEdit()->append(textStream.readLine());
            printf("read line\n");
        }
        printf("end\n");
    }
}

//打印文本
void ImgProcess::ShowPrintText()
{
    QPrinter     printer;
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec()) {
        //获得QTextEdit对象的文档
        QTextDocument *doc = m_showWidget->getTxtEdit()->document();
        doc->print(&printer);  //打印
    }
}

//打印图像
void ImgProcess::ShowPrintImage()
{
    QPrinter     printer;  //新建一个QPrinter对象
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec()) {
        QPainter painter(&printer);
        QRect    rect = painter.viewport();  //获得QPainter对象的视图矩形区域
        QSize    size = m_image.size();      //获得图像的大小
        /* 按照图形的比例大小重新设置视图矩形区域 */
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(m_image.rect());  //设置QPainter窗口大小为图像的大小
        painter.drawImage(0, 0, m_image);   //打印图像
    }
}

//放大
void ImgProcess::ShowZoomIn()
{
    if (m_image.isNull())  //有效性判断
        return;
    QTransform transform;  //声明一个QTransform类的实例
    transform.scale(2, 2);
    m_image = m_image.transformed(transform);
    //重新设置显示图形
    m_showWidget->getImgLabel()->setPixmap(QPixmap::fromImage(m_image));
}

//缩小
void ImgProcess::ShowZoomOut()
{
    if (m_image.isNull()) return;
    QTransform transform;
    transform.scale(0.5, 0.5);  //(a)
    m_image = m_image.transformed(transform);
    m_showWidget->getImgLabel()->setPixmap(QPixmap::fromImage(m_image));
}

//翻转90度
void ImgProcess::ShowRotate90()
{
    if (m_image.isNull()) return;
    QTransform transform;
    transform.rotate(90);
    m_image = m_image.transformed(transform);
    m_showWidget->getImgLabel()->setPixmap(QPixmap::fromImage(m_image));
}

//翻转180度
void ImgProcess::ShowRotate180()
{
    if (m_image.isNull()) return;
    QTransform transform;
    transform.rotate(180);
    m_image = m_image.transformed(transform);
    m_showWidget->getImgLabel()->setPixmap(QPixmap::fromImage(m_image));
}

//翻转270度
void ImgProcess::ShowRotate270()
{
    if (m_image.isNull()) return;
    QTransform transform;
    transform.rotate(270);
    m_image = m_image.transformed(transform);
    m_showWidget->getImgLabel()->setPixmap(QPixmap::fromImage(m_image));
}

//垂直显示
void ImgProcess::ShowMirrorVertical()
{
    if (m_image.isNull()) return;
    m_image = m_image.mirrored(false, true);
    m_showWidget->getImgLabel()->setPixmap(QPixmap::fromImage(m_image));
}

//水平显示
void ImgProcess::ShowMirrorHorizontal()
{
    if (m_image.isNull()) return;
    m_image = m_image.mirrored(true, false);
    m_showWidget->getImgLabel()->setPixmap(QPixmap::fromImage(m_image));
}

//
void ImgProcess::ShowFontComboBox(QFont comboFont)  //设置字体
{
    QTextCharFormat fmt;     //创建一个QTextCharFormat对象
    fmt.setFont(comboFont);  //选择的字体设置给QTextCharFormat对象
    mergeFormat(fmt);        //将新的格式应用到光标选区内的字符
}

//合并格式化
void ImgProcess::mergeFormat(QTextCharFormat format)
{
    QTextCursor cursor = m_showWidget->getTxtEdit()->textCursor();
    //获得编辑框中的光标
    if (!cursor.hasSelection()) cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    m_showWidget->getTxtEdit()->mergeCurrentCharFormat(format);
}

//
void ImgProcess::ShowSizeSpinBox(QString spinValue)  //设置字号
{
    QTextCharFormat fmt;
    fmt.setFontPointSize(spinValue.toFloat());
    m_showWidget->getTxtEdit()->mergeCurrentCharFormat(fmt);
}

void ImgProcess::ShowBoldBtn()  //设置文字显示加粗
{
    QTextCharFormat fmt;
    fmt.setFontWeight(m_boldBtn->isChecked() ? QFont::Bold : QFont::Normal);
    m_showWidget->getTxtEdit()->mergeCurrentCharFormat(fmt);
}

void ImgProcess::ShowItalicBtn()  //设置文字显示斜体
{
    QTextCharFormat fmt;
    fmt.setFontItalic(m_italicBtn->isChecked());
    m_showWidget->getTxtEdit()->mergeCurrentCharFormat(fmt);
}

void ImgProcess::ShowUnderlineBtn()  //设置文字加下画线
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(m_underlineBtn->isChecked());
    m_showWidget->getTxtEdit()->mergeCurrentCharFormat(fmt);
}

void ImgProcess::ShowColorBtn()  //设置文字颜色
{
    QColor color = QColorDialog::getColor(Qt::red, this);
    if (color.isValid()) {
        QTextCharFormat fmt;
        fmt.setForeground(color);
        m_showWidget->getTxtEdit()->mergeCurrentCharFormat(fmt);
    }
}

void ImgProcess::ShowCurrentFormatChanged(const QTextCharFormat &fmt)
{
    m_fontComboBox->setCurrentIndex(m_fontComboBox->findText(fmt.fontFamily()));
    m_sizeComboBox->setCurrentIndex(m_sizeComboBox->findText(QString::number(fmt.fontPointSize())));
    m_boldBtn->setChecked(fmt.font().bold());
    m_italicBtn->setChecked(fmt.fontItalic());
    m_underlineBtn->setChecked(fmt.fontUnderline());
}

void ImgProcess::ShowAlignment(QAction *act)
{
    if (act == m_leftAction) m_showWidget->getTxtEdit()->setAlignment(Qt::AlignLeft);
    if (act == m_rightAction) m_showWidget->getTxtEdit()->setAlignment(Qt::AlignRight);
    if (act == m_centerAction) m_showWidget->getTxtEdit()->setAlignment(Qt::AlignCenter);
    if (act == m_justifyAction) m_showWidget->getTxtEdit()->setAlignment(Qt::AlignJustify);
}

void ImgProcess::ShowCursorPositionChanged()
{
    if (m_showWidget->getTxtEdit()->alignment() == Qt::AlignLeft) m_leftAction->setChecked(true);
    if (m_showWidget->getTxtEdit()->alignment() == Qt::AlignRight) m_rightAction->setChecked(true);
    if (m_showWidget->getTxtEdit()->alignment() == Qt::AlignCenter) m_centerAction->setChecked(true);
    if (m_showWidget->getTxtEdit()->alignment() == Qt::AlignJustify) m_justifyAction->setChecked(true);
}

void ImgProcess::ShowList(int index)
{
    //获得编辑框的QTextCursor对象指针
    QTextCursor cursor = m_showWidget->getTxtEdit()->textCursor();
    if (index != 0) {
        QTextListFormat::Style style = QTextListFormat::ListDisc;
        switch (index)  //设置style属性值
        {
            default:
            case 1:
                style = QTextListFormat::ListDisc;
                break;
            case 2:
                style = QTextListFormat::ListCircle;
                break;
            case 3:
                style = QTextListFormat::ListSquare;
                break;
            case 4:
                style = QTextListFormat::ListDecimal;
                break;
            case 5:
                style = QTextListFormat::ListLowerAlpha;
                break;
            case 6:
                style = QTextListFormat::ListUpperAlpha;
                break;
            case 7:
                style = QTextListFormat::ListLowerRoman;
                break;
            case 8:
                style = QTextListFormat::ListUpperRoman;
                break;
        }
        /* 设置缩进值 */
        cursor.beginEditBlock();
        QTextBlockFormat blockFmt = cursor.blockFormat();
        QTextListFormat  listFmt;
        if (cursor.currentList()) {
            listFmt = cursor.currentList()->format();
        } else {
            listFmt.setIndent(blockFmt.indent() + 1);
            blockFmt.setIndent(0);
            cursor.setBlockFormat(blockFmt);
        }
        listFmt.setStyle(style);
        cursor.createList(listFmt);
        cursor.endEditBlock();
    } else {
        QTextBlockFormat bfmt;
        bfmt.setObjectIndex(-1);
        cursor.mergeBlockFormat(bfmt);
    }
}
