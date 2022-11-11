#ifndef GUI_SETTINGS_VIEW_H
#define GUI_SETTINGS_VIEW_H

#include <QWidget>


class QFrame;


namespace st::ui::gui {

class SettingsView : public QWidget
{
	public:
		SettingsView();

	private:
		QFrame* createHorizontalLine();
};

}
#endif // !GUI_SETTINGS_VIEW_H
