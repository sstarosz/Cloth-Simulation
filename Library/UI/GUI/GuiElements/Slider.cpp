#include "Slider.hpp"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QSlider>
#include <QLabel>
#include <QIntValidator>
#include <QDoubleValidator>

#include <limits>

namespace st::ui::gui
{
	StBaseSlider::StBaseSlider(QString name, QWidget* parent):
		QWidget(parent),
		m_pName(new QLabel(name)),
		m_pInputValue(new QLineEdit("Empty Line Edit")),
		m_pSlider(new QSlider(Qt::Orientation::Horizontal))
	{
		QHBoxLayout* mainLayout = new QHBoxLayout(this);

		//Label

		//Input Value
		//setupValidator();
		//connect(m_pInputValue, &QLineEdit::editingFinished,
		//		this, &Slider::editingFinished);

		//Slider
		m_pSlider->setRange(0, 100);
		m_pInputValue->setText(QString::number(m_pSlider->value()));
		//connect(m_pSlider, &QSlider::valueChanged,
		//	this, &Slider::updateEditLine);


		mainLayout->addWidget(m_pName);
		mainLayout->addWidget(m_pInputValue);
		mainLayout->addWidget(m_pSlider);

		setMinimumSize(400, 200);
	}

	//template<typename type>
	//void Slider<type>::setupValidator()
	//{
	//	switch (m_sliderDataType)
	//	{
	//	case SliderType::UINT:
	//		m_pInputValue->setValidator(new QIntValidator(0, 100, this));
	//		break;
	//	case SliderType::INT:
	//		m_pInputValue->setValidator(new QIntValidator(-100, 100, this));
	//		break;
	//	case SliderType::DOUBLE:
	//		m_pInputValue->setValidator(new QDoubleValidator(-100.0, 100.0, 10, this));
	//		break;
	//	case SliderType::UDOBULE:
	//		m_pInputValue->setValidator(new QDoubleValidator(0.0, 100.0, 10, this));
	//		break;
	//	default:
	//		break;
	//	}
	//}


	//template<typename type>
	//void Slider<type>::updateEditLine(int value)
	//{
	//	m_pInputValue->setText(QString::number(value));
	//}

	//template<typename type>
	//void Slider<type>::editingFinished()
	//{
	//	m_pName->setText("Test");
	//}


	void StBaseSlider::setValidator(QValidator* validator)
	{
		m_pInputValue->setValidator(validator);
	}



	/*------------------------------------------------------
	----------------------IntSlider-------------------------
	------------------------------------------------------*/


    void IntStSlider::setLimit(int64_t minValue, int64_t maxValue)
    {
		m_minLimit = minValue;
		m_maxLimit = maxValue;
    }

	int64_t IntStSlider::getMinLimit() const
	{
		return m_minLimit;
	}

	int64_t IntStSlider::getMaxLimit() const
	{
		return m_maxLimit;
	}


    IntStSlider::IntStSlider(QWidget* parent):
		IntStSlider(("None name"),
					std::numeric_limits<int64_t>::min(),
					std::numeric_limits<int64_t>::max(),
					parent)
    {
	}

	IntStSlider::IntStSlider(QString name,
						 int64_t minValue,
						 int64_t maxValue,
						 QWidget* parent):
		StBaseSlider(name, parent),
		m_minLimit(minValue),
		m_maxLimit(maxValue)
	{
		setValidator(new QIntValidator(m_minLimit, m_maxLimit, this));
	}



}