#include "SettingsView.hpp"

#include "GuiElements/Slider.hpp"
#include "GuiElements/WidgetBox.hpp"

#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>

namespace st::ui::gui
{
	SettingsView::SettingsView()
	{
		setMinimumSize(600, 800);
		QVBoxLayout* mainLayout = new QVBoxLayout(this);

		mainLayout->addWidget(new QLabel("Test"));
		mainLayout->addWidget(new QLabel("Test"));




		/*StSliderBase example*/
		mainLayout->addWidget(createHorizontalLine());
		mainLayout->addWidget(new IntSlider("Color", 0, 255, this));


		/*WidgetBox example*/
		mainLayout->addWidget(createHorizontalLine());
		mainLayout->addWidget(new WidgetBox());

	}

	QFrame* SettingsView::createHorizontalLine()
	{
		QFrame* line = new QFrame();
		line->setObjectName(QString::fromUtf8("line"));
		line->setGeometry(QRect(320, 150, 118, 3));
		line->setFrameShape(QFrame::HLine);
		line->setFrameShadow(QFrame::Sunken);
		return line;
	}
}

