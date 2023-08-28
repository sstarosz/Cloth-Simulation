#ifndef GUI_TOOLBAR_HPP_
#define GUI_TOOLBAR_HPP_

#include <QToolBar>

namespace st::ui::gui
{
    class ToolBar : public QToolBar
    {
        Q_OBJECT
    public:
        explicit ToolBar(const QString &title, QWidget *parent);
        
    private:
        // Add private members here

    signals:
        void resetPlanePosition();

    private slots:
        void resetPlanePositionSlot();

    };

}

#endif //  GUI_TOOLBAR_HPP_