#include "ToolBar.hpp"

namespace st::ui::gui
{
    ToolBar::ToolBar(const QString &title, QWidget *parent):
    QToolBar(parent)
    {
        setWindowTitle(title);
        setObjectName(title);

        QAction* action1 = new QAction("Reset Plane Position", this);
        QAction* action2 = new QAction("Action 2", this);


        addAction(action1);
        addAction(action2);

        connect(action1, &QAction::triggered, this, &ToolBar::resetPlanePositionSlot);

    }

    void ToolBar::resetPlanePositionSlot()
    {
        emit resetPlanePosition();
    }

}
