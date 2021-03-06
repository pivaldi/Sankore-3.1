#ifndef UBDOCKPALETTEWIDGET_H
#define UBDOCKPALETTEWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QString>

typedef enum
{
    eUBDockPaletteWidget_BOARD,
    eUBDockPaletteWidget_WEB,
    eUBDockPaletteWidget_DOCUMENT,
    eUBDockPaletteWidget_DESKTOP,
} eUBDockPaletteWidgetMode;

class UBDockPaletteWidget : public QWidget
{

    Q_OBJECT
public:
    UBDockPaletteWidget(QWidget* parent=0, const char* name="UBDockPaletteWidget");
    ~UBDockPaletteWidget();

    QPixmap iconToRight();
    QPixmap iconToLeft();
    QString name();

    void registerMode(eUBDockPaletteWidgetMode mode);

    QVector<eUBDockPaletteWidgetMode> GetRegisteredModes() { return mRegisteredModes; }
    bool visibleState(){return mVisibleState;}
    void setVisibleState(bool state){mVisibleState = state;}

signals:
    void hideTab(const QString& widgetName);
    void showTab(const QString& widgetName);

public slots:
    void slot_changeMode(eUBDockPaletteWidgetMode newMode);


protected:
    QPixmap mIconToRight;   // arrow like this: >
    QPixmap mIconToLeft;    // arrow like this: <
    QString mName;

    /* The current widget available mode list */
    QVector<eUBDockPaletteWidgetMode> mRegisteredModes;

    bool mVisibleState;
};

#endif // UBDOCKPALETTEWIDGET_H
