#ifndef GUI_TRANSFORMATION_VIEW_H
#define GUI_TRANSFORMATION_VIEW_H

#include <QWidget>


class QFrame;

namespace st::ui::gui {

	enum class TranslationDirection : uint64_t
	{
		Left,
		Right,
		Bottom,
		Top,
		Back,
		Front
	};


	class TransformationView : public QWidget
	{



	public:
		TransformationView();

	private:
		QFrame* createHorizontalLine();



	public slots:
		void transform(TranslationDirection direction, QString value);

	};

}
#endif // !GUI_SETTINGS_VIEW_H
