#include "TransformationView.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QFrame>

namespace st::ui::gui {

	TransformationView::TransformationView()
	{
		setMinimumSize(600, 800);
		QVBoxLayout* mainLayout = new QVBoxLayout(this);

		mainLayout->addWidget(new QLabel("Translate"));


		//Transformation
		// 

		//Left Right
		QHBoxLayout* leftRightLayout = new QHBoxLayout();
		QPushButton* leftButton = new QPushButton("Left");
		QPushButton* rightButton = new QPushButton("Right");
		QLineEdit* leftRightLineEdit = new QLineEdit("0.0");


		connect(leftButton, &QPushButton::clicked, this,
			[=]() {
				transform(TranslationDirection::Left, leftRightLineEdit->text());
			});

		connect(rightButton, &QPushButton::clicked, this,
			[=]() {
				transform(TranslationDirection::Right, leftRightLineEdit->text());
			});

		leftRightLayout->addWidget(leftButton);
		leftRightLayout->addWidget(leftRightLineEdit);
		leftRightLayout->addWidget(rightButton);


		QHBoxLayout* downUpLayout = new QHBoxLayout();
		QPushButton* downButton = new QPushButton("Down");
		QLineEdit* downUpLineEdit = new QLineEdit("0.0");
		QPushButton* upButton = new QPushButton("Up");

		connect(downButton, &QPushButton::clicked, this,
			[=]() {
				transform(TranslationDirection::Bottom, downUpLineEdit->text());
			});

		connect(upButton, &QPushButton::clicked, this,
			[=]() {
				transform(TranslationDirection::Top, downUpLineEdit->text());
			});


		downUpLayout->addWidget(downButton);
		downUpLayout->addWidget(downUpLineEdit);
		downUpLayout->addWidget(upButton);


		//Front Back
		QHBoxLayout* frontBackLayout = new QHBoxLayout();
		QPushButton* backButton = new QPushButton("Back");
		QLineEdit* backFrontLineEdit = new QLineEdit("0.0");
		QPushButton* frontButton = new QPushButton("Front");


		connect(backButton, &QPushButton::clicked, this,
			[=]() {
				transform(TranslationDirection::Back, backFrontLineEdit->text());
			});

		connect(frontButton, &QPushButton::clicked, this,
			[=]() {
				transform(TranslationDirection::Front, backFrontLineEdit->text());
			});

		frontBackLayout->addWidget(backButton);
		frontBackLayout->addWidget(backFrontLineEdit);
		frontBackLayout->addWidget(frontButton);

		mainLayout->addLayout(leftRightLayout);
		mainLayout->addLayout(downUpLayout);
		mainLayout->addLayout(frontBackLayout);

	}


	void TransformationView::transform(TranslationDirection direction, QString value)
	{
		switch (direction)
		{
		case st::ui::gui::TranslationDirection::Left:
			qDebug() << "Left" << value;
			break;
		case st::ui::gui::TranslationDirection::Right:
			qDebug() << "Right" << value;
			break;
		case st::ui::gui::TranslationDirection::Bottom:
			qDebug() << "Bottom" << value;
			break;
		case st::ui::gui::TranslationDirection::Top:
			qDebug() << "Top" << value;
			break;
		case st::ui::gui::TranslationDirection::Back:
			qDebug() << "Back" << value;
			break;
		case st::ui::gui::TranslationDirection::Front:
			qDebug() << "Front" << value;
			break;
		default:
			break;
		}
	}




	QFrame* TransformationView::createHorizontalLine()
	{
		QFrame* line = new QFrame();
		line->setObjectName(QString::fromUtf8("line"));
		line->setGeometry(QRect(320, 150, 118, 3));
		line->setFrameShape(QFrame::HLine);
		line->setFrameShadow(QFrame::Sunken);
		return line;
	}
}
